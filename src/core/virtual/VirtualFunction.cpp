#include <svm/core/virtual/VirtualFunction.hpp>

namespace svm::core {
	VirtualFunctionInfo::VirtualFunctionInfo(std::uint16_t arity) noexcept
		: m_Arity(arity) {}
	VirtualFunctionInfo::VirtualFunctionInfo(bool hasResult) noexcept
		: m_HasResult(hasResult) {}
	VirtualFunctionInfo::VirtualFunctionInfo(std::uint16_t arity, bool hasResult) noexcept
		: m_Arity(arity), m_HasResult(hasResult) {}
	VirtualFunctionInfo::VirtualFunctionInfo(VirtualFunctionInfo&& functionInfo) noexcept
		: m_Arity(functionInfo.m_Arity), m_HasResult(functionInfo.m_HasResult) {}

	VirtualFunctionInfo& VirtualFunctionInfo::operator=(VirtualFunctionInfo&& functionInfo) noexcept {
		m_Arity = functionInfo.m_Arity;
		m_HasResult = functionInfo.m_HasResult;
		return *this;
	}

	std::uint16_t VirtualFunctionInfo::GetArity() const noexcept {
		return m_Arity;
	}
	bool VirtualFunctionInfo::HasResult() const noexcept {
		return m_HasResult;
	}
}