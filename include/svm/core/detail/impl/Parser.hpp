#pragma once
#include <svm/core/Parser.hpp>

#include <svm/Memory.hpp>

namespace svm::core {
	template<typename T>
	T Parser::ReadFile() noexcept {
		T result = reinterpret_cast<T&>(m_File[m_Cursor]);
		m_Cursor += sizeof(result);

		if (sizeof(result) > 1 && GetEndian() != Endian::Little) return ReverseEndian(result);
		else return result;
	}
	inline std::string Parser::ReadFileString() {
		const std::uint32_t length = ReadFile<std::uint32_t>();
		std::string result(length, 0);
		std::copy(m_File.begin() + m_Cursor, m_File.begin() + m_Cursor + length, result.data());
		m_Cursor += length;
		return result;
	}
	inline auto Parser::ReadFile(std::size_t size) noexcept {
		const auto begin = m_File.begin() + m_Cursor;
		const auto end = m_File.begin() + (m_Cursor += size);
		return std::make_pair(begin, end);
	}

	template<typename T>
	void Parser::ParseConstants(std::vector<T>& pool) noexcept {
		for (T& obj : pool) {
			obj.Value = ReadFile<decltype(obj.Value)>();
		}
	}
}