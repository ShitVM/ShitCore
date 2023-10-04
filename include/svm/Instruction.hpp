#pragma once

#include <svm/Specification.hpp>

#include <cstdint>
#include <ostream>
#include <vector>

namespace svm {
	enum class OpCode : std::uint8_t {
		Nop,

		Push,
		Pop,
		Load,
		Store,
		Lea,
		FLea,
		TLoad,
		TStore,
		Copy,
		Swap,

		Add,
		Sub,
		Mul,
		IMul,
		Div,
		IDiv,
		Mod,
		IMod,
		Neg,
		Inc,
		Dec,

		And,
		Or,
		Xor,
		Not,
		Shl,
		Sal,
		Shr,
		Sar,

		Cmp,
		ICmp,
		Jmp,
		Je,
		Jne,
		Ja,
		Jae,
		Jb,
		Jbe,
		Call,
		Ret,

		ToB,				// Not supported
		ToSh,				// Not supported
		ToI,
		ToL,
		ToSi,
		ToD,
		ToP,

		Null,
		New,
		Delete,
		GCNull,
		GCNew,

		APush,
		ANew,
		AGCNew,
		ALea,
		Count,
	};

	static constexpr const char* Mnemonics[] = {
		"nop",
		"push", "pop", "load", "store", "lea", "flea", "tload", "tstore", "copy", "swap",
		"add", "sub", "mul", "imul", "div", "idiv", "mod", "imod", "neg", "inc", "dec",
		"and", "or", "xor", "not", "shl", "shr", "sal", "sar",
		"cmp", "icmp", "jmp", "je", "jne", "ja", "jae", "jb", "jbe", "call", "ret",
		"tob", "tosh", "toi", "tol", "tosi", "tod", "top",
		"null", "new", "delete", "gcnull", "gcnew",
		"apush", "anew", "agcnew", "alea", "count",
	};

	static constexpr bool HasOperand[] = {
		false/*nop*/,
		true/*push*/, false/*pop*/, true/*load*/, true/*store*/, true/*lea*/, true/*flea*/, false/*tload*/, false/*tstore*/, false/*copy*/, false/*swap*/,
		false/*add*/, false/*sub*/, false/*mul*/, false/*imul*/, false/*div*/, false/*idiv*/, false/*mod*/, false/*imod*/, false/*neg*/, false/*inc*/, false/*dec*/,
		false/*and*/, false/*or*/, false/*xor*/, false/*not*/, false/*shl*/, false/*shr*/, false/*sal*/, false/*sar*/,
		false/*cmp*/, false/*icmp*/, true/*jmp*/, true/*je*/, true/*jne*/, true/*ja*/, true/*jae*/, true/*jb*/, true/*jbe*/, true/*call*/, false/*ret*/,
		false/*tob*/, false/*tosh*/, false/*toi*/, false/*tol*/, false/*tosi*/, false/*tod*/, true/*top*/,
		false/*null*/, true/*new*/, false/*delete*/, false/*gcnull*/, true/*gcnew*/,
		true/*apush*/, true/*anew*/, true/*agcnew*/, false/*alea*/, false/*count*/,
	};

	OpCode ConvertOpCode(std::uint8_t opCode, ShitBCVersion version) noexcept;
	std::uint8_t ConvertOpCode(OpCode opCode, ShitBCVersion version) noexcept;
}

namespace svm {
	class Instruction final {
	public:
		svm::OpCode OpCode = svm::OpCode::Nop;
		std::uint32_t Operand = 0;
		std::uint64_t Offset = 0;
		std::uint8_t OperandIndex = 0;

	public:
		Instruction() noexcept = default;
		Instruction(svm::OpCode opCode) noexcept;
		Instruction(svm::OpCode opCode, std::uint64_t offset) noexcept;
		Instruction(svm::OpCode opCode, std::uint32_t operand, std::uint64_t offset) noexcept;
		Instruction(svm::OpCode opCode, std::uint32_t operand, std::uint8_t operandIndex) noexcept;
		Instruction(const Instruction& instruction) noexcept;
		~Instruction() = default;

	public:
		Instruction& operator=(const Instruction& instruction) noexcept;
		bool operator==(const Instruction&) = delete;
		bool operator!=(const Instruction&) = delete;

	public:
		bool HasOperand() const noexcept;
	};

	std::ostream& operator<<(std::ostream& stream, const Instruction& instruction);
}

namespace svm {
	class Instructions final {
	private:
		std::vector<std::uint64_t> m_Labels;
		std::vector<Instruction> m_Instructions;

	public:
		Instructions() noexcept = default;
		Instructions(std::vector<std::uint64_t> labels, std::vector<Instruction> instructions) noexcept;
		Instructions(Instructions&& instructions) noexcept;
		~Instructions() = default;

	public:
		Instructions& operator=(Instructions&& instructions) noexcept;
		bool operator==(const Instructions&) = delete;
		bool operator!=(const Instructions&) = delete;

	public:
		void Clear() noexcept;

		std::uint64_t GetLabel(std::uint32_t index) const noexcept;
		std::uint32_t GetLabelCount() const noexcept;
		const Instruction& GetInstruction(std::uint64_t index) const noexcept;
		std::uint64_t GetInstructionCount() const noexcept;

		std::uint32_t AddLabel(std::uint64_t index);
		void SetLabel(std::uint32_t index, std::uint64_t label) noexcept;
		std::uint64_t AddInstruction(const Instruction& instruction);
	};

	std::ostream& operator<<(std::ostream& stream, const Instructions& instructions);
}