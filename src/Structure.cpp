#include <svm/Structure.hpp>

#include <utility>

namespace svm {
	StructureInfo::StructureInfo(std::vector<Field> fields, TypeInfo&& type) noexcept
		: Fields(std::move(fields)), Type(std::move(type)) {}
	StructureInfo::StructureInfo(StructureInfo&& structInfo) noexcept
		: Fields(std::move(structInfo.Fields)), Type(std::move(structInfo.Type)) {}

	StructureInfo& StructureInfo::operator=(StructureInfo&& structInfo) noexcept {
		Fields = std::move(structInfo.Fields);
		Type = std::move(structInfo.Type);
		return *this;
	}
}