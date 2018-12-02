#include "disassembler.h"

QString GetOperationName(quint8 operation)
{
	switch (operation)
	{
	case CCPU::InstructionCode::NOP: return "nop";
	case CCPU::InstructionCode::RET: return "ret";
	case CCPU::InstructionCode::INT: return "int";
	case CCPU::InstructionCode::BRK: return "int 3";
	case CCPU::InstructionCode::ADD: return "add";
	case CCPU::InstructionCode::SUB: return "sub";
	case CCPU::InstructionCode::MUL: return "mul";
	case CCPU::InstructionCode::DIV: return "div";
	case CCPU::InstructionCode::HLT: return "halt";
	case CCPU::InstructionCode::PUSH: return "push";
	case CCPU::InstructionCode::POP: return "pop";
	case CCPU::InstructionCode::PUSHF: return "push f";
	case CCPU::InstructionCode::POPF: return "pop f";
	case CCPU::InstructionCode::CALL: return "call";
	case CCPU::InstructionCode::ASG: return "assign";
	case CCPU::InstructionCode::LOD: return "load";
	case CCPU::InstructionCode::STR: return "store";
	case CCPU::InstructionCode::MOV: return "mov";
	case CCPU::InstructionCode::CMP: return "cmp";
	case CCPU::InstructionCode::JMP: return "jmp";
	case CCPU::InstructionCode::JMPC: return "jmpc";
	case CCPU::InstructionCode::SWP: return "swap";
	case CCPU::InstructionCode::AND: return "and";
	case CCPU::InstructionCode::OR: return "or";
	case CCPU::InstructionCode::XOR: return "xor";
	case CCPU::InstructionCode::NOT: return "not";
	case CCPU::InstructionCode::NAND: return "nand";
	case CCPU::InstructionCode::NOR: return "nor";
	case CCPU::InstructionCode::INC: return "inc";
	case CCPU::InstructionCode::DEC: return "dec";
	case CCPU::InstructionCode::ADDS: return "adds";
	case CCPU::InstructionCode::SUBS: return "adds";
	case CCPU::InstructionCode::MULS: return "muls";
	case CCPU::InstructionCode::DIVS: return "divs";
	}

	return "nop";
}

CDisassembler::CDisassembler()
{
}

CDisassembler::~CDisassembler()
{
}

QString CDisassembler::Disassemble(quint8 instruction[8])
{
	QString result = GetOperationName(instruction[0]);
	switch (instruction[0])
	{
	case CCPU::InstructionCode::ADD: 
	case CCPU::InstructionCode::SUB: 
	case CCPU::InstructionCode::MUL: 
	case CCPU::InstructionCode::DIV:
	{
		if ((instruction[1] & CCPU::InstructionComponents::SIZE_MASK) == CCPU::InstructionComponents::BYTE)
			result += " byte r";
		else if ((instruction[1] & CCPU::InstructionComponents::SIZE_MASK) == CCPU::InstructionComponents::WORD)
			result += " word r";
		else if ((instruction[1] & CCPU::InstructionComponents::SIZE_MASK) == CCPU::InstructionComponents::DOUBLE_WORD)
			result += " dword r";
		else if ((instruction[1] & CCPU::InstructionComponents::SIZE_MASK) == CCPU::InstructionComponents::QUAD_WORD)
			result += " qword r";

		result += QString("%1,%2").arg(instruction[2]).arg((qint32)instruction[3]);
		break;
	}
	case CCPU::InstructionCode::ADDS:
	case CCPU::InstructionCode::SUBS:
	case CCPU::InstructionCode::MULS:
	case CCPU::InstructionCode::DIVS:
	{
		if ((instruction[1] & CCPU::InstructionComponents::SIZE_MASK) == CCPU::InstructionComponents::BYTE)
			result += " byte r";
		else if ((instruction[1] & CCPU::InstructionComponents::SIZE_MASK) == CCPU::InstructionComponents::WORD)
			result += " word r";
		else if ((instruction[1] & CCPU::InstructionComponents::SIZE_MASK) == CCPU::InstructionComponents::DOUBLE_WORD)
			result += " dword r";
		else if ((instruction[1] & CCPU::InstructionComponents::SIZE_MASK) == CCPU::InstructionComponents::QUAD_WORD)
			result += " qword r";

		result += QString("%1,%2").arg(instruction[2]).arg((quint32)instruction[3]);
		break;
	}
	case CCPU::InstructionCode::PUSH:
	case CCPU::InstructionCode::POP:
	{
		result += QString(" %1%2").arg(instruction[1] & 0x80 ? 'a' : 'r').arg(QString::number(instruction[1] & 0x80 ? instruction[1] & 0x07 : instruction[1] & 0x3F));
		break;
	}
	case CCPU::InstructionCode::CALL:
		result += QString(" %1h").arg(*(quint32*)(instruction + 1), 8, 16, QChar('0'));
		break;
	case CCPU::InstructionCode::ASG:
	case CCPU::InstructionCode::LOD:
	case CCPU::InstructionCode::STR:
	case CCPU::InstructionCode::MOV:
	case CCPU::InstructionCode::JMP:
	case CCPU::InstructionCode::SWP:
	case CCPU::InstructionCode::AND:
	case CCPU::InstructionCode::OR:
	case CCPU::InstructionCode::XOR:
	case CCPU::InstructionCode::NOT:
	case CCPU::InstructionCode::NAND:
	case CCPU::InstructionCode::NOR:
	case CCPU::InstructionCode::INC:
	case CCPU::InstructionCode::DEC:
		break;
	}

	return result;
}
