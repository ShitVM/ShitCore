#pragma once

#include <svm/Specification.hpp>
#include <svm/Structure.hpp>
#include <svm/core/ByteFile.hpp>

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace svm::core {
	class Parser {
	private:
		std::vector<std::uint8_t> m_File;
		std::size_t m_Cursor = 0;

		ByteFile m_ByteFile;
		ShitBFVersion m_ShitBFVersion = ShitBFVersion::Latest;
		ShitBCVersion m_ShitBCVersion = ShitBCVersion::Latest;

	public:
		Parser() noexcept = default;
		Parser(Parser&& parser) noexcept;
		~Parser() = default;

	public:
		Parser& operator=(Parser&& parser) noexcept;
		bool operator==(const Parser&) = delete;
		bool operator!=(const Parser&) = delete;

	public:
		void Clear() noexcept;

		void Open(const std::string& path);
		void Parse();
		ByteFile GetResult() noexcept;

	private:
		template<typename T>
		T ReadFile() noexcept;
		template<>
		inline std::string ReadFile<std::string>();
		inline auto ReadFile(std::size_t size) noexcept;

		void ParseDependencies();
		void ParseConstantPool();
		template<typename T>
		void ParseConstants(std::vector<T>& pool) noexcept;
		void ParseStructures();
		void ParseFunctions();
		void ParseMappings();
		void ParseMappings(std::vector<Mapping>& mappings) noexcept;
		Instructions ParseInstructions();

		void FindCycle() const;
		bool FindCycle(const Structures& structures, std::unordered_map<std::uint32_t, int>& visited, std::vector<Structure>& cycle, std::uint32_t node) const;
		void CalcSize();
		std::size_t CalcSize(Structures& structures, std::uint32_t node);
		void CalcOffset();
		OpCode ReadOpCode() noexcept;
	};
}

#include "detail/impl/Parser.hpp"