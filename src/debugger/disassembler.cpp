#include "disassembler.h"
#include <QFile>

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
	case CCPU::InstructionCode::IN: return "in";
	case CCPU::InstructionCode::OUT: return "out";
	}

	return "???";
}

QString Disassemble(quint8 instruction[8])
{
	QString result = GetOperationName(instruction[0]);
	switch (instruction[0])
	{
	case CCPU::InstructionCode::ADD: 
	case CCPU::InstructionCode::SUB: 
	case CCPU::InstructionCode::MUL: 
	case CCPU::InstructionCode::DIV:
	case CCPU::InstructionCode::ADDS:
	case CCPU::InstructionCode::SUBS:
	case CCPU::InstructionCode::MULS:
	case CCPU::InstructionCode::DIVS:
	case CCPU::InstructionCode::AND:
	case CCPU::InstructionCode::OR:
	case CCPU::InstructionCode::XOR:
	case CCPU::InstructionCode::NOT:
	case CCPU::InstructionCode::NAND:
	case CCPU::InstructionCode::NOR:
	{
		if ((instruction[1] & CCPU::InstructionComponents::SIZE_MASK) == CCPU::InstructionComponents::BYTE)
			result += " byte r";
		else if ((instruction[1] & CCPU::InstructionComponents::SIZE_MASK) == CCPU::InstructionComponents::WORD)
			result += " word r";
		else if ((instruction[1] & CCPU::InstructionComponents::SIZE_MASK) == CCPU::InstructionComponents::DOUBLE_WORD)
			result += " dword r";
		else if ((instruction[1] & CCPU::InstructionComponents::SIZE_MASK) == CCPU::InstructionComponents::QUAD_WORD)
			result += " qword r";

		result += QString("%1, %2").arg(instruction[2]).arg((qint32)instruction[3]);
		break;
	}
	case CCPU::InstructionCode::PUSH:
	case CCPU::InstructionCode::POP:
	{
		result += QString(" %1%2").arg(instruction[1] & 0x80 ? 'a' : 'r').arg(QString::number(instruction[1] & 0x80 ? instruction[1] & 0x07 : instruction[1] & 0x3F));
		break;
	}
	case CCPU::InstructionCode::JMPC:
	{
		switch(instruction[1])
		{
		case CCPU::EQUAL: result = "je"; break;
		case CCPU::NOT_EQUAL: result = "jne"; break;
		case CCPU::ABOVE: result = "ja"; break;
		case CCPU::ABOVE_OR_EQUAL: result = "jae"; break;
		case CCPU::BELOW: result = "jb"; break;
		case CCPU::BELOW_OR_EQUAL: result = "jbe"; break;
		case CCPU::GREATER: result = "jg"; break;
		case CCPU::GREATER_OR_EQUAL: result = "jge"; break;
		case CCPU::LESS: result = "jl"; break;
		case CCPU::LESS_OR_EQUAL: result = "jle"; break;
		case CCPU::OVERFLOW_SET: result = "jo"; break;
		case CCPU::NOT_OVERFLOW: result = "jno"; break;
		case CCPU::SIGN_SET: result = "js"; break;
		case CCPU::NOT_SIGN: result = "jns"; break;
		case CCPU::PARITY_SET: result = "jp"; break;
		case CCPU::NOT_PARITY: result = "jnp"; break;
		}
		quint32 address = *(quint32*)(instruction + 2);
		if (s_mapRelocaionTable.contains(address))
			result += QString(" %1").arg(s_mapRelocaionTable[address]);
		else
			result += QString(" %1h").arg(address, 8, 16, QChar('0'));
		break;
	}
	case CCPU::InstructionCode::CALL:
	case CCPU::InstructionCode::JMP:
	{
		quint32 address = *(quint32*)(instruction + 1);
		if(s_mapRelocaionTable.contains(address))
			result += QString(" %1").arg(s_mapRelocaionTable[address]);
		else
			result += QString(" %1h").arg(address, 8, 16, QChar('0'));
		break;
	} 
	case CCPU::InstructionCode::ASG:
		result += QString(" %1, %2h")
			.arg(QString("%1%2").arg(instruction[1] & 0x80 ? 'a' : 'r').arg(instruction[1] & 0x80 ? instruction[1] & 0x07 : instruction[1] & 0x3F))
			.arg(*(quint32*)(instruction + 2), 8, 16, QChar('0'));
		break;
	case CCPU::InstructionCode::LOD:
		result += QString(" %1, a%2")
			.arg(QString("%1%2").arg(instruction[1] & 0x80 ? 'a' : 'r').arg(instruction[1] & 0x80 ? instruction[1] & 0x07 : instruction[1] & 0x3F))
			.arg(instruction[2] & 0x07, 8, 16, QChar('0'));
		break;
	case CCPU::InstructionCode::STR:
		result += QString(" %1, a%2")
			.arg(QString("%1%2").arg(instruction[1] & 0x80 ? 'a' : 'r').arg(instruction[1] & 0x80 ? instruction[1] & 0x07 : instruction[1] & 0x3F))
			.arg(instruction[2] & 0x07, 8, 16, QChar('0'));
		break;
	case CCPU::InstructionCode::MOV:
	case CCPU::InstructionCode::SWP:
	case CCPU::InstructionCode::CMP:
		result += QString(" %1, %2")
			.arg(QString("%1%2").arg(instruction[1] & 0x80 ? 'a' : 'r').arg(instruction[1] & 0x80 ? instruction[1] & 0x07 : instruction[1] & 0x3F))
			.arg(QString("%1%2").arg(instruction[2] & 0x80 ? 'a' : 'r').arg(instruction[2] & 0x80 ? instruction[2] & 0x07 : instruction[2] & 0x3F));
		break;
	case CCPU::InstructionCode::INC:
	case CCPU::InstructionCode::DEC:
		result += QString(" %1")
			.arg(QString("%1%2").arg(instruction[1] & 0x80 ? 'a' : 'r').arg(instruction[1] & 0x80 ? instruction[1] & 0x07 : instruction[1] & 0x3F));
		break;
	case CCPU::InstructionCode::IN:
	case CCPU::InstructionCode::OUT:
		result += QString(" r%1, %2h")
			.arg(instruction[1] & 0x3F)
			.arg(instruction[2], 4, 16, QChar('0'));
		break;
	}

	return result;
}

