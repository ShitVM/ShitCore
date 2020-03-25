#include <svm/Instruction.hpp>

#include <utility>

namespace svm {
	Instruction::Instruction(svm::OpCode opCode) noexcept
		: OpCode(opCode) {}
	Instruction::Instruction(svm::OpCode opCode, std::uint64_t offset) noexcept
		: OpCode(opCode), Offset(offset) {}
	Instruction::Instruction(svm::OpCode opCode, std::uint32_t operand) noexcept
		: OpCode(opCode), Operand(operand) {}
	Instruction::Instruction(svm::OpCode opCode, std::uint32_t operand, std::uint64_t offset) noexcept
		: OpCode(opCode), Operand(operand), Offset(offset) {}

	Instruction& Instruction::operator=(const Instruction& instruction) noexcept {
		OpCode = instruction.OpCode;
		Operand = instruction.Operand;
		Offset = instruction.Offset;
		return *this;
	}
	bool Instruction::operator==(const Instruction& rhs) const noexcept {
		return OpCode == rhs.OpCode && Operand == rhs.Operand;
	}
	bool Instruction::operator!=(const Instruction& rhs) const noexcept {
		return OpCode != rhs.OpCode || Operand != rhs.Operand;
	}

	bool Instruction::HasOperand() const noexcept {
		return svm::HasOperand[static_cast<std::uint8_t>(OpCode)];
	}
}

namespace svm {
	Instructions::Instructions(std::vector<std::uint64_t> labels, std::vector<Instruction> instructions) noexcept
		: m_Labels(std::move(labels)), m_Instructions(std::move(instructions)) {}
	Instructions::Instructions(Instructions&& instructions) noexcept
		: m_Labels(std::move(instructions.m_Labels)), m_Instructions(std::move(instructions.m_Instructions)) {}

	Instructions& Instructions::operator=(Instructions&& instructions) noexcept {
		m_Labels = std::move(instructions.m_Labels);
		m_Instructions = std::move(instructions.m_Instructions);
		return *this;
	}

	void Instructions::Clear() noexcept {
		m_Labels.clear();
		m_Instructions.clear();
	}

	std::uint64_t Instructions::GetLabel(std::uint32_t index) const noexcept {
		return m_Labels[index];
	}
	std::uint32_t Instructions::GetLabelCount() const noexcept {
		return static_cast<std::uint32_t>(m_Labels.size());
	}
	const Instruction& Instructions::GetInstruction(std::uint64_t index) const noexcept {
		return m_Instructions[static_cast<std::size_t>(index)];
	}
	std::uint64_t Instructions::GetInstructionCount() const noexcept {
		return m_Instructions.size();
	}
	std::uint32_t Instructions::AddLabel(std::uint64_t index) {
		m_Labels.push_back(index);
		return static_cast<std::uint32_t>(m_Labels.size() - 1);
	}
	void Instructions::SetLabel(std::uint32_t index, std::uint64_t label) noexcept {
		m_Labels[index] = label;
	}
	std::uint64_t Instructions::AddInstruction(const Instruction& instruction) {
		m_Instructions.push_back(instruction);
		return m_Instructions.size() - 1;
	}
}