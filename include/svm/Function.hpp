#pragma once

#include <svm/Instruction.hpp>
#include <svm/detail/ReferenceWrapper.hpp>

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

namespace svm {
	class FunctionInfo final {
	public:
		std::string Name;
		std::uint32_t Module = 0;
		std::uint16_t Arity = 0;
		bool HasResult = false;
		svm::Instructions Instructions;

	public:
		FunctionInfo() noexcept = default;
		FunctionInfo(std::string name, std::uint16_t arity, bool hasResult) noexcept;
		FunctionInfo(std::string name, std::uint16_t arity, svm::Instructions&& instructions) noexcept;
		FunctionInfo(std::string name, std::uint16_t arity, bool hasResult, svm::Instructions&& instructions) noexcept;
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