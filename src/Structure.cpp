#include <svm/Structure.hpp>

#include <svm/IO.hpp>
#include <svm/Object.hpp>

#include <utility>

namespace svm {
	bool Field::IsArray() const noexcept {
		return Count >= 1;
	}
}

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

	std::ostream& operator<<(std::ostream& stream, const StructureInfo& structure) {
		const std::string defIndent = detail::MakeIndent(stream);
		const std::string indentOnce(4, ' ');

		const std::uint32_t fieldCount = static_cast<std::uint32_t>(structure.Fields.size());

		stream << defIndent << "Structure: " << structure.Type.Size << "B\n"
			   << defIndent << indentOnce << "Fields: " << fieldCount;
		for (std::uint32_t i = 0; i < fieldCount; ++i) {
			const Field& field = structure.Fields[i];
			stream << '\n' << defIndent << indentOnce << indentOnce << '[' << i << "]: " << field.Type->Name;
			if (field.IsArray()) {
				stream << '[' << field.Count << "](" << field.Type->Size * field.Count + sizeof(ArrayObject);
			} else {
				stream << field.Type->Size;
			}
			stream << "B)";
		}
		return stream;
	}
}

namespace svm {
	std::ostream& operator<<(std::ostream& stream, const Structure& structure) {
		return stream << *structure;
	}
	std::ostream& operator<<(std::ostream& stream, const Structures& structures) {
		const std::string defIndent = detail::MakeIndent(stream);
		const std::string indentOnce(4, ' ');

		stream << defIndent << "Structures: " << structures.size() << Indent << Indent;
		for (std::uint32_t i = 0; i < structures.size(); ++i) {
			stream << '\n' << defIndent << indentOnce << '[' << i << "]:\n" << structures[i];
		}
		return stream << UnIndent << UnIndent;
	}
}