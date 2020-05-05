#include <svm/IO.hpp>

#include <iomanip>
#include <ios>

namespace svm {
	namespace detail {
		int DepthIndex() {
			static const int index = std::ios_base::xalloc();
			return index;
		}
		std::string MakeIndent(std::ostream& stream) {
			return std::string(stream.iword(DepthIndex()) * 4, ' ');
		}
	}
	std::ostream& Indent(std::ostream& stream) {
		++stream.iword(detail::DepthIndex());
		return stream;
	}
	std::ostream& UnIndent(std::ostream& stream) {
		--stream.iword(detail::DepthIndex());
		return stream;
	}

	detail::QWord QWord(std::uint64_t value) noexcept {
		return { value };
	}
	detail::Hex Hex(std::uint64_t value) noexcept {
		return { value };
	}
	std::ostream& operator<<(std::ostream& stream, detail::QWord qword) {
		return stream << std::hex << std::uppercase << std::setw(16) << std::setfill('0')
					  << qword.Value
					  << std::dec << std::nouppercase;
	}
	std::ostream& operator<<(std::ostream& stream, detail::Hex hex) {
		return stream << std::hex << std::uppercase << hex.Value << std::dec << std::nouppercase;
	}
}