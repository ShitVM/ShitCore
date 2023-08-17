#pragma once
#include <svm/core/ModuleBase.hpp>

#include <utility>

namespace svm::core {
	template<typename F>
	ModuleBase<F>::ModuleBase(ModulePath path) noexcept
		: m_Path(std::move(path)) {}
	template<typename F>
	ModuleBase<F>::ModuleBase(ModulePath path, std::vector<Dependency> dependencies, Structures&& structures,
		F&& functions, Mappings&& mappings) noexcept
		: m_Path(std::move(path)), m_Dependencies(std::move(dependencies)), m_Structures(std::move(structures)),
		m_Functions(std::move(functions)), m_Mappings(std::move(mappings)) {}
	template<typename F>
	ModuleBase<F>::ModuleBase(ModuleBase&& module) noexcept
		: m_Path(std::move(module.m_Path)), m_Dependencies(std::move(module.m_Dependencies)), m_Structures(std::move(module.m_Structures)),
		m_Functions(std::move(module.m_Functions)), m_Mappings(std::move(module.m_Mappings)) {}

	template<typename F>
	ModuleBase<F>& ModuleBase<F>::operator=(ModuleBase&& module) noexcept {
		m_Path = std::move(module.m_Path);
		m_Dependencies = std::move(module.m_Dependencies);
		m_Structures = std::move(module.m_Structures);
		m_Functions = std::move(module.m_Functions);
		m_Mappings = std::move(module.m_Mappings);
		return *this;
	}

	template<typename F>
	const ModulePath& ModuleBase<F>::GetPath() const noexcept {
		return m_Path;
	}
	template<typename F>
	void ModuleBase<F>::SetPath(ModulePath newPath) noexcept {
		m_Path = std::move(newPath);
	}
	template<typename F>
	const std::vector<Dependency>& ModuleBase<F>::GetDependencies() const noexcept {
		return m_Dependencies;
	}
	template<typename F>
	std::vector<Dependency>& ModuleBase<F>::GetDependencies() noexcept {
		return m_Dependencies;
	}
	template<typename F>
	void ModuleBase<F>::SetDependencies(std::vector<Dependency> newDependencies) noexcept {
		m_Dependencies = std::move(newDependencies);
	}
	template<typename F>
	const Structures& ModuleBase<F>::GetStructures() const noexcept {
		return m_Structures;
	}
	template<typename F>
	Structures& ModuleBase<F>::GetStructures() noexcept {
		return m_Structures;
	}
	template<typename F>
	void ModuleBase<F>::SetStructures(Structures&& newStructures) noexcept {
		m_Structures = std::move(newStructures);
	}
	template<typename F>
	const F& ModuleBase<F>::GetFunctions() const noexcept {
		return m_Functions;
	}
	template<typename F>
	F& ModuleBase<F>::GetFunctions() noexcept {
		return m_Functions;
	}
	template<typename F>
	void ModuleBase<F>::SetFunctions(F&& newFunctions) noexcept {
		m_Functions = std::move(newFunctions);
	}
	template<typename F>
	const Mappings& ModuleBase<F>::GetMappings() const noexcept {
		return m_Mappings;
	}
	template<typename F>
	Mappings& ModuleBase<F>::GetMappings() noexcept {
		return m_Mappings;
	}
	template<typename F>
	void ModuleBase<F>::SetMappings(Mappings&& newMappings) noexcept {
		m_Mappings = std::move(newMappings);
	}

	template<typename F>
	void ModuleBase<F>::UpdateStructureInfos(std::uint32_t module) noexcept {
		for (StructureInfo& structure : m_Structures) {
			structure.Type.Module = module;
		}
	}
	template<typename F>
	void ModuleBase<F>::UpdateFunctionInfos(std::uint32_t module) noexcept {
		for (auto& function : m_Functions) {
			function.Module = module;
		}
	}
}