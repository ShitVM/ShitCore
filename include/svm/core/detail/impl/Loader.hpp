#pragma once
#include <svm/core/Loader.hpp>

#include <svm/Structure.hpp>
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

		const auto index = static_cast<std::uint32_t>(m_Modules.size());
		auto byteFile = parser.GetResult();
		byteFile.UpdateStructureInfos(index);
		byteFile.UpdateFunctionInfos(index);

		return *m_Modules.emplace_back(std::make_unique<ModuleInfo<FI>>(std::move(byteFile)));
	}
	template<typename FI>
	VirtualModule<FI>& Loader<FI>::Create(const std::string& virtualPath) {
		const auto index = static_cast<std::uint32_t>(m_Modules.size());
		auto module = VirtualModule<FI>(svm::detail::GetAbsolutePath(virtualPath));
		module.UpdateStructureInfos(index);

		return std::get<VirtualModule<FI>>(
			m_Modules.emplace_back(std::make_unique<ModuleInfo<FI>>(std::move(module)))->Module);
	}
	template<typename FI>
	void Loader<FI>::LoadDependencies(Module<FI> module) {
		for (const auto& dependency : module->GetDependencies()) {
			if (GetModule(dependency) != nullptr) continue; // TODO: 무조건 의존성을 불러오도록 변경

			const auto path = svm::detail::fs::u8path(module->GetPath()).parent_path() / dependency;

			LoadDependencies(Load(path.generic_string()));
		}

		const auto structCount = module->GetStructureCount();
		for (std::uint32_t i = 0; i < structCount; ++i) {
			for (auto& field : module->GetStructure(i).Fields) {
				const auto target = GetModule(module->GetDependencies()[field.Type->Module - 1]);
				field.Type = target->GetStructure(field.Type->Name)->Type;
			}
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
}