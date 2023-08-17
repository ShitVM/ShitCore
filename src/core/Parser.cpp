#include <svm/core/Parser.hpp>

#include <fstream>
#include <ios>

namespace svm::core {
	Parser::Parser(Parser&& parser) noexcept
		: m_File(std::move(parser.m_File)), m_Cursor(parser.m_Cursor),
		m_ByteFile(std::move(parser.m_ByteFile)), m_ShitBFVersion(parser.m_ShitBFVersion), m_ShitBCVersion(parser.m_ShitBCVersion) {}

	Parser& Parser::operator=(Parser&& parser) noexcept {
		m_File = std::move(parser.m_File);
		m_Cursor = parser.m_Cursor;

		m_ByteFile = std::move(parser.m_ByteFile);
		m_ShitBFVersion = parser.m_ShitBFVersion;
		m_ShitBCVersion = parser.m_ShitBCVersion;
		return *this;
	}

	void Parser::Clear() noexcept {
		m_File.clear();
		m_Cursor = 0;

		m_ByteFile.Clear();
		m_ShitBFVersion = ShitBFVersion::Latest;
		m_ShitBCVersion = ShitBCVersion::Latest;
	}

	void Parser::Open(const std::filesystem::path& path) {
		std::ifstream stream(path, std::ifstream::binary);
		if (!stream) throw std::runtime_error("Failed to open the file.");

		stream.seekg(0, std::ifstream::end);
		const std::streamsize length = stream.tellg();
		stream.seekg(0, std::ifstream::beg);

		if (length) {
			std::vector<std::uint8_t> bytes(static_cast<std::size_t>(length));
			stream.read(reinterpret_cast<char*>(bytes.data()), length);
			if (stream.gcount() != length) throw std::runtime_error("Failed to read the file.");

			m_File = std::move(bytes);
		}

		m_Cursor = 0;

		m_ByteFile.Clear();
		m_ByteFile.SetPath(std::filesystem::canonical(path));
	}
	void Parser::Parse() {
		if (!m_File.size()) throw std::runtime_error("Failed to parse the file. Invalid format.");

		static constexpr std::uint8_t magic[] = { 0x74, 0x68, 0x74, 0x68 };
		const auto [magicBegin, magicEnd] = ReadFile(4);
		if (!std::equal(magicBegin, magicEnd, magic)) throw std::runtime_error("Failed to parse the file. Invalid format.");

		m_ShitBFVersion = ReadFile<ShitBFVersion>();
		m_ShitBCVersion = ReadFile<ShitBCVersion>();

		if (m_ShitBFVersion > ShitBFVersion::Latest ||
			m_ShitBFVersion < ShitBFVersion::Least) throw std::runtime_error("Failed to parse the file. Incompatible ShitBF version.");
		if (m_ShitBCVersion > ShitBCVersion::Latest ||
			m_ShitBCVersion < ShitBCVersion::Least) throw std::runtime_error("Failed to parse the file. Incompatible ShitBC version.");

		ParseDependencies();
		ParseMappings();
		ParseConstantPool();
		ParseStructures();
		ParseFunctions();
		m_ByteFile.SetEntrypoint(ParseInstructions());
	}
	ByteFile Parser::GetResult() noexcept {
		return std::move(m_ByteFile);
	}

	Type Parser::GetType(Structures& structures, TypeCode code) {
		auto result = GetFundamentalType(code);
		if (result != NoneType) return result;

		result = GetStructureType(structures, code);
		if (result != NoneType) return result;

		return m_ByteFile.GetMappings().GetStructureMapping(
			static_cast<std::uint32_t>(code)
			- static_cast<std::uint32_t>(TypeCode::Structure)
			- static_cast<std::uint32_t>(structures.size())).TempType;
	}

