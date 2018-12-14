#include "disassembler_decor.h"
#include <exception>
#include <QRegularExpression>

IDisassemblerDecorations::IDisassemblerDecorations(IDisassembler* pDisassembler)
	: m_pCoreDisassembler(pDisassembler)
{
	if (m_pCoreDisassembler == nullptr)
		throw std::exception();
}

IDisassemblerDecorations::~IDisassemblerDecorations()
{
}

void IDisassemblerDecorations::Process()
{
	m_pCoreDisassembler->Process();
}

CSymbolizer::CSymbolizer(IDisassembler* pDisassembler)
	: IDisassemblerDecorations(pDisassembler)
{}

static QString GetOperationName(quint8 operation)
{
	switch (operation)
	{
	case CCPU::InstructionCode::CALL: return "call";
	case CCPU::InstructionCode::ASG: return "asg";
	case CCPU::InstructionCode::JMP: return "jmp";
	}

	return "";
}

void CSymbolizer::Process()
{
	IDisassemblerDecorations::Process();
	const char* data = GetData().data();
	int i = 1;

	quint32 pos = GetCodeSectionAddress();
	QStringList::iterator it = GetResult().begin() + 1;
	for (; pos < GetRelocationTableAddress();)
	{
		if (GetFRelocationTable().contains(pos))
		{
			GetResult().insert(it, GetFRelocationTable()[pos] + ":");
			++it;
			++i;
		}

		const quint8* instruction = (const quint8*)&data[pos];
		QString symbolizedLine = GetOperationName(instruction[0]);
		switch (instruction[0])
		{
		case CCPU::InstructionCode::JMPC:
		{
			switch (instruction[1])
			{
			case CCPU::EQUAL: symbolizedLine = "je"; break;
			case CCPU::NOT_EQUAL: symbolizedLine = "jne"; break;
			case CCPU::ABOVE: symbolizedLine = "ja"; break;
			case CCPU::ABOVE_OR_EQUAL: symbolizedLine = "jae"; break;
			case CCPU::BELOW: symbolizedLine = "jb"; break;
			case CCPU::BELOW_OR_EQUAL: symbolizedLine = "jbe"; break;
			case CCPU::GREATER: symbolizedLine = "jg"; break;
			case CCPU::GREATER_OR_EQUAL: symbolizedLine = "jge"; break;
			case CCPU::LESS: symbolizedLine = "jl"; break;
			case CCPU::LESS_OR_EQUAL: symbolizedLine = "jle"; break;
			case CCPU::OVERFLOW_SET: symbolizedLine = "jo"; break;
			case CCPU::NOT_OVERFLOW: symbolizedLine = "jno"; break;
			case CCPU::SIGN_SET: symbolizedLine = "js"; break;
			case CCPU::NOT_SIGN: symbolizedLine = "jns"; break;
			case CCPU::PARITY_SET: symbolizedLine = "jp"; break;
			case CCPU::NOT_PARITY: symbolizedLine = "jnp"; break;
			}

			quint32 address = *(quint32*)(instruction + 2);
			if (HasSymbol(address))
				symbolizedLine += QString(" [%1]").arg(SymbolAt(address));

			break;
		}
		case CCPU::InstructionCode::CALL:
		case CCPU::InstructionCode::JMP:
		{
			quint32 address = *(quint32*)(instruction + 1);
			if (HasSymbol(address))
				symbolizedLine += QString(" [%1]").arg(SymbolAt(address));

			break;
		}
		case CCPU::InstructionCode::ASG:
		{
			quint32 address = *(quint32*)(instruction + 2);
			if (instruction[1] & 0x80 && HasSymbol(address))
			{
				symbolizedLine += QString(" a%1, [%2]")
					.arg(instruction[1] & 0x07)
					.arg(SymbolAt(address));
				break;
			}
			else
			{
				symbolizedLine = "";
			}
		}
		}

		if (symbolizedLine != "")
		{
			*it = "    " + symbolizedLine;
		}

		//m_mapLineAddresses[i++] = pos;
		if(data[pos] != CCPU::InstructionCode::NOP)
			++it;

		pos += CCPU::s_mapInstructions[data[pos]].first;
	}
}

bool CSymbolizer::HasSymbol(quint32 address) const
{
	return GetFRelocationTable().contains(address) || GetVRelocationTable().contains(address);
}

QString CSymbolizer::SymbolAt(quint32 address) const
{
	if(GetFRelocationTable().contains(address))
		return GetFRelocationTable()[address];
	else if(GetVRelocationTable().contains(address))
		return GetVRelocationTable()[address].first;
	return "";
}

CColorizer::CColorizer(IDisassembler* pDisassembler)
	: IDisassemblerDecorations(pDisassembler)
{}

void CColorizer::Process()
{
	IDisassemblerDecorations::Process();
	QString const sectionColor = "fff";
	QString const labelColor = "f66";
	QString const instructionColor = "6f6";

	const char* data = GetData().data();
	int i = 1;

	quint32 pos = GetCodeSectionAddress();
	QStringList::iterator it = GetResult().begin();
	for (; it != GetResult().end() && pos < GetRelocationTableAddress();)
	{
		if (it->operator[](0) == '.')
		{
			*it = QString("<font style=\"font-weight:%2;color:#%1\">%3</font>").arg(sectionColor).arg("bold").arg(*it);
			++it;
			continue;
		}

		if (GetFRelocationTable().contains(pos))
		{
			*it = QString("<font style=\"font-weight:%2;color:#%1\">%3</font>").arg(labelColor).arg("bold").arg(*it);
			++it;
			++i;
		}

		if (data[pos] != CCPU::InstructionCode::NOP)
		{
			QString instruction = *it;
			instruction.replace(QRegularExpression("^[ ]*"), "");
			qint32 pos1 = instruction.indexOf(' ');
			instruction.insert(pos1 < 0 ? instruction.size() : pos1, "</font>");
			instruction.push_front(QString("    <font style=\"font-weight:%2;color:#%1\">").arg(instructionColor).arg("normal"));
			*it = instruction;
			++it;
		}

		pos += CCPU::s_mapInstructions[data[pos]].first;
	}
}