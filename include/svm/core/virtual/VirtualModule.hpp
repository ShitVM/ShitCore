#pragma once

#include <svm/Structure.hpp>
#include <svm/core/virtual/VirtualFunction.hpp>

#include <string>
#include <string_view>

namespace svm::core {
	template<typename FI>
	class VirtualModule {
	private:
		std::string m_Path;
		Structures m_Structures;
		VirtualFunctions<FI> m_Functions;

	public:
		VirtualModule() noexcept = default;
		VirtualModule(std::string path) noexcept;
		VirtualModule(std::string path, Structures&& structures, VirtualFunctions<FI>&& functions) noexcept;
		VirtualModule(VirtualModule&& module) noexcept;
		~VirtualModule() = default;

	public:
		VirtualModule& operator=(VirtualModule&& module) noexcept;
		bool operator==(const VirtualModule&) = delete;
		bool operator!=(const VirtualModule&) = delete;

	public:
		std::string_view GetPath() const noexcept;
		const Structures& GetStructures() const noexcept;
		Structures& GetStructures() noexcept;
		const VirtualFunctions<FI>& GetFunctions() const noexcept;
		VirtualFunctions<FI>& GetFunctions() noexcept;

		void UpdateStructureCodes(std::uint32_t offset) noexcept;
	};
}

#include "detail/impl/VirtualModule.hpp"