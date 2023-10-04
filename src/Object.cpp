#include <svm/Object.hpp>

#include <cassert>

namespace svm {
	Object::Object(Type type) noexcept
		: m_Type(type) {

		assert(type != nullptr);
	}

	Type Object::GetType() const noexcept {
		return m_Type;
	}
	void Object::SetType(Type newType) noexcept {
		assert(newType != nullptr);

		m_Type = newType;
	}
}

namespace svm {
	IntObject::IntObject() noexcept
		: Object(IntType) {}
	IntObject::IntObject(RawIntObject rawObject) noexcept
		: Object(IntType), RawObject(rawObject) {}

	PointerObject IntObject::CastToPointer(Type targetType, std::size_t count) const noexcept {
		assert(targetType != nullptr);

		return RawPointerObject{ targetType, count, reinterpret_cast<void*>(RawObject.Value) };
	}
}

namespace svm {
	LongObject::LongObject() noexcept
		: Object(LongType) {}
	LongObject::LongObject(RawLongObject rawObject) noexcept
		: Object(LongType), RawObject(rawObject) {}

	PointerObject LongObject::CastToPointer(Type targetType, std::size_t count) const noexcept {
		assert(targetType != nullptr);

		return RawPointerObject{ targetType, count, reinterpret_cast<void*>(RawObject.Value) };
	}
}

namespace svm {
	SingleObject::SingleObject() noexcept
		: Object(SingleType) {}
	SingleObject::SingleObject(RawSingleObject rawObject) noexcept
		: Object(SingleType), RawObject(rawObject) {}

	PointerObject SingleObject::CastToPointer(Type targetType, std::size_t count) const noexcept {
		assert(targetType != nullptr);

		return RawPointerObject{ targetType, count,
			reinterpret_cast<void*>(static_cast<std::uintptr_t>(RawObject.Value)) };
	}
}

namespace svm {
	DoubleObject::DoubleObject() noexcept
		: Object(DoubleType) {}
	DoubleObject::DoubleObject(RawDoubleObject rawObject) noexcept
		: Object(DoubleType), RawObject(rawObject) {}

	PointerObject DoubleObject::CastToPointer(Type targetType, std::size_t count) const noexcept {
		assert(targetType != nullptr);

		return RawPointerObject{ targetType, count,
			reinterpret_cast<void*>(static_cast<std::uintptr_t>(RawObject.Value)) };
	}
}

namespace svm {
	PointerObject::PointerObject() noexcept
		: Object(PointerType) {}
	PointerObject::PointerObject(RawPointerObject rawObject) noexcept
		: Object(PointerType), RawObject(rawObject) {

		assert(RawObject.Type != nullptr || RawObject.Address == nullptr);
	}

	PointerObject PointerObject::CastToPointer(Type targetType, std::size_t count) const noexcept {
		assert(targetType != nullptr);

		return RawPointerObject{ targetType, count, RawObject.Address };
	}
}

namespace svm {
	GCPointerObject::GCPointerObject() noexcept
		: Object(GCPointerType) {}
	GCPointerObject::GCPointerObject(RawGCPointerObject rawObject) noexcept
		: Object(GCPointerType), RawObject(rawObject) {}
}

namespace svm {
	ArrayObject::ArrayObject(std::size_t count) noexcept
		: Object(ArrayType), Count(count) {

		assert(Count > 0);
	}
}

namespace svm {
	StructureObject::StructureObject(Type type) noexcept
		: Object(type) {

		assert(type.IsStructure());
	}
}