#pragma once

#include <svm/Type.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace svm {
	struct Mapping {
		std::uint32_t Module;
		std::string Name;
	};

	struct StructureMapping final : Mapping {
		TypeInfo TempType;
	};

	using FunctionMapping = Mapping;
}

namespace svm {
	class Mappings final {
	private:
		std::vector<StructureMapping> m_StructureMappings;
		std::vector<FunctionMapping> m_FunctionMappings;

	public:
		Mappings() = default;
		Mappings(std::vector<StructureMapping> structures, std::vector<FunctionMapping> functions) noexcept;
		Mappings(Mappings&& mappings) noexcept;
		~Mappings() = default;

	public:
		Mappings& operator=(Mappings&& mappings) noexcept;
		bool operator==(const Mappings&) = delete;
		bool operator!=(const Mappings&) = delete;

	public:
		void Clear() noexcept;

		void AddStructureMapping(std::uint32_t module, std::string name);
		void AddFunctionMapping(std::uint32_t module, std::string name);

		const StructureMapping& GetStructureMapping(std::uint32_t index) const noexcept;
		std::uint32_t GetStructureMappingCount() const noexcept;
		const FunctionMapping& GetFunctionMapping(std::uint32_t index) const noexcept;
		std::uint32_t GetFunctionMappingCount() const noexcept;
	};
}