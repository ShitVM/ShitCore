#pragma once

#include <svm/Function.hpp>
#include <svm/Instruction.hpp>
#include <svm/Structure.hpp>
#include <svm/core/ConstantPool.hpp>

#include <string>
#include <string_view>

namespace svm::core {
	class ByteFile {
	private:
		std::string m_Path;
		ConstantPool m_ConstantPool;
		Structures m_Structures;
		Functions m_Functions;
		Instructions m_Entrypoint;

	public:
		ByteFile() noexcept = default;
		ByteFile(std::string path, ConstantPool&& constantPool, Structures&& structures,
			Functions&& functions, Instructions&& entrypoint) noexcept;
		ByteFile(ByteFile&& byteFile) noexcept;
		~ByteFile() = default;

	public:
		ByteFile& operator=(ByteFile&& byteFile) noexcept;
		bool operator==(const ByteFile&) = delete;
		bool operator!=(const ByteFile&) = delete;

	public:
		void Clear() noexcept;

		std::string_view GetPath() const noexcept;
		void SetPath(std::string newPath) noexcept;
		const ConstantPool& GetConstantPool() const noexcept;
		ConstantPool& GetConstantPool() noexcept;
		void SetConstantPool(ConstantPool&& newConstantPool) noexcept;
		const Structures& GetStructures() const noexcept;
		Structures& GetStructures() noexcept;
		void SetStructures(Structures&& newStructures) noexcept;
		const Functions& GetFunctions() const noexcept;
		Functions& GetFunctions() noexcept;
		void SetFunctions(Functions&& newFunctions) noexcept;
		const Instructions& GetEntrypoint() const noexcept;
		Instructions& GetEntrypoint() noexcept;
		void SetEntrypoint(Instructions&& newEntrypoint) noexcept;
	};
}