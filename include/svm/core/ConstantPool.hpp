#pragma once

#include <svm/Object.hpp>

#include <cstdint>
#include <limits>
#include <ostream>
#include <vector>

namespace svm::core {
	class ConstantPool {
	public:
		static constexpr std::uint32_t NPos = std::numeric_limits<std::uint32_t>::max();

	private:
		std::vector<IntObject> m_IntPool;
		std::vector<LongObject> m_LongPool;
		std::vector<SingleObject> m_SinglePool;
		std::vector<DoubleObject> m_DoublePool;

	public:
		ConstantPool() noexcept = default;
		ConstantPool(std::vector<IntObject> intPool, std::vector<LongObject> longPool,
			std::vector<SingleObject> singlePool, std::vector<DoubleObject> doublePool) noexcept;
		ConstantPool(ConstantPool&& constantPool) noexcept = default;
		~ConstantPool() = default;

	public:
		ConstantPool& operator=(ConstantPool&& constantPool) noexcept = default;
		bool operator==(const ConstantPool&) const = delete;
		bool operator!=(const ConstantPool&) const = delete;

	public:
		void Clear() noexcept;

		template<typename T>
		const T& GetConstant(std::uint32_t index) const noexcept;
		Type GetConstantType(std::uint32_t index) const noexcept;
		template<typename T>
		std::uint32_t GetOffset() const noexcept;
		std::uint32_t GetIntOffset() const noexcept;
		std::uint32_t GetLongOffset() const noexcept;
		std::uint32_t GetSingleOffset() const noexcept;
		std::uint32_t GetDoubleOffset() const noexcept;
		std::uint32_t GetAllCount() const noexcept;
		template<typename T>
		std::uint32_t GetCount() const noexcept;
		std::uint32_t GetIntCount() const noexcept;
		std::uint32_t GetLongCount() const noexcept;
		std::uint32_t GetSingleCount() const noexcept;
		std::uint32_t GetDoubleCount() const noexcept;

		std::uint32_t AddIntConstant(std::uint32_t value);
		std::uint32_t AddLongConstant(std::uint64_t value);
		std::uint32_t AddSingleConstant(float value);
		std::uint32_t AddDoubleConstant(double value);
		std::uint32_t FindIntConstant(std::uint32_t value) const noexcept;
		std::uint32_t FindLongConstant(std::uint64_t value) const noexcept;
		std::uint32_t FindSingleConstant(float value) const noexcept;
		std::uint32_t FindDoubleConstant(double value) const noexcept;

		const std::vector<IntObject>& GetIntPool() const noexcept;
		const std::vector<LongObject>& GetLongPool() const noexcept;
		const std::vector<SingleObject>& GetSinglePool() const noexcept;
		const std::vector<DoubleObject>& GetDoublePool() const noexcept;
	};

	std::ostream& operator<<(std::ostream& stream, const ConstantPool& constantPool);
}

#include "detail/impl/ConstantPool.hpp"