#include <svm/core/virtual/VirtualFunction.hpp>

#include <utility>

namespace svm::core {
	VirtualFunctionInfo::VirtualFunctionInfo(std::string name, std::uint16_t arity) noexcept
		: m_Name(std::move(name)), m_Arity(arity) {}
	VirtualFunctionInfo::VirtualFunctionInfo(std::string name, bool hasResult) noexcept
		: m_Name(std::move(name)), m_HasResult(hasResult) {}
	VirtualFunctionInfo::VirtualFunctionInfo(std::string name, std::uint16_t arity, bool hasResult) noexcept
		: m_Name(std::move(name)), m_Arity(arity), m_HasResult(hasResult) {}
	VirtualFunctionInfo::VirtualFunctionInfo(VirtualFunctionInfo&& functionInfo) noexcept
		: m_Name(std::move(functionInfo.m_Name)), m_Arity(functionInfo.m_Arity), m_HasResult(functionInfo.m_HasResult) {}

	VirtualFunctionInfo& VirtualFunctionInfo::operator=(VirtualFunctionInfo&& functionInfo) noexcept {
		m_Name = std::move(functionInfo.m_Name);
		m_Arity = functionInfo.m_Arity;
		m_HasResult = functionInfo.m_HasResult;
		return *this;
	}

	std::string_view VirtualFunctionInfo::GetName() const noexcept {
		return m_Name;
	}
	std::uint16_t VirtualFunctionInfo::GetArity() const noexcept {
		return m_Arity;
	}
	bool VirtualFunctionInfo::HasResult() const noexcept {
		return m_HasResult;
	}
}