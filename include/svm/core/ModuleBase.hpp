#pragma once

#include <svm/Mapping.hpp>
#include <svm/Structure.hpp>

#include <filesystem>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace svm::core {
	using ModulePath = std::variant<std::filesystem::path, std::string>;

	std::ostream& operator<<(std::ostream& stream, const ModulePath& path);

	template<typename F>
	class ModuleBase {
	private:
		ModulePath m_Path;
		std::vector<std::string> m_Dependencies;
		Structures m_Structures;
		F m_Functions;
		Mappings m_Mappings;

	public:
		ModuleBase() noexcept = default;
		explicit ModuleBase(ModulePath path) noexcept;
		ModuleBase(ModulePath path, std::vector<std::string> dependencies, Structures&& structures,
			F&& functions, Mappings&& mappings) noexcept;
		ModuleBase(ModuleBase&& module) noexcept;
		~ModuleBase() = default;

	public:
		ModuleBase& operator=(ModuleBase&& module) noexcept;
		bool operator==(const ModuleBase&) = delete;
		bool operator!=(const ModuleBase&) = delete;

	public:
		const ModulePath& GetPath() const noexcept;
		void SetPath(ModulePath newPath) noexcept;
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
		void UpdateFunctionInfos(std::uint32_t module) noexcept;
	};
}

#include "detail/impl/ModuleBase.hpp"