#include <svm/Memory.hpp>

namespace svm {
#ifndef SVM_LITTLE
	Endian GetEndian() noexcept {
		static const std::uint32_t tester = 1;
		static const Endian endian =
			*reinterpret_cast<const std::uint8_t*>(&tester) == 1 ?
			Endian::Little : Endian::Big;

		return endian;
	}
#endif
}

namespace svm {
	std::size_t CalcNearestMultiplier(std::size_t value, std::size_t divider) noexcept {
		const auto q = value / divider;
		const auto r = value % divider;

		if (r == 0)
			return value;
		else
			return (q + 1) * divider;
	}
}