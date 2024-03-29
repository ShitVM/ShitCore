#pragma once

#include <svm/core/Module.hpp>
#include <svm/core/virtual/VirtualModule.hpp>

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace svm::core {
	template<typename FI>
	class Loader {
	private:
		Modules<FI> m_Modules;
		std::vector<std::filesystem::path> m_LibraryDirectories;

	public:
		Loader() noexcept = default;
		Loader(Loader&& loader) noexcept;
		~Loader() = default;

	public:
		Loader& operator=(Loader&& loader) noexcept;
		bool operator==(const Loader&) = delete;
		bool operator!=(const Loader&) = delete;

	public:
		void Clear() noexcept;

		void AddLibraryDirectory(const std::filesystem::path& path);

		Module<FI> Load(const std::filesystem::path& path);
		VirtualModule<FI>& Create(const std::filesystem::path& path);
		VirtualModule<FI>& Create(const std::string& path);
		void Build(VirtualModule<FI>& module);

		Module<FI> GetModule(std::uint32_t index) const noexcept;
		Module<FI> GetModule(const ModulePath& path) const noexcept;
		std::uint32_t GetModuleCount() const noexcept;
		const Modules<FI>& GetModules() const noexcept;
		Modules<FI>& GetModules() noexcept;
		void SetModules(Modules<FI>&& newModules) noexcept;

		ModulePath ResolveDependency(Module<FI> module, const std::string& dependency) const;

	private:
		void LoadDependencies(ModuleInfo<FI>* module);

		void FindCycle(ModuleInfo<FI>* module) const;
		bool FindCycle(std::unordered_map<void*, std::unordered_map<std::uint32_t, int>>& visited,
			ModuleInfo<FI>* module, std::uint32_t node) const;
		void CalcSize(ModuleInfo<FI>* module);
		std::size_t CalcSize(ModuleInfo<FI>* module, std::uint32_t node);
		void CalcOffset(ModuleInfo<FI>* module);

		ModuleInfo<FI>* GetModuleInternal(const ModulePath& path) const noexcept;
	};
}

#include "detail/impl/Loader.hpp"