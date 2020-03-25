#pragma once

#include <svm/Instruction.hpp>
#include <svm/detail/ReferenceWrapper.hpp>

#include <cstdint>
#include <ostream>
#include <vector>

namespace svm {
	class FunctionInfo final {
	public:
		std::uint16_t Arity = 0;
		bool HasResult = false;
		svm::Instructions Instructions;

	public:
		FunctionInfo() noexcept = default;
		FunctionInfo(std::uint16_t arity) noexcept;
		FunctionInfo(std::uint16_t arity, bool hasResult) noexcept;
		FunctionInfo(std::uint16_t arity, svm::Instructions&& instructions) noexcept;
		FunctionInfo(bool hasResult) noexcept;
		FunctionInfo(bool hasResult, svm::Instructions&& instructions) noexcept;
		FunctionInfo(std::uint16_t arity, bool hasResult, svm::Instructions&& instructions) noexcept;
		FunctionInfo(FunctionInfo&& functionInfo) noexcept;
		~FunctionInfo() = default;

	public:
		FunctionInfo& operator=(FunctionInfo&& functionInfo) noexcept;
		bool operator==(const FunctionInfo&) = delete;
		bool operator!=(const FunctionInfo&) = delete;
	};

	std::ostream& operator<<(std::ostream& stream, const FunctionInfo& function);
}

namespace svm {
	class Function final : public detail::ReferenceWrapper<FunctionInfo> {
	public:
		using detail::ReferenceWrapper<FunctionInfo>::ReferenceWrapper;
	};

	using Functions = std::vector<FunctionInfo>;

	std::ostream& operator<<(std::ostream& stream, const Function& function);
	std::ostream& operator<<(std::ostream& stream, const Functions& functions);
}