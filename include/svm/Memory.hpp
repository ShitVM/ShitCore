#pragma once

#include <svm/Predefined.hpp>

#include <cstddef>
#include <cstdint>

namespace svm {
	enum class Endian {
		Little,
		Big,
	};

#ifdef SVM_LITTLE
	constexpr Endian GetEndian() noexcept;
#else
	Endian GetEndian() noexcept;
#endif

	template<typename T>
	T ReverseEndian(const T& value) noexcept;
}

namespace svm {
	template<typename T>
	T CreateObjectFromBytes(const std::uint8_t* begin) noexcept;

	std::size_t CalcNearestMultiplier(std::size_t value, std::size_t divider) noexcept;
}

#include "detail/impl/Memory.hpp"