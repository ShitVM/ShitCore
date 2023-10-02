#include <svm/core/Parser.hpp>

#include <fstream>
#include <ios>
#include <stdexcept>

namespace svm::core {
	void Parser::Clear() noexcept {
		m_File.clear();
		m_Cursor = 0;

		m_ByteFile.Clear();
		m_ShitBFVersion = ShitBFVersion::Latest;
		m_ShitBCVersion = ShitBCVersion::Latest;
	}

	void Parser::Open(const std::filesystem::path& path) {
		std::ifstream stream(path, std::ifstream::binary);
		if (!stream)
			throw std::runtime_error("Failed to open the file.");

		stream.seekg(0, std::ifstream::end);

		const std::streamsize length = stream.tellg();

		stream.seekg(0, std::ifstream::beg);

		if (length) {
			std::vector<std::uint8_t> bytes(static_cast<std::size_t>(length));
			
			stream.read(reinterpret_cast<char*>(bytes.data()), length);

			if (stream.gcount() != length)
				throw std::runtime_error("Failed to read the file.");

			m_File = std::move(bytes);
		}

		m_Cursor = 0;

		m_ByteFile.Clear();
		m_ByteFile.SetPath(std::filesystem::canonical(path));
	}
	void Parser::Parse() {
		if (!m_File.size())
			throw std::runtime_error("Failed to parse the file. Invalid format.");

		static constexpr std::uint8_t magic[] = { 0x74, 0x68, 0x74, 0x68 };
		const auto [magicBegin, magicEnd] = ReadFile(4);
		if (!std::equal(magicBegin, magicEnd, magic))
			throw std::runtime_error("Failed to parse the file. Invalid format.");

		m_ShitBFVersion = ReadFile<ShitBFVersion>();
		m_ShitBCVersion = ReadFile<ShitBCVersion>();

		if (m_ShitBFVersion > ShitBFVersion::Latest ||
			m_ShitBFVersion < ShitBFVersion::Least) {

			throw std::runtime_error("Failed to parse the file. Incompatible ShitBF version.");
		} else if (
			m_ShitBCVersion > ShitBCVersion::Latest ||
			m_ShitBCVersion < ShitBCVersion::Least) {

			throw std::runtime_error("Failed to parse the file. Incompatible ShitBC version.");
		}

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
		if (const auto fundamentalType = GetFundamentalType(code);
			fundamentalType != NoneType) {

			return fundamentalType;
		} else if (
			const auto structureType = GetStructureType(structures, code);
			structureType != NoneType) {

			return structureType;
		} else {
			return m_ByteFile.GetMappings().GetStructureMapping(
				static_cast<std::uint32_t>(code)
				- static_cast<std::uint32_t>(TypeCode::Structure)
				- static_cast<std::uint32_t>(structures.size())).TempType;
		}
	}

	void Parser::ParseDependencies() {
		const auto depenCount = ReadFile<std::uint32_t>();
		std::vector<Dependency> dependencies(depenCount);

		for (auto& dependency : dependencies) {
			dependency.Path = ReadFileString();
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

		const auto singleCount = ReadFile<std::uint32_t>();
		std::vector<SingleObject> singlePool(singleCount);

		ParseConstants(singlePool);

		const auto doubleCount = ReadFile<std::uint32_t>();
		std::vector<DoubleObject> doublePool(doubleCount);

		ParseConstants(doublePool);

		m_ByteFile.SetConstantPool({
			std::move(intPool),
			std::move(longPool),
			std::move(singlePool),
			std::move(doublePool)
		});
	}
	void Parser::ParseStructures() {
		const auto structCount = ReadFile<std::uint32_t>();
		Structures structures(structCount);

		for (std::uint32_t i = 0; i < structCount; ++i) {
			structures[i].Type.Name = ReadFileString();
			structures[i].Name = structures[i].Type.Name;
			structures[i].Type.Code = static_cast<TypeCode>(
				i + static_cast<std::uint32_t>(TypeCode::Structure)
			);

			const auto fieldCount = ReadFile<std::uint32_t>();

			structures[i].Fields.resize(fieldCount);

			for (auto& field : structures[i].Fields) {
				const auto fieldTypeCode = ReadFile<std::uint32_t>();

				field.Type = GetType(structures, static_cast<TypeCode>(fieldTypeCode & 0x7FFFFFFF));
				field.Count = (fieldTypeCode >> 31 ? ReadFile<std::uint64_t>() : 0);
			}
		}

		m_ByteFile.SetStructures(std::move(structures));
	}
	void Parser::ParseFunctions() {
		const auto funcCount = ReadFile<std::uint32_t>();
		Functions functions(funcCount);

		for (auto& function : functions) {
			function.Name = ReadFileString();
			function.Arity = ReadFile<std::uint16_t>();
			function.HasResult = ReadFile<bool>();
			function.Instructions = ParseInstructions();
		}

		m_ByteFile.SetFunctions(std::move(functions));
	}
	Instructions Parser::ParseInstructions() {
		const auto labelCount = ReadFile<std::uint32_t>();
		std::vector<std::uint64_t> labels(labelCount);

		for (auto& label : labels) {
			label = ReadFile<std::uint64_t>();
		}

		const auto instCount = ReadFile<std::uint64_t>();
		std::vector<Instruction> insts(static_cast<std::size_t>(instCount));
		std::uint64_t offset = 0;

		for (auto& inst : insts) {
			inst.OpCode = ReadOpCode();
			inst.Offset = offset++;

			if (inst.HasOperand()) {
				inst.Operand = ReadFile<std::uint32_t>();

				offset += 4;
			}
		}

		return { std::move(labels), std::move(insts) };
	}

	OpCode Parser::ReadOpCode() noexcept {
		return ConvertOpCode(ReadFile<std::uint8_t>(), m_ShitBCVersion);
	}
}