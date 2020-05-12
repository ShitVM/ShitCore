#pragma once

#include <svm/Function.hpp>
#include <svm/Instruction.hpp>
#include <svm/Mapping.hpp>
#include <svm/Structure.hpp>
#include <svm/core/ConstantPool.hpp>

#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace svm::core {
	class ByteFile {
	private:
		std::string m_Path;
		std::vector<std::string> m_Dependencies;
		ConstantPool m_ConstantPool;
		Structures m_Structures;
		Functions m_Functions;
		Mappings m_Mappings;
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

		std::string_view GetPath() const noexcept;
		void SetPath(std::string newPath) noexcept;
		const std::vector<std::string>& GetDependencies() const noexcept;
		std::vector<std::string>& GetDependencies() noexcept;
		void SetDependencies(std::vector<std::string> newDependencies) noexcept;
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
		const Mappings& GetMappings() const noexcept;
		Mappings& GetMappings() noexcept;
		void SetMappings(Mappings&& newMappings) noexcept;
		Instructions& GetEntrypoint() noexcept;
		void SetEntrypoint(Instructions&& newEntrypoint) noexcept;

		void UpdateStructureInfos(std::uint32_t module) noexcept;
	};

	std::ostream& operator<<(std::ostream& stream, const ByteFile& byteFile);
}