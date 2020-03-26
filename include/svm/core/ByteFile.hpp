#pragma once

#include <svm/Function.hpp>
#include <svm/Instruction.hpp>
#include <svm/Structure.hpp>
#include <svm/core/ConstantPool.hpp>

#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace svm {
	struct Mapping final {
		std::uint32_t Module;
		std::uint32_t Index;
	};

	class Mappings final {
	private:
		std::vector<Mapping> m_StructureMappings;
		std::vector<Mapping> m_FunctionMappings;

	public:
		Mappings() = default;
		Mappings(std::vector<Mapping>&& structures, std::vector<Mapping>&& functions) noexcept;
		Mappings(Mappings&& mappings) noexcept;
		~Mappings() = default;

	public:
		Mappings& operator=(Mappings&& mappings) noexcept;
		bool operator==(const Mappings&) = delete;
		bool operator!=(const Mappings&) = delete;

	public:
		void Clear() noexcept;

		const Mapping& GetStructureMapping(std::uint32_t index) const noexcept;
		std::uint32_t GetStructureMappingCount() const noexcept;
		const Mapping& GetFunctionMapping(std::uint32_t index) const noexcept;
		std::uint32_t GetFunctionMappingCount() const noexcept;
	};
}

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

		void UpdateStructureCodes(std::uint32_t structureCodeOffset) noexcept;
	};

	std::ostream& operator<<(std::ostream& stream, const ByteFile& byteFile);
}