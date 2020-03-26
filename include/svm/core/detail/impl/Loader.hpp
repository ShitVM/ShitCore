#pragma once
#include <svm/core/Loader.hpp>

#include <svm/core/Parser.hpp>
#include <svm/detail/FileSystem.hpp>

#include <algorithm>
#include <variant>

namespace svm::core {
	template<typename FI>
	Loader<FI>::Loader(Loader&& loader) noexcept
		: m_Modules(std::move(loader.m_Modules)) {}

	template<typename FI>
	Loader<FI>& Loader<FI>::operator=(Loader&& loader) noexcept {
		m_Modules = std::move(loader.m_Modules);
		return *this;
	}

	template<typename FI>
	void Loader<FI>::Clear() noexcept {
		m_Modules.clear();
	}

	template<typename FI>
	Module<FI> Loader<FI>::Load(const std::string& path) {
		Parser parser;
		parser.Open(path);
		parser.Parse();

		return m_Modules.emplace_back(parser.GetResult());
	}
	template<typename FI>
	VirtualModule<FI>& Loader<FI>::Create(const std::string& virtualPath) {
		ModuleInfo<FI>& module = m_Modules.emplace_back(VirtualModule<FI>(detail::GetAbsolutePath(virtualPath)));
		return std::get<VirtualModule<FI>>(module.Module);
	}
	template<typename FI>
	void Loader<FI>::LoadDependencies(Module<FI> module) {
		if (!std::holds_alternative<ByteFile>(module->Module)) return;

		const ByteFile& byteFile = std::get<ByteFile>(module->Module);
		const std::vector<std::string>& dependencies = byteFile.GetDependencies();

		for (const std::string& dependency : dependencies) {
			if (GetModule(dependency) != nullptr) continue;

			LoadDependencies(Load(dependency));
		}
	}

	template<typename FI>
	Module<FI> Loader<FI>::GetModule(std::uint32_t index) const noexcept {
		return m_Modules[index];
	}
	template<typename FI>
	Module<FI> Loader<FI>::GetModule(const std::string& path) const noexcept {
		const auto iter = std::find_if(m_Modules.begin(), m_Modules.end(), [path](const auto& module) {
			return module.GetPath() == path;
		});
		if (iter == m_Modules.end()) return nullptr;
		else return *iter;
	}
	template<typename FI>
	std::uint32_t Loader<FI>::GetModuleCount() const noexcept {
		return static_cast<std::uint32_t>(m_Modules.size());
	}

	template<typename FI>
	void Loader<FI>::UpdateStructureCodes() {
		std::uint32_t offset = 0;
		for (std::uint32_t i = 0; i < m_Modules.size(); ++i) {
			ModuleInfo<FI>& module = m_Modules[i];
			module.UpdateStructureCodes(offset);
			offset += module.GetStructureCount();
		}
	}
}