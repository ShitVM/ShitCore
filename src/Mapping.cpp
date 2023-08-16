#include <svm/Mapping.hpp>

#include <svm/IO.hpp>

#include <utility>

namespace svm {
	std::ostream& operator<<(std::ostream& stream, const Mapping& mapping) {
		const std::string defIndent = detail::MakeIndent(stream);

		stream << defIndent << "Module: [" << mapping.Module << "]\n"
			   << defIndent << "Name: \"" << mapping.Name << '"';
		return stream;
	}
}

namespace svm {
	Mappings::Mappings(std::vector<StructureMapping> structures, std::vector<FunctionMapping> functions) noexcept
		: m_StructureMappings(std::move(structures)), m_FunctionMappings(std::move(functions)) {}
	Mappings::Mappings(Mappings&& mappings) noexcept
		: m_StructureMappings(std::move(mappings.m_StructureMappings)), m_FunctionMappings(std::move(mappings.m_FunctionMappings)) {}

	Mappings& Mappings::operator=(Mappings&& mappings) noexcept {
		m_StructureMappings = std::move(mappings.m_StructureMappings);
		m_FunctionMappings = std::move(mappings.m_FunctionMappings);
		return *this;
	}

	void Mappings::Clear() noexcept {
		m_StructureMappings.clear();
		m_FunctionMappings.clear();
	}

	void Mappings::AddStructureMapping(std::uint32_t module, std::string name) {
		m_StructureMappings.push_back({ module, name });

		m_StructureMappings.back().TempType.Name = std::move(name);
		m_StructureMappings.back().TempType.Module = module + 1;
	}
	void Mappings::AddFunctionMapping(std::uint32_t module, std::string name) {
		m_FunctionMappings.push_back({ module, std::move(name) });
	}

	const StructureMapping& Mappings::GetStructureMapping(std::uint32_t index) const noexcept {
		return m_StructureMappings[index];
	}
	std::uint32_t Mappings::GetStructureMappingCount() const noexcept {
		return static_cast<std::uint32_t>(m_StructureMappings.size());
	}
	const FunctionMapping& Mappings::GetFunctionMapping(std::uint32_t index) const noexcept {
		return m_FunctionMappings[index];
	}
	std::uint32_t Mappings::GetFunctionMappingCount() const noexcept {
		return static_cast<std::uint32_t>(m_FunctionMappings.size());
	}

	std::ostream& operator<<(std::ostream& stream, const Mappings& mappings) {
		const std::string defIndent = detail::MakeIndent(stream);
		const std::string indentOnce(4, ' ');

		const std::uint32_t structureMappingCount = mappings.GetStructureMappingCount();
		const std::uint64_t functionMappingCount = mappings.GetFunctionMappingCount();

		stream << defIndent << "Mappings:\n"
			   << defIndent << indentOnce << "Structures: " << structureMappingCount
			   << Indent << Indent << Indent;

		for (std::uint32_t i = 0; i < structureMappingCount; ++i) {
			const Mapping& mapping = mappings.GetStructureMapping(i);
			stream << '\n' << defIndent << indentOnce << indentOnce << '[' << i << "]:\n" << mapping;
		}

		stream << '\n' << defIndent << indentOnce << "Functions: " << functionMappingCount;

		for (std::uint32_t i = 0; i < functionMappingCount; ++i) {
			const Mapping& mapping = mappings.GetFunctionMapping(i);
			stream << '\n' << defIndent << indentOnce << indentOnce << '[' << i << "]:\n" << mapping;
		}

		stream << UnIndent << UnIndent << UnIndent;
		return stream;
	}
}