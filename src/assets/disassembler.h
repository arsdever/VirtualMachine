#pragma once

#include "assets_global.h"
#include <cpu>

#include <QMap>

class ASSETS_EXPORT CDisassembler
{
public:
	CDisassembler();
	~CDisassembler();

	static QString Disassemble(quint8 instruction[8]);
	static void SetRelocationTable(QMap<quint32, QString> const& map);
	static QString Disassemble(QString const& file);

	static QMap<quint32, QString> s_mapRelocaionTable;
	static QMap<quint32, quint32> s_mapLineAddresses;
	static quint32 s_nSymbolTableAddress;
	static quint32 s_nDataSectionAddress;
	static quint32 s_nCodeSectionAddress;
	static quint32 s_nRelocationTableAddress;
	static bool s_bSkipNop;
};

