#pragma once

#include <svm/core/Module.hpp>
#include <svm/core/virtual/VirtualModule.hpp>

#include <cstdint>
#include <string>

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

		ModuleInfo<FI>* GetModuleInternal(const std::string& path) noexcept;
	};
}

#include "detail/impl/Loader.hpp"