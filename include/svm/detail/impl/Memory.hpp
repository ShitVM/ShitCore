#pragma once
#include <svm/Memory.hpp>

#include <algorithm>
#include <cstring>

namespace svm {
#ifdef SVM_LITTLE
	constexpr Endian GetEndian() noexcept {
		return Endian::Little;
	}
#endif

	template<typename T>
	T ReverseEndian(const T& value) noexcept {
		alignas(T) std::uint8_t buffer[sizeof(T)];

		std::memcpy(buffer, &value, sizeof(T));
		std::reverse(buffer, buffer + sizeof(T));

		return *reinterpret_cast<T*>(buffer);
	}
}

namespace svm {
	template<typename T>
	T CreateObjectFromBytes(const std::uint8_t* begin) noexcept {
		alignas(T) std::uint8_t buffer[sizeof(T)];

		std::memcpy(buffer, begin, sizeof(T));

		return *reinterpret_cast<T*>(buffer);
	}

	template<typename T>
	std::size_t Pade(std::size_t dataSize) noexcept {
		const std::size_t temp = dataSize / sizeof(T) * sizeof(T);
		if (dataSize == temp) return dataSize;
		else return temp + sizeof(T);
	}
}