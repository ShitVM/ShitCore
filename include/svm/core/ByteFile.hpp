#pragma once

#include <svm/Function.hpp>
#include <svm/Instruction.hpp>
#include <svm/Mapping.hpp>
#include <svm/Structure.hpp>
#include <svm/core/ConstantPool.hpp>
#include <svm/core/ModuleBase.hpp>

#include <cstdint>
#include <ostream>
#include <string>

namespace svm::core {
	class ByteFile : public ModuleBase<Functions> {
	private:
		ConstantPool m_ConstantPool;
		Instructions m_Entrypoint;

	public:
		ByteFile() noexcept = default;
		ByteFile(std::string path, std::vector<std::string> dependencies, ConstantPool&& constantPool, Structures&& structures,
			Functions&& functions, Mappings&& mappings, Instructions&& entrypoint) noexcept;
		ByteFile(ByteFile&& byteFile) noexcept;
		~ByteFile() = default;

	public:
		ByteFile& operator=(ByteFile&& byteFile) noexcept;
		bool operator==(const ByteFile&) = delete;
		bool operator!=(const ByteFile&) = delete;

	public:
		void Clear() noexcept;

		const ConstantPool& GetConstantPool() const noexcept;
		ConstantPool& GetConstantPool() noexcept;
		void SetConstantPool(ConstantPool&& newConstantPool) noexcept;
		const Instructions& GetEntrypoint() const noexcept;
		Instructions& GetEntrypoint() noexcept;
		void SetEntrypoint(Instructions&& newEntrypoint) noexcept;

		void UpdateFunctionInfos(std::uint32_t module) noexcept;
	};

	std::ostream& operator<<(std::ostream& stream, const ByteFile& byteFile);
}