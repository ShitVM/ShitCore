#include <svm/Function.hpp>

#include <utility>

namespace svm {
	FunctionInfo::FunctionInfo(std::uint16_t arity) noexcept
		: Arity(arity) {}
	FunctionInfo::FunctionInfo(std::uint16_t arity, bool hasResult) noexcept
		: Arity(arity), HasResult(hasResult) {}
	FunctionInfo::FunctionInfo(std::uint16_t arity, svm::Instructions&& instructions) noexcept
		: Arity(arity), Instructions(std::move(instructions)) {}
	FunctionInfo::FunctionInfo(bool hasResult) noexcept
		: HasResult(hasResult) {}
	FunctionInfo::FunctionInfo(bool hasResult, svm::Instructions&& instructions) noexcept
		: HasResult(hasResult), Instructions(std::move(instructions)) {}
	FunctionInfo::FunctionInfo(std::uint16_t arity, bool hasResult, svm::Instructions&& instructions) noexcept
		: Arity(arity), HasResult(hasResult), Instructions(std::move(instructions)) {}
	FunctionInfo::FunctionInfo(FunctionInfo&& functionInfo) noexcept
		: Arity(functionInfo.Arity), HasResult(functionInfo.HasResult), Instructions(std::move(functionInfo.Instructions)) {}

	FunctionInfo& FunctionInfo::operator=(FunctionInfo&& functionInfo) noexcept {
		Arity = functionInfo.Arity;
		HasResult = functionInfo.HasResult;
		Instructions = std::move(functionInfo.Instructions);
		return *this;
	}
}