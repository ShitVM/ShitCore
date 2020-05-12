#pragma once

#include <svm/Structure.hpp>
#include <svm/core/virtual/VirtualFunction.hpp>

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace svm::core {
	template<typename FI>
	class VirtualModule {
	private:
		std::string m_Path;
		std::vector<std::string> m_Dependencies;
		Structures m_Structures;
		VirtualFunctions<FI> m_Functions;

	public:
		VirtualModule() noexcept = default;
		VirtualModule(std::string path) noexcept;
		VirtualModule(std::string path, Structures&& structures, VirtualFunctions<FI>&& functions) noexcept;
		VirtualModule(std::string path, std::vector<std::string> dependencies, Structures&& structures, VirtualFunctions<FI>&& functions) noexcept;
		VirtualModule(VirtualModule&& module) noexcept;
		~VirtualModule() = default;

	public:
		VirtualModule& operator=(VirtualModule&& module) noexcept;
		bool operator==(const VirtualModule&) = delete;
		bool operator!=(const VirtualModule&) = delete;

	public:
		std::string_view GetPath() const noexcept;
		const std::vector<std::string>& GetDependencies() const noexcept;
		std::vector<std::string>& GetDependencies() noexcept;
		const Structures& GetStructures() const noexcept;
		Structures& GetStructures() noexcept;
		const VirtualFunctions<FI>& GetFunctions() const noexcept;
		VirtualFunctions<FI>& GetFunctions() noexcept;

		void UpdateStructureInfos(std::uint32_t module) noexcept;
	};
}

#include "detail/impl/VirtualModule.hpp"