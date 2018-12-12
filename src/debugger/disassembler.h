#pragma once

#include "debugger_global.h"
#include <cpu>
#include <QMap>

static QString Disassemble(quint8 instruction[8]);
static void SetRelocationTable(QMap<quint32, QString> const& map);
extern "C" DEBUGGER_EXPORT void Disassemble(QString const& file, QString& result);
extern "C" DEBUGGER_EXPORT bool SymbolExist(quint32 address);
extern "C" DEBUGGER_EXPORT bool LineToAddress(quint32 line, quint32* address);

static QMap<quint32, QString> s_mapRelocaionTable;
static QMap<quint32, quint32> s_mapLineAddresses;
static quint32 s_nSymbolTableAddress;
static quint32 s_nDataSectionAddress;
static quint32 s_nCodeSectionAddress;
static quint32 s_nRelocationTableAddress;
static bool s_bSkipNop = true;
