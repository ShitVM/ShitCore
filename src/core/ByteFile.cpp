#include <svm/core/ByteFile.hpp>

#include <svm/IO.hpp>

#include <utility>

namespace svm::core {
	ByteFile::ByteFile(std::string path, ConstantPool&& constantPool, Structures&& structures,
		Functions&& functions, Instructions&& entrypoint) noexcept
		: m_Path(std::move(path)), m_ConstantPool(std::move(constantPool)), m_Structures(std::move(structures)),
		m_Functions(std::move(functions)), m_Entrypoint(std::move(entrypoint)) {}
	ByteFile::ByteFile(ByteFile&& byteFile) noexcept
		: m_Path(std::move(byteFile.m_Path)), m_ConstantPool(std::move(byteFile.m_ConstantPool)), m_Structures(std::move(byteFile.m_Structures)),
		m_Functions(std::move(byteFile.m_Functions)), m_Entrypoint(std::move(byteFile.m_Entrypoint)) {}

	ByteFile& ByteFile::operator=(ByteFile&& byteFile) noexcept {
		m_Path = std::move(byteFile.m_Path);
		m_ConstantPool = std::move(byteFile.m_ConstantPool);
		m_Structures = std::move(byteFile.m_Structures);
		m_Functions = std::move(byteFile.m_Functions);
		m_Entrypoint = std::move(byteFile.m_Entrypoint);
		return *this;
	}

	void ByteFile::Clear() noexcept {
		m_Path.clear();
		m_ConstantPool.Clear();
		m_Structures.clear();
		m_Functions.clear();
		m_Entrypoint.Clear();
	}
	std::string_view ByteFile::GetPath() const noexcept {
		return m_Path;
	}
	void ByteFile::SetPath(std::string newPath) noexcept {
		m_Path = std::move(newPath);
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
	Instructions& ByteFile::GetEntrypoint() noexcept {
		return m_Entrypoint;
	}
	void ByteFile::SetEntrypoint(Instructions&& newEntrypoint) noexcept {
		m_Entrypoint = std::move(newEntrypoint);
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