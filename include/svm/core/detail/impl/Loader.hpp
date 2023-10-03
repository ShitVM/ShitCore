#pragma once
#include "../../Loader.hpp"

#include <svm/Object.hpp>
#include <svm/Structure.hpp>
#include <svm/Type.hpp>
#include <svm/core/Parser.hpp>

#include <algorithm>
#include <cassert>
#include <memory>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <variant>

namespace svm::core {
	template<typename FI>
	void Loader<FI>::Clear() noexcept {
		m_Modules.clear();
	}

	template<typename FI>
	void Loader<FI>::AddLibraryDirectory(const std::filesystem::path& path) {
		m_LibraryDirectories.push_back(std::filesystem::canonical(path));
	}

	template<typename FI>
	Module<FI> Loader<FI>::Load(const std::filesystem::path& path) {
		Parser parser;

		parser.Open(path);
		parser.Parse();

		auto byteFile = parser.GetResult();
		const auto index = static_cast<std::uint32_t>(m_Modules.size());

		byteFile.UpdateStructureInfos(index);
		byteFile.UpdateFunctionInfos(index);

		const auto result = m_Modules.emplace_back(
			std::make_unique<ModuleInfo<FI>>(std::move(byteFile))).get();

		LoadDependencies(result);

		return *result;
	}
	template<typename FI>
	VirtualModule<FI>& Loader<FI>::Create(const std::filesystem::path& path) {
		return std::get<VirtualModule<FI>>(m_Modules.emplace_back(
			std::make_unique<ModuleInfo<FI>>(VirtualModule<FI>(
				std::filesystem::weakly_canonical(path))))->Module);
	}
	template<typename FI>
	VirtualModule<FI>& Loader<FI>::Create(const std::string& path) {
		assert(path.size() >= 2);
		assert(path[0] == '/');

		return std::get<VirtualModule<FI>>(m_Modules.emplace_back(
			std::make_unique<ModuleInfo<FI>>(VirtualModule<FI>(
				std::filesystem::weakly_canonical(std::filesystem::u8path(path)).generic_string())))->Module);
	}
	template<typename FI>
	void Loader<FI>::Build(VirtualModule<FI>& module) {
		const auto moduleIter = std::find_if(m_Modules.begin(), m_Modules.end(),
			[&module](const auto& module2) {
				return std::holds_alternative<VirtualModule<FI>>(module2->Module) &&
					&std::get<VirtualModule<FI>>(module2->Module) == &module;
			});
		const auto index = static_cast<std::uint32_t>(std::distance(m_Modules.begin(), moduleIter));

		module.UpdateStructureInfos(index);
		module.UpdateFunctionInfos(index);

		LoadDependencies(moduleIter->get());
	}

