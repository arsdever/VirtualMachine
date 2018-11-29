#include "translator.h"

#include <QFile>
#include <QDataStream>
#include <QMap>

QMap<QString, quint8> InstructionMap = {
	{ "NOP", 1 },
	{ "RET", 2 },
	{ "BRK", 3 },
	{ "INT", 4 },
	{ "ADD", 5 },
	{ "SUB", 6 },
	{ "MUL", 7 },
	{ "DIV", 8 },
	{ "MOV", 9 },
	{ "ASG", 10 },
	{ "LOD", 11 },
	{ "STR", 12 },
	{ "JMP", 13 },
	{ "SWP", 14 },
	{ "AND", 15 },
	{ "OR", 16 },
	{ "XOR", 17 },
	{ "NOT", 18 },
	{ "NAND", 19 },
	{ "NOR", 20 },
	{ "INC", 21 },
	{ "DEC", 22 },
	{ "HLT", 23 },
	{ "ADDS", 24 },
	{ "SUBS", 25 },
	{ "MULS", 26 },
	{ "DIVS", 27 },
	{ "PUSH", 28 },
	{ "POP", 29 },
	{ "PUSHF", 30 },
	{ "POPF", 31 },
	{ "CALL", 32}
};

bool isInMap(QString item, QMap<QString, quint8> map)
{
	return map.find(item) != map.end();
}

CAssembler::CAssembler(QString const& path)
	: m_strPath(path)
{}

void CAssembler::Output(QString path)
{
	if (path == "")
		path = m_strPath;

	QFile output(path);
	output.open(QIODevice::WriteOnly);
	if (!output.isOpen())
		return;

	output.write(m_strDataOutput.toLatin1());
}


void CAssembler::Assemble()
{
	QFile input(m_strPath);
	input.open(QIODevice::ReadOnly);
	if (!input.isOpen())
		return;

	QString data = input.readAll();
	QStringList tokens = data.split(' ');
	input.close();

	for (QString token : tokens)
		if (isInMap(token, InstructionMap))
			m_strDataOutput.append(InstructionMap[token]);
		else
		{

		}
	}
}