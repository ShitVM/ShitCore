#pragma once
#include <svm/core/Loader.hpp>

#include <svm/core/Parser.hpp>
#include <svm/detail/FileSystem.hpp>

#include <algorithm>
#include <memory>
#include <utility>
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

		return *m_Modules.emplace_back(std::make_unique<ModuleInfo<FI>>(parser.GetResult()));
	}
	template<typename FI>
	VirtualModule<FI>& Loader<FI>::Create(const std::string& virtualPath) {
		ModuleInfo<FI>& module = *m_Modules.emplace_back(
			std::make_unique<ModuleInfo<FI>>(VirtualModule<FI>(svm::detail::GetAbsolutePath(virtualPath))));
		return std::get<VirtualModule<FI>>(module.Module);
	}
	template<typename FI>
	void Loader<FI>::LoadDependencies(Module<FI> module) {
		if (!std::holds_alternative<ByteFile>(module->Module)) return;

		const ByteFile& byteFile = std::get<ByteFile>(module->Module);
		const std::vector<std::string>& dependencies = byteFile.GetDependencies();

		for (const std::string& dependency : dependencies) {
			if (GetModule(dependency) != nullptr) continue;

			const svm::detail::fs::path& path = svm::detail::fs::u8path(byteFile.GetPath()).parent_path() / dependency;

			LoadDependencies(Load(path.generic_string()));
		}
	}

	template<typename FI>
	Module<FI> Loader<FI>::GetModule(std::uint32_t index) const noexcept {
		return *m_Modules[index];
	}
	template<typename FI>
	Module<FI> Loader<FI>::GetModule(const std::string& path) const noexcept {
		const std::string absPath = svm::detail::GetAbsolutePath(path);
		const auto iter = std::find_if(m_Modules.begin(), m_Modules.end(), [absPath](const auto& module) {
			return module->GetPath() == absPath;
		});
		if (iter == m_Modules.end()) return nullptr;
		else return **iter;
	}
	template<typename FI>
	std::uint32_t Loader<FI>::GetModuleCount() const noexcept {
		return static_cast<std::uint32_t>(m_Modules.size());
	}
	template<typename FI>
	const Modules<FI>& Loader<FI>::GetModules() const noexcept {
		return m_Modules;
	}
	template<typename FI>
	Modules<FI>& Loader<FI>::GetModules() noexcept {
		return m_Modules;
	}
	template<typename FI>
	void Loader<FI>::SetModules(Modules<FI>&& newModules) noexcept {
		m_Modules = std::move(newModules);
	}

	template<typename FI>
	void Loader<FI>::UpdateStructureInfos() {
		for (std::size_t i = 0; i < m_Modules.size(); ++i) {
			m_Modules[i]->UpdateStructureInfos(static_cast<std::uint32_t>(i));
		}
	}
}