void SetRelocationTable(QMap<quint32, QString> const& map)
{
	s_mapRelocaionTable = map;
}

extern "C" DEBUGGER_EXPORT void Disassemble(QString const& file, QString& result)
{
	QFile input(file);
	input.open(QIODevice::ReadOnly);
	if (!input.isOpen())
	{
		result = "";
		return;
	}

	QByteArray bytes = input.readAll();
	quint8* data = (quint8*)bytes.data();
	s_nSymbolTableAddress = (quint32)data[0];
	s_nDataSectionAddress = (quint32)data[4];
	s_nCodeSectionAddress = (quint32)data[8];
	s_nRelocationTableAddress = (quint32)data[12];

	QMap<quint32, QString> map;
	quint32 pos = s_nRelocationTableAddress;

	while (1)
	{
		QString name((char*)(&data[pos]));

		if (name == "")
			break;

		map[(quint32)data[pos + name.size() + 1]] = name;
		pos += name.size() + 5;
	}

	SetRelocationTable(map);
	result = ".code\n";

	int i = 1;
	for (quint32 pos = s_nCodeSectionAddress; pos < s_nRelocationTableAddress;)
	{
		if (s_mapRelocaionTable.contains(pos))
		{
			result += s_mapRelocaionTable[pos] + ":\n";
			++i;
		}
		QString instruction = Disassemble(&data[pos]);

		if (instruction != "nop" || !s_bSkipNop)
		{
			result += "    " + instruction + '\n';

			s_mapLineAddresses[i++] = pos;
		}
		pos += CCPU::s_mapInstructions[data[pos]].first;
	}
}

extern "C" DEBUGGER_EXPORT bool SymbolExist(quint32 address)
{
	return s_mapRelocaionTable.contains(address);
}

extern "C" DEBUGGER_EXPORT bool LineToAddress(quint32 line, quint32* address)
{
	if (!s_mapLineAddresses.contains(line))
		return false;

	if (address == nullptr)
		return false;

	*address = s_mapLineAddresses[line];
	return true;
}
