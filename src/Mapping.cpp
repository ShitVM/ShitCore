#include <svm/Mapping.hpp>

#include <utility>

namespace svm {
	Mappings::Mappings(std::vector<Mapping> structures, std::vector<Mapping> functions) noexcept
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

	void Mappings::AddStructureMapping(std::uint32_t module, std::string name) {
		m_StructureMappings.push_back({ module, std::move(name) });
	}
	void Mappings::AddFunctionMapping(std::uint32_t module, std::string name) {
		m_FunctionMappings.push_back({ module, std::move(name) });
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