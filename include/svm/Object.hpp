#pragma once

#include <svm/Type.hpp>

#include <cstddef>
#include <cstdint>

namespace svm {
	struct RawIntObject final {
		std::uint32_t Value = 0;
	};

	struct RawLongObject final {
		std::uint64_t Value = 0;
	};

	struct RawSingleObject final {
		float Value = 0.0f;
	};

	struct RawDoubleObject final {
		double Value = 0.0;
	};

	struct RawPointerObject final {
		svm::Type Type = nullptr;
		void* Address = nullptr;
	};

	struct RawGCPointerObject final {
		void* Address = nullptr;
	};
}

namespace svm {
	class Object {
	private:
		Type m_Type;

	protected:
		Object() noexcept = default;
		explicit Object(Type type) noexcept;
		Object(const Object& other) noexcept = default;

	public:
		~Object() = default;

	protected:
		Object& operator=(const Object& other) noexcept = default;

	public:
		bool operator==(const Object&) const = delete;
		bool operator!=(const Object&) const = delete;

	public:
		Type GetType() const noexcept;
		void SetType(Type newType) noexcept;
	};
}

namespace svm {
	class IntObject final : public Object {
	public:
		RawIntObject RawObject;

	public:
		IntObject() noexcept;
		IntObject(RawIntObject rawObject) noexcept;
		IntObject(const IntObject& other) noexcept = default;
		~IntObject() = default;

	public:
		IntObject& operator=(const IntObject& other) noexcept = default;

	public:
		template<typename T>
		inline T Cast() const noexcept;
	};

	class LongObject final : public Object {
	public:
		RawLongObject RawObject;

	public:
		LongObject() noexcept;
		LongObject(RawLongObject rawObject) noexcept;
		LongObject(const LongObject& other) noexcept = default;
		~LongObject() = default;

	public:
		LongObject& operator=(const LongObject& other) noexcept = default;

	public:
		template<typename T>
		inline T Cast() const noexcept;
	};

	class SingleObject final : public Object {
	public:
		RawSingleObject RawObject;

	public:
		SingleObject() noexcept;
		SingleObject(RawSingleObject rawObject) noexcept;
		SingleObject(const SingleObject& other) noexcept = default;
		~SingleObject() = default;

	public:
		SingleObject& operator=(const SingleObject& other) noexcept = default;

	public:
		template<typename T>
		inline T Cast() const noexcept;
	};

	class DoubleObject final : public Object {
	public:
		RawDoubleObject RawObject;

	public:
		DoubleObject() noexcept;
		DoubleObject(RawDoubleObject rawObject) noexcept;
		DoubleObject(const DoubleObject& other) noexcept = default;
		~DoubleObject() = default;

	public:
		DoubleObject& operator=(const DoubleObject& other) noexcept = default;

	public:
		template<typename T>
		inline T Cast() const noexcept;
	};

	class PointerObject final : public Object {
	public:
		RawPointerObject RawObject;

	public:
		PointerObject() noexcept;
		PointerObject(RawPointerObject rawObject) noexcept;
		PointerObject(const PointerObject& other) noexcept = default;
		~PointerObject() = default;

	public:
		PointerObject& operator=(const PointerObject& other) noexcept = default;

	public:
		template<typename T>
		inline T Cast() const noexcept;
	};

	class GCPointerObject final : public Object {
	public:
		RawGCPointerObject RawObject;

	public:
		GCPointerObject() noexcept;
		GCPointerObject(RawGCPointerObject rawObject) noexcept;
		GCPointerObject(const GCPointerObject& other) noexcept = default;
		~GCPointerObject() = default;

	public:
		GCPointerObject& operator=(const GCPointerObject& other) noexcept = default;
	};

	class ArrayObject final : public Object {
	public:
		std::size_t Count = 0;

	public:
		explicit ArrayObject(std::size_t count) noexcept;
		ArrayObject(const ArrayObject&) = delete;
		~ArrayObject() = default;

	public:
		ArrayObject& operator=(const ArrayObject&) = delete;
	};

	class StructureObject final : public Object {
	public:
		explicit StructureObject(Type type) noexcept;
		StructureObject(const StructureObject&) = delete;
		~StructureObject() = default;

	public:
		StructureObject& operator=(const StructureObject&) = delete;
	};
}

#include "detail/impl/Object.hpp"