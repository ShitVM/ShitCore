#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace svm {
	struct Mapping final {
		std::uint32_t Module;
		std::string Name;
	};

	class Mappings final {
	private:
		std::vector<Mapping> m_StructureMappings;
		std::vector<Mapping> m_FunctionMappings;

	public:
		Mappings() = default;
		Mappings(std::vector<Mapping> structures, std::vector<Mapping> functions) noexcept;
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

		const Mapping& GetStructureMapping(std::uint32_t index) const noexcept;
		std::uint32_t GetStructureMappingCount() const noexcept;
		const Mapping& GetFunctionMapping(std::uint32_t index) const noexcept;
		std::uint32_t GetFunctionMappingCount() const noexcept;
	};
}