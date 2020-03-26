#pragma once

#include <svm/Type.hpp>
#include <svm/detail/ReferenceWrapper.hpp>

#include <cstddef>
#include <cstdint>
#include <ostream>
#include <vector>

namespace svm {
	struct Field final {
		std::size_t Offset = 0;
		svm::Type Type;
		std::uint64_t Count = 0;

		bool IsArray() const noexcept;
	};
}

namespace svm {
	class StructureInfo {
	public:
		std::vector<Field> Fields;
		TypeInfo Type;

	public:
		StructureInfo() noexcept = default;
		StructureInfo(std::vector<Field> fields, TypeInfo&& type) noexcept;
		StructureInfo(StructureInfo&& structInfo) noexcept;
		~StructureInfo() = default;

	public:
		StructureInfo& operator=(StructureInfo&& structInfo) noexcept;
		bool operator==(const StructureInfo&) = delete;
		bool operator!=(const StructureInfo&) = delete;
	};

	std::ostream& operator<<(std::ostream& stream, const StructureInfo& structure);
}

namespace svm {
	class Structure final : public detail::ReferenceWrapper<StructureInfo> {
	public:
		using detail::ReferenceWrapper<StructureInfo>::ReferenceWrapper;
	};

	using Structures = std::vector<StructureInfo>;

	std::ostream& operator<<(std::ostream& stream, const Structure& structure);
	std::ostream& operator<<(std::ostream& stream, const Structures& structures);
}