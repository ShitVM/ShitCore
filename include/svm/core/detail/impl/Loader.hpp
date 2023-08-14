#pragma once
#include <svm/core/Loader.hpp>

#include <svm/Memory.hpp>
#include <svm/Structure.hpp>
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
	Module<FI> Loader<FI>::Load(const std::filesystem::path& path) {
		Parser parser;
		parser.Open(path);
		parser.Parse();

		const auto index = static_cast<std::uint32_t>(m_Modules.size());
		auto byteFile = parser.GetResult();
		byteFile.UpdateStructureInfos(index);
		byteFile.UpdateFunctionInfos(index);

		auto result = m_Modules.emplace_back(std::make_unique<ModuleInfo<FI>>(std::move(byteFile))).get();
		LoadDependencies(result);
		return *result;
	}
	template<typename FI>
	VirtualModule<FI>& Loader<FI>::Create(std::filesystem::path path) {
		 return std::get<VirtualModule<FI>>(
			 m_Modules.emplace_back(std::make_unique<ModuleInfo<FI>>(VirtualModule<FI>(std::move(path))))->Module);
	}
	template<typename FI>
	VirtualModule<FI>& Loader<FI>::Create(std::string path) {
		assert(path.size() >= 2);
		assert(path[0] == '/');

		return std::get<VirtualModule<FI>>(
			m_Modules.emplace_back(std::make_unique<ModuleInfo<FI>>(VirtualModule<FI>(std::move(path))))->Module);
	}
	template<typename FI>
	void Loader<FI>::Build(VirtualModule<FI>& module) {
		const auto moduleIter = std::find_if(m_Modules.begin(), m_Modules.end(), [&module](const auto& module2) {
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
		const auto iter = std::find_if(m_Modules.begin(), m_Modules.end(), [path](const auto& module) {
			return module->GetPath() == path;
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
	ModulePath Loader<FI>::ResolveDependency(Module<FI> module, const std::string& dependency) const {
		if (dependency[0] == '/') return dependency;

		if (std::holds_alternative<std::filesystem::path>(module->GetPath())) {
			return std::filesystem::canonical(
				std::get<std::filesystem::path>(module->GetPath()).parent_path()
				/ std::filesystem::u8path(dependency));
		} else if (std::holds_alternative<std::string>(module->GetPath())) {
			return std::filesystem::weakly_canonical(
				std::filesystem::u8path(std::get<std::string>(module->GetPath())).parent_path()
				/ std::filesystem::u8path(dependency)).generic_u8string();
		}
	}

	template<typename FI>
	void Loader<FI>::LoadDependencies(ModuleInfo<FI>* module) {
		for (const auto& dependency : module->GetDependencies()) {
			const auto path = ResolveDependency(*module, dependency);
			if (GetModuleInternal(path) != nullptr) continue;

			Load(std::get<std::filesystem::path>(path));
		}

		const auto structCount = module->GetStructureCount();
		for (std::uint32_t i = 0; i < structCount; ++i) {
			for (auto& field : module->GetStructure(i).Fields) {
				if (field.Type->Code != TypeCode::None) continue;

				const auto& dependency = module->GetDependencies()[field.Type->Module - 1];
				const auto target = GetModuleInternal(ResolveDependency(*module, dependency));
				field.Type = target->GetStructure(field.Type->Name)->Type;
			}
		}

		FindCycle(module);
		CalcSize(module);
		CalcOffset(module);
	}

	template<typename FI>
	void Loader<FI>::FindCycle(ModuleInfo<FI>* module) const {
		const auto structCount = module->GetStructureCount();
		for (std::uint32_t i = 0; i < structCount; ++i) {
			std::unordered_map<void*, std::unordered_map<std::uint32_t, int>> visited; // visited[module][structure]

			if (FindCycle(visited, module, i)) throw std::runtime_error("Failed to load the file. Detected circular reference.");
		}
	}
	template<typename FI>
	bool Loader<FI>::FindCycle(std::unordered_map<void*, std::unordered_map<std::uint32_t, int>>& visited,
		ModuleInfo<FI>* module, std::uint32_t node) const {
		int& status = visited[module][node];
		if (status) return status == 1;

		const StructureInfo& structure = module->GetStructure(node);
		const auto fieldCount = static_cast<std::uint32_t>(structure.Fields.size());

		status = 1; // In searching
		for (std::uint32_t i = 0; i < fieldCount; ++i) {
			const Type type = structure.Fields[i].Type;
			if (!type.IsStructure()) continue;

			const bool hasCycle = FindCycle(visited, m_Modules[type->Module].get(),
				static_cast<std::uint32_t>(type->Code) - static_cast<std::uint32_t>(TypeCode::Structure));
			if (hasCycle) return true;
		}

		status = 2; // No cycle
		return false;
	}
	template<typename FI>
	void Loader<FI>::CalcSize(ModuleInfo<FI>* module) {
		const auto structCount = module->GetStructureCount();
		for (std::uint32_t i = 0; i < structCount; ++i) {
			module->GetStructure(i).Type.Size = CalcSize(module, i);
		}
	}
	template<typename FI>
	std::size_t Loader<FI>::CalcSize(ModuleInfo<FI>* module, std::uint32_t node) {
		StructureInfo& structure = module->GetStructure(node);
		std::size_t& s = structure.Type.Size;
		if (s) return s;

		const auto fieldCount = static_cast<std::uint32_t>(structure.Fields.size());
		for (std::size_t i = 0; i < fieldCount; ++i) {
			const Field& field = structure.Fields[i];
			const Type type = field.Type;
			std::size_t size = type->Size;
			if (type.IsStructure()) {
				size = CalcSize(m_Modules[type->Module].get(),
					static_cast<std::uint32_t>(type->Code) - static_cast<std::uint32_t>(TypeCode::Structure));
			}

			if (field.IsArray()) {
				s += static_cast<std::size_t>(size * field.Count + sizeof(std::uint64_t));
			} else {
				s += size;
			}
		}

		s += sizeof(Type);
		return s = Pade<void*>(s);
	}
	template<typename FI>
	void Loader<FI>::CalcOffset(ModuleInfo<FI>* module) {
		const auto structCount = module->GetStructureCount();
		for (std::uint32_t i = 0; i < structCount; ++i) {
			StructureInfo& structure = module->GetStructure(i);
			const auto fieldCount = static_cast<std::uint32_t>(structure.Fields.size());

			std::size_t offset = sizeof(Type);
			for (std::uint32_t j = 0; j < fieldCount; ++j) {
				Field& field = structure.Fields[j];

				field.Offset = offset;
				if (field.IsArray()) {
					offset += static_cast<std::size_t>(field.Type->Size * field.Count + sizeof(ArrayObject));
				} else {
					offset += field.Type->Size;
				}
			}
		}
	}

	template<typename FI>
	ModuleInfo<FI>* Loader<FI>::GetModuleInternal(const ModulePath& path) noexcept {
		const auto iter = std::find_if(m_Modules.begin(), m_Modules.end(), [path](const auto& module) {
			return module->GetPath() == path;
		});
		if (iter == m_Modules.end()) return nullptr;
		else return iter->get();
	}
}