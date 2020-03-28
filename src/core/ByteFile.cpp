#include <svm/core/ByteFile.hpp>

#include <svm/IO.hpp>

#include <utility>

namespace svm {
	Mappings::Mappings(std::vector<Mapping>&& structures, std::vector<Mapping>&& functions) noexcept
		: m_StructureMappings(std::move(structures)), m_FunctionMappings(std::move(functions)) {}
	Mappings::Mappings(Mappings&& mappings) noexcept
		: m_StructureMappings(std::move(mappings.m_StructureMappings)), m_FunctionMappings(std::move(mappings.m_FunctionMappings)) {}

	Mappings& Mappings::operator=(Mappings&& mappings) noexcept {
		m_StructureMappings = std::move(mappings.m_StructureMappings);
		m_FunctionMappings = std::move(mappings.m_FunctionMappings);
		return *this;
	}

	void Mappings::Clear() noexcept {
		m_StructureMappings.clear();
		m_FunctionMappings.clear();
	}

	void Mappings::AddStructureMapping(std::uint32_t module, std::uint32_t structure) {
		m_StructureMappings.push_back({ module, structure });
	}
	void Mappings::AddFunctionMapping(std::uint32_t module, std::uint32_t structure) {
		m_FunctionMappings.push_back({ module, structure });
	}

	const Mapping& Mappings::GetStructureMapping(std::uint32_t index) const noexcept {
		return m_StructureMappings[index];
	}
	std::uint32_t Mappings::GetStructureMappingCount() const noexcept {
		return static_cast<std::uint32_t>(m_StructureMappings.size());
	}
	const Mapping& Mappings::GetFunctionMapping(std::uint32_t index) const noexcept {
		return m_FunctionMappings[index];
	}
	std::uint32_t Mappings::GetFunctionMappingCount() const noexcept {
		return static_cast<std::uint32_t>(m_FunctionMappings.size());
	}
}

namespace svm::core {
	ByteFile::ByteFile(std::string path, std::vector<std::string> dependencies, ConstantPool&& constantPool, Structures&& structures,
		Functions&& functions, Mappings&& mappings, Instructions&& entrypoint) noexcept
		: m_Path(std::move(path)), m_Dependencies(std::move(dependencies)), m_ConstantPool(std::move(constantPool)), m_Structures(std::move(structures)),
		m_Functions(std::move(functions)), m_Mappings(std::move(mappings)), m_Entrypoint(std::move(entrypoint)) {}
	ByteFile::ByteFile(ByteFile&& byteFile) noexcept
		: m_Path(std::move(byteFile.m_Path)), m_Dependencies(std::move(byteFile.m_Dependencies)), m_ConstantPool(std::move(byteFile.m_ConstantPool)),
		m_Structures(std::move(byteFile.m_Structures)), m_Functions(std::move(byteFile.m_Functions)), m_Mappings(std::move(byteFile.m_Mappings)),
		m_Entrypoint(std::move(byteFile.m_Entrypoint)) {}

	ByteFile& ByteFile::operator=(ByteFile&& byteFile) noexcept {
		m_Path = std::move(byteFile.m_Path);
		m_Dependencies = std::move(byteFile.m_Dependencies);
		m_ConstantPool = std::move(byteFile.m_ConstantPool);
		m_Structures = std::move(byteFile.m_Structures);
		m_Functions = std::move(byteFile.m_Functions);
		m_Mappings = std::move(byteFile.m_Mappings);
		m_Entrypoint = std::move(byteFile.m_Entrypoint);
		return *this;
	}

	void ByteFile::Clear() noexcept {
		m_Path.clear();
		m_Dependencies.clear();
		m_ConstantPool.Clear();
		m_Structures.clear();
		m_Functions.clear();
		m_Mappings.Clear();
		m_Entrypoint.Clear();
	}
	std::string_view ByteFile::GetPath() const noexcept {
		return m_Path;
	}
	void ByteFile::SetPath(std::string newPath) noexcept {
		m_Path = std::move(newPath);
	}
	const std::vector<std::string>& ByteFile::GetDependencies() const noexcept {
		return m_Dependencies;
	}
	std::vector<std::string>& ByteFile::GetDependencies() noexcept {
		return m_Dependencies;
	}
	void ByteFile::SetDependencies(std::vector<std::string> newDependencies) noexcept {
		m_Dependencies = std::move(newDependencies);
	}
	const ConstantPool& ByteFile::GetConstantPool() const noexcept {
		return m_ConstantPool;
	}
	ConstantPool& ByteFile::GetConstantPool() noexcept {
		return m_ConstantPool;
	}
	void ByteFile::SetConstantPool(ConstantPool&& newConstantPool) noexcept {
		m_ConstantPool = std::move(newConstantPool);
	}
	const Structures& ByteFile::GetStructures() const noexcept {
		return m_Structures;
	}
	Structures& ByteFile::GetStructures() noexcept {
		return m_Structures;
	}
	void ByteFile::SetStructures(Structures&& newStructures) noexcept {
		m_Structures = std::move(newStructures);
	}
	const Functions& ByteFile::GetFunctions() const noexcept {
		return m_Functions;
	}
	Functions& ByteFile::GetFunctions() noexcept {
		return m_Functions;
	}
	void ByteFile::SetFunctions(Functions&& newFunctions) noexcept {
		m_Functions = std::move(newFunctions);
	}
	const Instructions& ByteFile::GetEntrypoint() const noexcept {
		return m_Entrypoint;
	}
	const Mappings& ByteFile::GetMappings() const noexcept {
		return m_Mappings;
	}
	Mappings& ByteFile::GetMappings() noexcept {
		return m_Mappings;
	}
	void ByteFile::SetMappings(Mappings&& newMappings) noexcept {
		m_Mappings = std::move(newMappings);
	}
	Instructions& ByteFile::GetEntrypoint() noexcept {
		return m_Entrypoint;
	}
	void ByteFile::SetEntrypoint(Instructions&& newEntrypoint) noexcept {
		m_Entrypoint = std::move(newEntrypoint);
	}

	void ByteFile::UpdateStructureCodes(std::uint32_t offset) noexcept {
		for (std::uint32_t i = 0; i < m_Structures.size(); ++i) {
			m_Structures[i].Type.Code = static_cast<TypeCode>(static_cast<std::uint32_t>(m_Structures[i].Type.Code) + offset);
		}
	}

	std::ostream& operator<<(std::ostream& stream, const ByteFile& byteFile) {
		const std::string defIndent = detail::MakeIndent(stream);
		const std::string indentOnce(4, ' ');

		stream << defIndent << "ByteFile:\n"
			   << defIndent << indentOnce << "Path: \"" << byteFile.GetPath() << "\"\n"
			   << Indent << byteFile.GetConstantPool() << '\n'
						 << byteFile.GetStructures() << '\n'
						 << byteFile.GetFunctions() << '\n' << UnIndent
			   << defIndent << indentOnce << "Entrypoint:\n"
			   << Indent << Indent << byteFile.GetEntrypoint() << UnIndent << UnIndent;
		return stream;
	}
}