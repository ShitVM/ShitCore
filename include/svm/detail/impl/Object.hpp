#pragma once
#include "../../Object.hpp"

namespace svm {
	template<>
	inline LongObject IntObject::Cast<LongObject>() const noexcept {
		return RawLongObject{ RawObject.Value };
	}
	template<>
	inline SingleObject IntObject::Cast<SingleObject>() const noexcept {
		return RawSingleObject{ static_cast<float>(RawObject.Value) };
	}
	template<>
	inline DoubleObject IntObject::Cast<DoubleObject>() const noexcept {
		return RawDoubleObject{ static_cast<double>(RawObject.Value) };
	}
}

namespace svm {
	template<>
	inline IntObject LongObject::Cast<IntObject>() const noexcept {
		return RawIntObject{ static_cast<std::uint32_t>(RawObject.Value) };
	}
	template<>
	inline SingleObject LongObject::Cast<SingleObject>() const noexcept {
		return RawSingleObject{ static_cast<float>(RawObject.Value) };
	}
	template<>
	inline DoubleObject LongObject::Cast<DoubleObject>() const noexcept {
		return RawDoubleObject{ static_cast<double>(RawObject.Value) };
	}
}

namespace svm {
	template<>
	inline IntObject SingleObject::Cast<IntObject>() const noexcept {
		return RawIntObject{ static_cast<std::uint32_t>(RawObject.Value) };

	}
	template<>
	inline LongObject SingleObject::Cast<LongObject>() const noexcept {
		return RawLongObject{ static_cast<std::uint64_t>(RawObject.Value) };
	}
	template<>
	inline DoubleObject SingleObject::Cast<DoubleObject>() const noexcept {
		return RawDoubleObject{ RawObject.Value };
	}
}

namespace svm {
	template<>
	inline IntObject DoubleObject::Cast<IntObject>() const noexcept {
		return RawIntObject{ static_cast<std::uint32_t>(RawObject.Value) };
	}
	template<>
	inline LongObject DoubleObject::Cast<LongObject>() const noexcept {
		return RawLongObject{ static_cast<std::uint64_t>(RawObject.Value) };
	}
	template<>
	inline SingleObject DoubleObject::Cast<SingleObject>() const noexcept {
		return RawSingleObject{ static_cast<float>(RawObject.Value) };
	}
}

namespace svm {
	template<>
	inline IntObject PointerObject::Cast<IntObject>() const noexcept {
		return RawIntObject{ static_cast<std::uint32_t>(
			reinterpret_cast<std::uintptr_t>(RawObject.Address)) };
	}
	template<>
	inline LongObject PointerObject::Cast<LongObject>() const noexcept {
		return RawLongObject{ static_cast<std::uint64_t>(
			reinterpret_cast<std::uintptr_t>(RawObject.Address)) };
	}
	template<>
	inline SingleObject PointerObject::Cast<SingleObject>() const noexcept {
		return RawSingleObject{ static_cast<float>(
			reinterpret_cast<std::uintptr_t>(RawObject.Address)) };
	}
	template<>
	inline DoubleObject PointerObject::Cast<DoubleObject>() const noexcept {
		return RawDoubleObject{ static_cast<double>(
			reinterpret_cast<std::uintptr_t>(RawObject.Address)) };
	}
}