#pragma once

#include <svm/detail/ReferenceWrapper.hpp>

#include <cstddef>
#include <cstdint>
#include <string>

namespace svm {
	enum class TypeCode : std::uint32_t {
		None,

		Byte,				// Not supported
		Short,				// Not supported
		Int,
		Long,
		Float,				// Not supported
		Double,
		Pointer,
		GCPointer,
		Array,
		Structure = 20,
	};

	class TypeInfo final {
	public:
		std::string Name;
		std::uint32_t Module = 0;
		TypeCode Code = TypeCode::None;
		std::size_t Size = 0;

	public:
		TypeInfo() noexcept = default;
		TypeInfo(std::string name, TypeCode code) noexcept;
		TypeInfo(std::string name, TypeCode code, std::size_t size) noexcept;
		TypeInfo(TypeInfo&& typeInfo) noexcept;
		~TypeInfo() = default;

	public:
		TypeInfo& operator=(TypeInfo&& typeInfo) noexcept;
		bool operator==(const TypeInfo&) = delete;
		bool operator!=(const TypeInfo&) = delete;
	};
}

namespace svm {
	class Type final : public detail::ReferenceWrapper<TypeInfo> {
	public:
		using detail::ReferenceWrapper<TypeInfo>::ReferenceWrapper;

	public:
		bool IsFundamentalType() const noexcept;
		bool IsPointer() const noexcept;
		bool IsArray() const noexcept;
		bool IsStructure() const noexcept;
		bool IsValidType() const noexcept;
	};

	extern const Type NoneType;
	extern const Type IntType;
	extern const Type LongType;
	extern const Type DoubleType;
	extern const Type PointerType;
	extern const Type GCPointerType;
	extern const Type ArrayType;

	Type GetFundamentalType(TypeCode code) noexcept;
}