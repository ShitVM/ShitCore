#include <svm/Function.hpp>

#include <svm/IO.hpp>

#include <ios>
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

	std::ostream& operator<<(std::ostream& stream, const FunctionInfo& function) {
		const std::string defIndent = detail::MakeIndent(stream);
		const std::string indentOnce(4, ' ');

		stream << defIndent << "Function:\n"
			   << defIndent << indentOnce << "Arity: " << function.Arity << '\n'
			   << defIndent << indentOnce << "HasResult: " << std::boolalpha << function.HasResult << std::noboolalpha << '\n'
			   << Indent << Indent << function.Instructions << UnIndent << UnIndent;
		return stream;
	}
}

namespace svm {
	std::ostream& operator<<(std::ostream& stream, const Function& function) {
		return stream << *function;
	}
	std::ostream& operator<<(std::ostream& stream, const Functions& functions) {
		const std::string defIndent = detail::MakeIndent(stream);
		const std::string indentOnce(4, ' ');

		stream << defIndent << "Functions: " << functions.size()
			   << Indent << Indent;
		for (std::uint32_t i = 0; i < functions.size(); ++i) {
			stream << '\n' << defIndent << indentOnce << '[' << i << "]:\n" << functions[i];
		}
		return stream << UnIndent << UnIndent;
	}
}