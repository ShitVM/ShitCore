#include <svm/Type.hpp>

#include <svm/Object.hpp>

#include <utility>

namespace svm {
	TypeInfo::TypeInfo(std::string name, TypeCode code) noexcept
		: Name(std::move(name)), Code(code) {}
	TypeInfo::TypeInfo(
		std::string name, TypeCode code,
		std::size_t size, std::size_t rawSize, std::size_t rawAlignment) noexcept

		: Name(std::move(name)), Code(code),
		Size(size), RawSize(rawSize), RawAlignment(rawAlignment) {}
}

namespace svm {
	bool Type::IsFundamentalType() const noexcept {
		const TypeCode code = GetReference().Code;
		return TypeCode::None != code && code < TypeCode::Array;
	}
	bool Type::IsPointer() const noexcept {
		const TypeCode code = GetReference().Code;
		return code == TypeCode::Pointer || code == TypeCode::GCPointer;
	}
	bool Type::IsArray() const noexcept {
		return GetReference().Code == TypeCode::Array;
	}
	bool Type::IsStructure() const noexcept {
		return GetReference().Code >= TypeCode::Structure;
	}
	bool Type::IsValidType() const noexcept {
		return GetReference().Code != TypeCode::None;
	}

	namespace {
		static const TypeInfo s_NoneType(
			"none", TypeCode::None
		);
		static const TypeInfo s_IntType(
			"int", TypeCode::Int,
			sizeof(IntObject), sizeof(RawIntObject), alignof(RawIntObject)
		);
		static const TypeInfo s_LongType(
			"long", TypeCode::Long,
			sizeof(LongObject), sizeof(RawLongObject), alignof(RawLongObject)
		);
		static const TypeInfo s_SingleType(
			"single", TypeCode::Single,
			sizeof(SingleObject), sizeof(RawSingleObject), alignof(RawSingleObject)
		);
		static const TypeInfo s_DoubleType(
			"double", TypeCode::Double,
			sizeof(DoubleObject), sizeof(RawDoubleObject), alignof(RawDoubleObject)
		);
		static const TypeInfo s_PointerType(
			"pointer", TypeCode::Pointer,
			sizeof(PointerObject), sizeof(RawPointerObject), alignof(RawPointerObject)
		);
		static const TypeInfo s_GCPointerType(
			"gcpointer", TypeCode::GCPointer,
			sizeof(GCPointerObject), sizeof(RawGCPointerObject), alignof(RawGCPointerObject)
		);
		static const TypeInfo s_ArrayType(
			"array", TypeCode::Array
		);
	}

	const Type NoneType = s_NoneType;
	const Type IntType = s_IntType;
	const Type LongType = s_LongType;
	const Type SingleType = s_SingleType;
	const Type DoubleType = s_DoubleType;
	const Type PointerType = s_PointerType;
	const Type GCPointerType = s_GCPointerType;
	const Type ArrayType = s_ArrayType;

	Type GetFundamentalType(TypeCode code) noexcept {
		switch (code) {
		case TypeCode::Int: return IntType;
		case TypeCode::Long: return LongType;
		case TypeCode::Single: return SingleType;
		case TypeCode::Double: return DoubleType;
		case TypeCode::Pointer: return PointerType;
		case TypeCode::GCPointer: return GCPointerType;
		default: return NoneType;
		}
	}
}