	template<typename FI>
	Module<FI> Loader<FI>::GetModule(std::uint32_t index) const noexcept {
		return *m_Modules[index];
	}
	template<typename FI>
	Module<FI> Loader<FI>::GetModule(const ModulePath& path) const noexcept {
		if (const auto module = GetModuleInternal(path); module)
			return *module;
		else
			return nullptr;
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
	ModulePath Loader<FI>::ResolveDependency(Module<FI> module, const std::string& dependency) const {
		if (dependency[0] == '/') {
			const auto dependencyPath = std::filesystem::u8path(dependency.substr(1));

			for (const auto& directory : m_LibraryDirectories) {
				const auto path = directory / dependencyPath;
				if (!std::filesystem::exists(path)) continue;

				return std::filesystem::weakly_canonical(path);
			}

			return std::filesystem::weakly_canonical(
				std::filesystem::u8path(dependency)).generic_string();
		} else {
			const auto& modulePath = module->GetPath();
			const auto dependencyPath = std::filesystem::u8path(dependency);

			if (std::holds_alternative<std::filesystem::path>(modulePath)) {
				return std::filesystem::weakly_canonical(
					std::get<std::filesystem::path>(modulePath).parent_path() /
					dependencyPath);
			} else {
				return std::filesystem::weakly_canonical(
					std::filesystem::u8path(std::get<std::string>(modulePath)).parent_path() /
					std::filesystem::u8path(dependency)).generic_u8string();
			}
		}
	}

	template<typename FI>
	void Loader<FI>::LoadDependencies(ModuleInfo<FI>* module) {
		const auto dependencyCount = module->GetDependencyCount();

		for (std::uint32_t i = 0; i < dependencyCount; ++i) {
			auto& dependency = module->GetDependency(i);
			const auto dependencyPath = ResolveDependency(*module, dependency.Path);

			if (const auto dependencyModule = GetModuleInternal(dependencyPath);
				dependencyModule) {

				dependency.Module = dependencyModule;
			} else if (std::holds_alternative<std::string>(dependencyPath)) {
				throw std::runtime_error("Failed to load the file. Unknown dependency.");
			} else {
				dependency.Module = Load(std::get<std::filesystem::path>(dependencyPath)).GetPointer();
			}
		}

		const auto structCount = module->GetStructureCount();

		for (std::uint32_t i = 0; i < structCount; ++i) {
			for (auto& field : module->GetStructure(i).Fields) {
				if (field.Type->Code != TypeCode::None)
					continue;

				const auto& dependency = module->GetDependency(field.Type->Module - 1);

				field.Type = static_cast<const ModuleInfo<FI>*>(dependency.Module)->
					GetStructure(field.Type->Name)->Type;
			}
		}

		FindCycle(module);
		UpdateTypeInfo(module);
	}

	template<typename FI>
	void Loader<FI>::FindCycle(ModuleInfo<FI>* module) const {
		const auto structCount = module->GetStructureCount();

		for (std::uint32_t i = 0; i < structCount; ++i) {
			std::unordered_map<
				void*,
				std::unordered_map<std::uint32_t, int>
			> visited; // visited[module][structure]

			if (FindCycle(visited, module, i))
				throw std::runtime_error("Failed to load the file. Detected circular reference.");
		}
	}
	template<typename FI>
	bool Loader<FI>::FindCycle(
		std::unordered_map<
		void*,
		std::unordered_map<std::uint32_t, int>
		>& visited, ModuleInfo<FI>* module, std::uint32_t node) const {

		auto& status = visited[module][node];

		if (status) {
			return status == 1;
		} else {
			status = 1; // In searching...
		}

		for (const auto& field : module->GetStructure(node).Fields) {
			const Type type = field.Type;
			if (!type.IsStructure()) continue;

			const bool hasCycle = FindCycle(
				visited,
				m_Modules[type->Module].get(),
				static_cast<std::uint32_t>(type->Code) - static_cast<std::uint32_t>(TypeCode::Structure)
			);

			if (hasCycle)
				return true;
		}

		status = 2; // No cycle
		return false;
	}
	template<typename FI>
	void Loader<FI>::UpdateTypeInfo(ModuleInfo<FI>* module) {
		const auto structCount = module->GetStructureCount();

		for (std::uint32_t i = 0; i < structCount; ++i) {
			UpdateTypeInfo(module, i);
		}
	}
	template<typename FI>
	void Loader<FI>::UpdateTypeInfo(ModuleInfo<FI>* module, std::uint32_t node) {
		auto& structure = module->GetStructure(node);
		if (structure.Type.RawSize) return;

		std::size_t offset = 0;

		for (auto& field : structure.Fields) {
			const Type type = field.Type;

			if (type.IsStructure()) {
				UpdateTypeInfo(
					m_Modules[type->Module].get(),
					static_cast<std::uint32_t>(type->Code) - static_cast<std::uint32_t>(TypeCode::Structure));
			}

			if (structure.Type.RawAlignment < type->RawAlignment) {
				structure.Type.RawAlignment = type->RawAlignment;
			}

			field.Offset = CalcNearestMultiplier(offset, type->RawAlignment);
			offset = field.Offset + static_cast<std::size_t>(
				type->RawSize * std::max(static_cast<std::uint64_t>(1), field.Count));
		}

		structure.Type.RawSize = offset;
		structure.Type.Size = CalcNearestMultiplier(
			structure.Type.RawSize + sizeof(StructureObject), ObjectAlignment);
	}

	template<typename FI>
	ModuleInfo<FI>* Loader<FI>::GetModuleInternal(const ModulePath& path) const noexcept {
		const auto iter = std::find_if(m_Modules.begin(), m_Modules.end(),
			[path](const auto& module) {
				return module->GetPath() == path;
			});
		if (iter == m_Modules.end()) return nullptr;
		else return iter->get();
	}
}