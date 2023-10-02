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

	template<typename T>
	std::size_t Pade(std::size_t dataSize) noexcept;
}

#include "detail/impl/Memory.hpp"