#include <svm/core/ConstantPool.hpp>

#include <svm/IO.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <string>
#include <utility>

namespace svm::core {
	ConstantPool::ConstantPool(std::vector<IntObject> intPool, std::vector<LongObject> longPool, std::vector<DoubleObject> doublePool) noexcept
		: m_IntPool(std::move(intPool)), m_LongPool(std::move(longPool)), m_DoublePool(std::move(doublePool)) {}
	ConstantPool::ConstantPool(ConstantPool&& constantPool) noexcept
		: m_IntPool(std::move(constantPool.m_IntPool)), m_LongPool(std::move(constantPool.m_LongPool)), m_DoublePool(std::move(constantPool.m_DoublePool)) {}

	ConstantPool& ConstantPool::operator=(ConstantPool&& constantPool) noexcept {
		m_IntPool = std::move(constantPool.m_IntPool);
		m_LongPool = std::move(constantPool.m_LongPool);
		m_DoublePool = std::move(constantPool.m_DoublePool);
		return *this;
	}

	void ConstantPool::Clear() noexcept {
		m_IntPool.clear();
		m_LongPool.clear();
		m_DoublePool.clear();
	}

	Type ConstantPool::GetConstantType(std::uint32_t index) const noexcept {
		assert(index < GetAllCount());

		if (index >= GetDoubleOffset()) return DoubleType;
		else if (index >= GetLongOffset()) return LongType;
		else return IntType;
	}
	std::uint32_t ConstantPool::GetIntOffset() const noexcept {
		return 0;
	}
	std::uint32_t ConstantPool::GetLongOffset() const noexcept {
		return GetIntOffset() + GetIntCount();
	}
	std::uint32_t ConstantPool::GetDoubleOffset() const noexcept {
		return GetLongOffset() + GetLongCount();
	}
	std::uint32_t ConstantPool::GetAllCount() const noexcept {
		return GetIntCount() + GetLongCount() + GetDoubleCount();
	}
	std::uint32_t ConstantPool::GetIntCount() const noexcept {
		return static_cast<std::uint32_t>(m_IntPool.size());
	}
	std::uint32_t ConstantPool::GetLongCount() const noexcept {
		return static_cast<std::uint32_t>(m_LongPool.size());
	}
	std::uint32_t ConstantPool::GetDoubleCount() const noexcept {
		return static_cast<std::uint32_t>(m_DoublePool.size());
	}

	std::uint32_t ConstantPool::AddIntConstant(std::uint32_t value) {
		m_IntPool.push_back(value);
		return GetIntCount() - 1;
	}
	std::uint32_t ConstantPool::AddLongConstant(std::uint64_t value) {
		m_LongPool.push_back(value);
		return GetLongCount() - 1;
	}
	std::uint32_t ConstantPool::AddDoubleConstant(double value) {
		m_DoublePool.push_back(value);
		return GetDoubleCount() - 1;
	}
	std::uint32_t ConstantPool::FindIntConstant(std::uint32_t value) const noexcept {
		const auto iter = std::find_if(m_IntPool.begin(), m_IntPool.end(), [value](const auto& object) {
			return object.Value == value;
		});
		if (iter == m_IntPool.end()) return NPos;
		else return static_cast<std::uint32_t>(std::distance(m_IntPool.begin(), iter));
	}
	std::uint32_t ConstantPool::FindLongConstant(std::uint64_t value) const noexcept {
		const auto iter = std::find_if(m_LongPool.begin(), m_LongPool.end(), [value](const auto& object) {
			return object.Value == value;
		});
		if (iter == m_LongPool.end()) return NPos;
		else return static_cast<std::uint32_t>(std::distance(m_LongPool.begin(), iter));
	}
	std::uint32_t ConstantPool::FindDoubleConstant(double value) const noexcept {
		const auto iter = std::find_if(m_DoublePool.begin(), m_DoublePool.end(), [value](const auto& object) {
			return object.Value == value;
		});
		if (iter == m_DoublePool.end()) return NPos;
		else return static_cast<std::uint32_t>(std::distance(m_DoublePool.begin(), iter));
	}

	const std::vector<IntObject>& ConstantPool::GetIntPool() const noexcept {
		return m_IntPool;
	}
	void ConstantPool::SetIntPool(std::vector<IntObject> newIntPool) noexcept {
		m_IntPool = std::move(newIntPool);
	}
	const std::vector<LongObject>& ConstantPool::GetLongPool() const noexcept {
		return m_LongPool;
	}
	void ConstantPool::SetLongPool(std::vector<LongObject> newLongPool) noexcept {
		m_LongPool = std::move(newLongPool);
	}
	const std::vector<DoubleObject>& ConstantPool::GetDoublePool() const noexcept {
		return m_DoublePool;
	}
	void ConstantPool::GetDoublePool(std::vector<DoubleObject> newDoublePool) noexcept {
		m_DoublePool = std::move(newDoublePool);
	}

	namespace {
		template<typename T>
		void PrintConstant(std::ostream& stream, const ConstantPool& constantPool, const std::string& defIndent, const std::string& indentOnce, std::uint32_t i) {
			const auto& constant = constantPool.GetConstant<T>(i);
			stream << '\n' << defIndent << indentOnce << '[' << i << "]: " << constant.GetType()->Name << '(' << constant.Value << ')';
		}
	}

	std::ostream& operator<<(std::ostream& stream, const ConstantPool& constantPool) {
		const std::string defIndent = detail::MakeIndent(stream);
		const std::string indentOnce(4, ' ');

		stream << defIndent << "ConstantPool: " << constantPool.GetAllCount();

		static constexpr std::uint32_t((ConstantPool::*types[])() const noexcept) = {
			&ConstantPool::GetIntCount,
			&ConstantPool::GetLongCount,
			&ConstantPool::GetDoubleCount,
		};

		std::uint32_t i = 0;
		for (auto type : types) {
			const std::uint32_t count = (constantPool.*type)();
			const std::uint32_t end = i + count;
			for (; i < end; ++i) {
				if (type == &ConstantPool::GetIntCount) {
					PrintConstant<IntObject>(stream, constantPool, defIndent, indentOnce, i);
				} else if (type == &ConstantPool::GetLongCount) {
					PrintConstant<LongObject>(stream, constantPool, defIndent, indentOnce, i);
				} else if (type == &ConstantPool::GetDoubleCount) {
					PrintConstant<DoubleObject>(stream, constantPool, defIndent, indentOnce, i);
				}
			}
		}

		return stream;
	}
}