#pragma once
#include <svm/Type.hpp>

namespace svm {
	template<typename T>
	Type GetType(const T& structures, TypeCode code) noexcept {
		switch (code) {
		case TypeCode::Int: return IntType;
		case TypeCode::Long: return LongType;
		case TypeCode::Double: return DoubleType;
		case TypeCode::Pointer: return PointerType;
		case TypeCode::GCPointer: return GCPointerType;
		default:
			if (TypeCode::Structure <= code) return structures[static_cast<std::uint32_t>(code) - static_cast<std::uint32_t>(TypeCode::Structure)].Type;
			else return NoneType;
		}
	}
}