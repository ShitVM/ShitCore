#pragma once

#include <svm/detail/ReferenceWrapper.hpp>

#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace svm::core {
	class VirtualFunctionInfo {
	public:
		std::uint32_t Module = 0;

	private:
		std::string m_Name;
		std::uint16_t m_Arity = 0;
		bool m_HasResult = false;

	public:
		VirtualFunctionInfo() noexcept = default;
		VirtualFunctionInfo(std::string name, std::uint16_t arity) noexcept;
		VirtualFunctionInfo(std::string name, bool hasResult) noexcept;
		VirtualFunctionInfo(std::string name, std::uint16_t arity, bool hasResult) noexcept;
		VirtualFunctionInfo(VirtualFunctionInfo&& functionInfo) noexcept;
		~VirtualFunctionInfo() = default;

	public:
		VirtualFunctionInfo& operator=(VirtualFunctionInfo&& functionInfo) noexcept;
		bool operator==(const VirtualFunctionInfo&) = delete;
		bool operator!=(const VirtualFunctionInfo&) = delete;

	public:
		std::string_view GetName() const noexcept;
		std::uint16_t GetArity() const noexcept;
		bool HasResult() const noexcept;
	};
}

namespace svm::core {
	template<typename T>
	class VirtualFunction final : public detail::ReferenceWrapper<T> {
		static_assert(std::is_base_of_v<VirtualFunctionInfo, T>);

	public:
		using detail::ReferenceWrapper<T>::ReferenceWrapper;
	};

	template<typename T>
	using VirtualFunctions = std::vector<T>;
}