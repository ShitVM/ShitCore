#pragma once

#include <cstdint>

namespace svm {
	enum class ShitBFVersion : std::uint16_t {
		v0_1_0,
		v0_2_0,
		v0_3_0,
		v0_4_0,
		v0_5_0,

		Least = v0_4_0,
		Latest = v0_5_0,
	};

	enum class ShitBCVersion : std::uint16_t {
		v0_1_0,
		v0_2_0,
		v0_3_0,
		v0_4_0,
		v0_5_0,

		Least = v0_4_0,
		Latest = v0_5_0,
	};
}