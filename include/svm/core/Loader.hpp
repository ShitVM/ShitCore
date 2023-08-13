#pragma once

#include <svm/core/Module.hpp>
#include <svm/core/virtual/VirtualModule.hpp>

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace svm::core {
	template<typename FI>
	class Loader {
	private:
		Modules<FI> m_Modules;

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

		Module<FI> Load(const std::string& path);
		VirtualModule<FI>& Create(const std::string& virtualPath);

		Module<FI> GetModule(std::uint32_t index) const noexcept;
		Module<FI> GetModule(const std::string& path) const noexcept;
		std::uint32_t GetModuleCount() const noexcept;
		const Modules<FI>& GetModules() const noexcept;
		Modules<FI>& GetModules() noexcept;
		void SetModules(Modules<FI>&& newModules) noexcept;

	private:
		void LoadDependencies(ModuleInfo<FI>* module);

		void FindCycle(ModuleInfo<FI>* module) const;
		bool FindCycle(std::unordered_map<void*, std::unordered_map<std::uint32_t, int>>& visited,
			ModuleInfo<FI>* module, std::uint32_t node) const;
		void CalcSize(ModuleInfo<FI>* module);
		std::size_t CalcSize(ModuleInfo<FI>* module, std::uint32_t node);
		void CalcOffset(ModuleInfo<FI>* module);

		ModuleInfo<FI>* GetModuleInternal(ModuleInfo<FI>* module, const std::string& path) noexcept;
	};
}

#include "detail/impl/Loader.hpp"