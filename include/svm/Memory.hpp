#pragma once

#include <svm/Predefined.hpp>

#include <cstddef>

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
	std::size_t Pade(std::size_t dataSize) noexcept;
}

#include "detail/impl/Memory.hpp"