#pragma once

#include <svm/Mapping.hpp>
#include <svm/Structure.hpp>

#include <string>
#include <string_view>
#include <vector>

namespace svm::core {
	template<typename F>
	class ModuleBase {
	private:
		std::string m_Path;
		std::vector<std::string> m_Dependencies;
		Structures m_Structures;
		F m_Functions;
		Mappings m_Mappings;

	public:
		ModuleBase() noexcept = default;
		explicit ModuleBase(std::string path) noexcept;
		ModuleBase(std::string path, std::vector<std::string> dependencies, Structures&& structures,
			F&& functions, Mappings&& mappings) noexcept;
		ModuleBase(ModuleBase&& module) noexcept;
		~ModuleBase() = default;

	public:
		ModuleBase& operator=(ModuleBase&& module) noexcept;
		bool operator==(const ModuleBase&) = delete;
		bool operator!=(const ModuleBase&) = delete;

	public:
		std::string_view GetPath() const noexcept;
		void SetPath(std::string newPath) noexcept;
		const std::vector<std::string>& GetDependencies() const noexcept;
		std::vector<std::string>& GetDependencies() noexcept;
		void SetDependencies(std::vector<std::string> newDependencies) noexcept;
		const Structures& GetStructures() const noexcept;
		Structures& GetStructures() noexcept;
		void SetStructures(Structures&& newStructures) noexcept;
		const F& GetFunctions() const noexcept;
		F& GetFunctions() noexcept;
		void SetFunctions(F&& newFunctions) noexcept;
		const Mappings& GetMappings() const noexcept;
		Mappings& GetMappings() noexcept;
		void SetMappings(Mappings&& newMappings) noexcept;

		void UpdateStructureInfos(std::uint32_t module) noexcept;
	};
}

#include "detail/impl/ModuleBase.hpp"