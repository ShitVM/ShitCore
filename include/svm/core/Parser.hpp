#pragma once

#include <svm/Specification.hpp>
#include <svm/Structure.hpp>
#include <svm/Type.hpp>
#include <svm/core/ByteFile.hpp>

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
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
		Parser(Parser&& other) noexcept = default;
		~Parser() = default;

	public:
		Parser& operator=(Parser&& other) noexcept = default;
		bool operator==(const Parser&) const = delete;

	public:
		void Clear() noexcept;

		void Open(const std::filesystem::path& path);
		void Parse();
		ByteFile GetResult() noexcept;

	private:
		template<typename T>
		T ReadFile() noexcept;
		inline std::string ReadFileString();
		inline auto ReadFile(std::size_t size) noexcept;

		Type GetType(Structures& structures, TypeCode code);

		void ParseDependencies();
		void ParseMappings();
		template<typename T>
		void ParseMappings(std::vector<T>& mappings) noexcept;
		void ParseConstantPool();
		template<typename T>
		void ParseConstants(std::vector<T>& constantPool) noexcept;
		void ParseStructures();
		void ParseFunctions();
		Instructions ParseInstructions();

		OpCode ReadOpCode() noexcept;
	};
}

#include "detail/impl/Parser.hpp"