	void Parser::ParseDependencies() {
		const auto depenCount = ReadFile<std::uint32_t>();
		std::vector<Dependency> dependencies;
		for (std::uint32_t i = 0; i < depenCount; ++i) {
			dependencies.push_back({ ReadFileString() });
		}

		m_ByteFile.SetDependencies(std::move(dependencies));
	}
	void Parser::ParseMappings() {
		const auto structMappingCount = ReadFile<std::uint32_t>();
		std::vector<StructureMapping> structMappings(structMappingCount);
		ParseMappings(structMappings);

		for (auto& mapping : structMappings) {
			mapping.TempType.Name = mapping.Name;
			mapping.TempType.Module = mapping.Module + 1;
		}

		const auto funcMappingCount = ReadFile<std::uint32_t>();
		std::vector<FunctionMapping> funcMappings(funcMappingCount);
		ParseMappings(funcMappings);

		m_ByteFile.SetMappings({ std::move(structMappings), std::move(funcMappings) });
	}
	void Parser::ParseConstantPool() {
		const auto intCount = ReadFile<std::uint32_t>();
		std::vector<IntObject> intPool(intCount);
		ParseConstants(intPool);

		const auto longCount = ReadFile<std::uint32_t>();
		std::vector<LongObject> longPool(longCount);
		ParseConstants(longPool);

		const auto doubleCount = ReadFile<std::uint32_t>();
		std::vector<DoubleObject> doublePool(doubleCount);
		ParseConstants(doublePool);

		m_ByteFile.SetConstantPool({ std::move(intPool), std::move(longPool), std::move(doublePool) });
	}
	void Parser::ParseStructures() {
		const auto structCount = ReadFile<std::uint32_t>();
		Structures structures(structCount);
		for (std::uint32_t i = 0; i < structCount; ++i) {
			structures[i].Type.Name = ReadFileString();
			structures[i].Name = structures[i].Type.Name;
			structures[i].Type.Code = static_cast<TypeCode>(i + static_cast<std::uint32_t>(TypeCode::Structure));

			const auto fieldCount = ReadFile<std::uint32_t>();
			structures[i].Fields.resize(fieldCount);

			for (std::uint32_t j = 0; j < fieldCount; ++j) {
				Field& field = structures[i].Fields[j];

				const auto typeCode = ReadFile<std::uint32_t>();
				field.Type = GetType(structures, static_cast<TypeCode>(typeCode & 0x7FFFFFFF));
				if (typeCode >> 31) {
					field.Count = ReadFile<std::uint64_t>();
				}
			}
		}

		m_ByteFile.SetStructures(std::move(structures));
	}
	void Parser::ParseFunctions() {
		const auto funcCount = ReadFile<std::uint32_t>();
		Functions functions(funcCount);
		for (std::uint32_t i = 0; i < funcCount; ++i) {
			functions[i].Name = ReadFileString();
			functions[i].Arity = ReadFile<std::uint16_t>();
			functions[i].HasResult = ReadFile<bool>();
			functions[i].Instructions = ParseInstructions();
		}

		m_ByteFile.SetFunctions(std::move(functions));
	}
	Instructions Parser::ParseInstructions() {
		const auto labelCount = ReadFile<std::uint32_t>();
		std::vector<std::uint64_t> labels(labelCount);
		for (std::uint32_t i = 0; i < labelCount; ++i) {
			labels[i] = ReadFile<std::uint64_t>();
		}

		const auto instCount = ReadFile<std::uint64_t>();
		std::vector<Instruction> insts(static_cast<std::size_t>(instCount));

		std::uint64_t nextOffset = 0;
		for (std::size_t i = 0; i < instCount; ++i) {
			insts[i].OpCode = ReadOpCode();
			insts[i].Offset = nextOffset;
			if (insts[i].HasOperand()) {
				insts[i].Operand = ReadFile<std::uint32_t>();
				nextOffset += 4;
			}
			++nextOffset;
		}

		return { std::move(labels), std::move(insts) };
	}

	OpCode Parser::ReadOpCode() noexcept {
		return ConvertOpCode(ReadFile<std::uint8_t>(), m_ShitBCVersion);
	}
}