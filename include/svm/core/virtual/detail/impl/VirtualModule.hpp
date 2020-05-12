#pragma once
#include <svm/core/virtual/VirtualModule.hpp>

#include <utility>

namespace svm::core {
	template<typename FI>
	VirtualModule<FI>::VirtualModule(std::string path) noexcept
		: m_Path(std::move(path)) {}
	template<typename FI>
	VirtualModule<FI>::VirtualModule(std::string path, Structures&& structures, VirtualFunctions<FI>&& functions) noexcept
		: m_Path(std::move(path)), m_Structures(std::move(structures)), m_Functions(std::move(functions)) {}
	template<typename FI>
	VirtualModule<FI>::VirtualModule(std::string path, std::vector<std::string> dependencies, Structures&& structures, VirtualFunctions<FI>&& functions) noexcept
		: m_Path(std::move(path)), m_Dependencies(std::move(dependencies)), m_Structures(std::move(structures)),
		m_Functions(std::move(functions)) {}
	template<typename FI>
	VirtualModule<FI>::VirtualModule(VirtualModule&& module) noexcept
		: m_Path(std::move(module.m_Path)), m_Dependencies(std::move(module.m_Dependencies)),
		m_Structures(std::move(module.m_Structures)), m_Functions(std::move(module.m_Functions)) {}

	template<typename FI>
	VirtualModule<FI>& VirtualModule<FI>::operator=(VirtualModule&& module) noexcept {
		m_Path = std::move(module.m_Path);
		m_Dependencies = std::move(module.m_Dependencies);
		m_Structures = std::move(module.m_Structures);
		m_Functions = std::move(module.m_Functions);
		return *this;
	}

	template<typename FI>
	std::string_view VirtualModule<FI>::GetPath() const noexcept {
		return m_Path;
	}
	template<typename FI>
	const std::vector<std::string>& VirtualModule<FI>::GetDependencies() const noexcept {
		return m_Dependencies;
	}
	template<typename FI>
	std::vector<std::string>& VirtualModule<FI>::GetDependencies() noexcept {
		return m_Dependencies;
	}
	template<typename FI>
	const Structures& VirtualModule<FI>::GetStructures() const noexcept {
		return m_Structures;
	}
	template<typename FI>
	Structures& VirtualModule<FI>::GetStructures() noexcept {
		return m_Structures;
	}
	template<typename FI>
	const VirtualFunctions<FI>& VirtualModule<FI>::GetFunctions() const noexcept {
		return m_Functions;
	}
	template<typename FI>
	VirtualFunctions<FI>& VirtualModule<FI>::GetFunctions() noexcept {
		return m_Functions;
	}

	template<typename FI>
	void VirtualModule<FI>::UpdateStructureInfos(std::uint32_t module) noexcept {
		for (StructureInfo& structure : m_Structures) {
			structure.Type.Module = module;
		}
	}
}