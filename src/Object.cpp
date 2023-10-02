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

	LongObject::LongObject() noexcept
		: Object(LongType) {}
	LongObject::LongObject(RawLongObject rawObject) noexcept
		: Object(LongType), RawObject(rawObject) {}

	SingleObject::SingleObject() noexcept
		: Object(SingleType) {}
	SingleObject::SingleObject(RawSingleObject rawObject) noexcept
		: Object(SingleType), RawObject(rawObject) {}

	DoubleObject::DoubleObject() noexcept
		: Object(DoubleType) {}
	DoubleObject::DoubleObject(RawDoubleObject rawObject) noexcept
		: Object(DoubleType), RawObject(rawObject) {}

	PointerObject::PointerObject() noexcept
		: Object(PointerType) {}
	PointerObject::PointerObject(RawPointerObject rawObject) noexcept
		: Object(PointerType), RawObject(rawObject) {

		assert(RawObject.Type != nullptr || RawObject.Address == nullptr);
	}

	GCPointerObject::GCPointerObject() noexcept
		: Object(GCPointerType) {}
	GCPointerObject::GCPointerObject(RawGCPointerObject rawObject) noexcept
		: Object(GCPointerType), RawObject(rawObject) {}

	ArrayObject::ArrayObject(std::size_t count) noexcept
		: Object(ArrayType), Count(count) {

		assert(Count > 0);
	}

	StructureObject::StructureObject(Type type) noexcept
		: Object(type) {

		assert(type.IsStructure());
	}
}