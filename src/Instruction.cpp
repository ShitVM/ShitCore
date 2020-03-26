#include <svm/Instruction.hpp>

#include <svm/IO.hpp>

#include <utility>

namespace svm {
	OpCode ConvertOpCode(std::uint8_t opCode, ShitBCVersion) noexcept {
		return static_cast<OpCode>(opCode);
	}
	std::uint8_t ConvertOpCode(svm::OpCode opCode, ShitBCVersion) noexcept {
		return static_cast<std::uint8_t>(opCode);
	}
}

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

	std::ostream& operator<<(std::ostream& stream, const Instruction& instruction) {
		stream << QWord(instruction.Offset) << ": " << Mnemonics[static_cast<std::uint8_t>(instruction.OpCode)];
		if (instruction.HasOperand()) {
			stream << " 0x" << Hex(instruction.Operand);
		}
		return stream;
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

	std::ostream& operator<<(std::ostream& stream, const Instructions& instructions) {
		const std::string defIndent = detail::MakeIndent(stream);
		const std::string indentOnce(4, ' ');

		const std::uint32_t labelCount = instructions.GetLabelCount();
		const std::uint64_t instCount = instructions.GetInstructionCount();

		stream << defIndent << "Instructions: " << instructions.GetInstructionCount() << '\n'
			   << defIndent << indentOnce << "Labels: " << instructions.GetLabelCount();

		for (std::uint32_t i = 0; i < labelCount; ++i) {
			const std::uint64_t label = instructions.GetLabel(i);
			stream << '\n' << defIndent << indentOnce << indentOnce
				   << '[' << i << "]: " << label << '(' << QWord(instructions.GetInstruction(label).Offset) << ')';
		}

		for (std::uint64_t i = 0; i < instCount; ++i) {
			stream << '\n' << defIndent << indentOnce << instructions.GetInstruction(i);
		}

		return stream;
	}
}