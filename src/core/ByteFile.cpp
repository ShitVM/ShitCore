#include <svm/core/ByteFile.hpp>

#include <svm/IO.hpp>

#include <utility>

namespace svm::core {
	ByteFile::ByteFile(std::string path, std::vector<std::string> dependencies, ConstantPool&& constantPool, Structures&& structures,
		Functions&& functions, Mappings&& mappings, Instructions&& entrypoint) noexcept
		: ModuleBase<Functions>(std::move(path), std::move(dependencies), std::move(structures),
			std::move(functions), std::move(mappings)), m_ConstantPool(std::move(constantPool)),
		m_Entrypoint(std::move(entrypoint)) {}
	ByteFile::ByteFile(ByteFile&& byteFile) noexcept
		: ModuleBase<Functions>(std::move(byteFile)), m_ConstantPool(std::move(byteFile.m_ConstantPool)),
		m_Entrypoint(std::move(byteFile.m_Entrypoint)) {}

	ByteFile& ByteFile::operator=(ByteFile&& byteFile) noexcept {
		ModuleBase<Functions>::operator=(std::move(byteFile));

		m_ConstantPool = std::move(byteFile.m_ConstantPool);
		m_Entrypoint = std::move(byteFile.m_Entrypoint);
		return *this;
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