#include "disassembler.h"
#include <QFile>
#include <QRegularExpression>

IDisassembler::~IDisassembler() {}

CDisassembler::CDisassembler(QString const& path)
	: IDisassembler()
	, m_strPath(path)
	, m_slResult()
	, m_bSkipNop(true)
{}

void CDisassembler::Process()
{
	QFile input(GetPath());
	input.open(QIODevice::ReadOnly);

	if (!input.isOpen())
		return;
	
	m_baData = input.readAll();
	ConstructRelocationTable(m_baData);
	Disassemble(m_baData);
}

static QString GetOperationName(quint8 operation)
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
	case CCPU::InstructionCode::ASG: return "asg";
	case CCPU::InstructionCode::LOD: return "load";
	case CCPU::InstructionCode::STR: return "str";
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

	return "???";
}

QString CDisassembler::DisassembleInstruction(const quint8* instruction)
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

		result += QString(" %1h").arg(address, 8, 16, QChar('0'));
		break;
	}
	case CCPU::InstructionCode::CALL:
	case CCPU::InstructionCode::JMP:
	{
		quint32 address = *(quint32*)(instruction + 1);
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
			.arg(instruction[2] & 0x07);
		break;
	case CCPU::InstructionCode::STR:
		result += QString(" %1, a%2")
			.arg(QString("%1%2").arg(instruction[1] & 0x80 ? 'a' : 'r').arg(instruction[1] & 0x80 ? instruction[1] & 0x07 : instruction[1] & 0x3F))
			.arg(instruction[2] & 0x07);
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
	}

	return result;
}

//void CDisassembler::Relocate(QString& instruction)
//{
//	QString ins = instruction.mid(0, instruction.indexOf(' '));
//	if (ins == "asg");
//}

//void CDisassembler::Colorize(QString& instructionLine)
//{
//	QString instruction = instructionLine.mid(0, instructionLine.indexOf(' '));
//	instructionLine.insert(instruction.size(), "</font>");
//	instructionLine.insert(0, "<font style=\"font-weight:bold;color:#6f6\">");
//	for (QString label : m_mapRelocaionTable)
//	{
//		int index = instructionLine.indexOf(label);
//		if(index > -1)
//			instructionLine.insert(index + label.size(), "</font>");
//			instructionLine.insert(index, "<font style=\"font-weight:bold;color:#f66\">");
//	}
//}

void CDisassembler::ConstructRelocationTable(QByteArray const& bytes)
{
	quint8* data = (quint8*)bytes.data();
	m_nSymbolTableAddress = (quint32)data[0];
	m_nDataSectionAddress = (quint32)data[4];
	m_nCodeSectionAddress = (quint32)data[8];
	m_nRelocationTableAddress = (quint32)data[12];

	VariableTable dmap;
	FunctionTable fmap;
	quint32 pos = m_nRelocationTableAddress;

	while (1)
	{
		QString name((char*)(&data[pos]));

		if (name == "")
			break;

		if (name[0] == ':')
		{
			dmap[(quint32)data[pos + name.size() + 2]] = QPair<QString, quint32>(name.mid(1), (quint8)data[pos + name.size() + 1]);
			pos += name.size() + 6;
		}
		else
		{
			fmap[(quint32)data[pos + name.size() + 1]] = name;
			pos += name.size() + 5;
		}
	}

	SetFRelocationTable(fmap);
	SetVRelocationTable(dmap);
}

void CDisassembler::Disassemble(QByteArray const& code)
{
	m_slResult.append(".code");
	const char* data = code.data();

	int i = 1;
	for (quint32 pos = m_nCodeSectionAddress; pos < m_nRelocationTableAddress;)
	{
		QString instruction = DisassembleInstruction((const quint8*)&data[pos]);

		if (instruction != "nop" || !m_bSkipNop)
		{
			m_slResult.append("    " + instruction);
			m_mapLineAddresses[i++] = pos;
		}
		pos += CCPU::s_mapInstructions[data[pos]].first;
	}
}