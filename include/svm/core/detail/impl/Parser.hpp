#pragma once
#include "../../Parser.hpp"

#include <svm/Memory.hpp>

#include <algorithm>
#include <utility>

namespace svm::core {
	template<typename T>
	T Parser::ReadFile() noexcept {
		const auto result = CreateObjectFromBytes<T>(m_File.data() + m_Cursor);

		m_Cursor += sizeof(T);

		if (sizeof(result) > 1 && GetEndian() != Endian::Little)
			return ReverseEndian(result);
		else
			return result;
	}
	inline std::string Parser::ReadFileString() {
		const auto length = ReadFile<std::uint32_t>();
		std::string result(length, 0);

		std::copy(
			m_File.begin() + m_Cursor,
			m_File.begin() + m_Cursor + length,
			result.data()
		);

		m_Cursor += length;

		return result;
	}
	inline auto Parser::ReadFile(std::size_t size) noexcept {
		const auto begin = m_File.begin() + m_Cursor;
		const auto end = m_File.begin() + (m_Cursor += size);

		return std::make_pair(begin, end);
	}

	template<typename T>
	void Parser::ParseMappings(std::vector<T>& mappings) noexcept {
		for (T& mapping : mappings) {
			mapping.Module = ReadFile<std::uint32_t>();
			mapping.Name = ReadFileString();
		}
	}

	template<typename T>
	void Parser::ParseConstants(std::vector<T>& constantPool) noexcept {
		for (T& obj : constantPool) {
			obj.RawObject.Value = ReadFile<decltype(obj.RawObject.Value)>();
		}
	}
}