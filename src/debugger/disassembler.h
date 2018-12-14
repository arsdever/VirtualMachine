#pragma once

#include "debugger_global.h"
#include <cpu>
#include <QMap>
#include <QPair>

class DEBUGGER_EXPORT IDisassembler
{
protected:
	typedef QMap<quint32, QPair<QString, quint8>> VariableTable;
	typedef QMap<quint32, QString> FunctionTable;

private:
	friend class IDisassemblerDecorations;

public:
	virtual ~IDisassembler() = 0;

	virtual void Process() = 0;
	virtual QString GetPath() const = 0;
	virtual QByteArray GetData() const = 0;
	virtual QStringList GetResult() const = 0;

	virtual VariableTable const& GetVRelocationTable() const = 0;
	virtual FunctionTable const& GetFRelocationTable() const = 0;
	virtual void SetVRelocationTable(VariableTable const& map) = 0;
	virtual void SetFRelocationTable(FunctionTable const& map) = 0;

	virtual quint32 GetCodeSectionAddress() const = 0;
	virtual quint32 GetDataSectionAddress() const = 0;
	virtual quint32 GetRelocationTableAddress() const = 0;

protected:
	virtual QString& GetPath() = 0;
	virtual QByteArray& GetData() = 0;
	virtual QStringList& GetResult() = 0;

	virtual void SetPath(QString const& path) = 0;
	virtual void SetResult(QStringList const& result) = 0;
};

class DEBUGGER_EXPORT CDisassembler : public IDisassembler
{
public:
	CDisassembler(QString const& path);
	virtual ~CDisassembler() = default;

public:
	void ConstructRelocationTable(QByteArray const& file);
	QString DisassembleInstruction(const quint8* instruction);
	virtual void Process() override;
	void Disassemble(QByteArray const& code);
	//bool HasSymbol(quint32 address);
	//QString SymbolAt(quint32 address);
	virtual QString GetPath() const override { return m_strPath; }
	virtual QByteArray GetData() const override { return m_baData; }
	virtual QStringList GetResult() const override { return m_slResult; }

	virtual VariableTable const& GetVRelocationTable() const override { return m_mapDataRelocationTable; }
	virtual FunctionTable const& GetFRelocationTable() const override { return m_mapRelocaionTable; }
	virtual void SetVRelocationTable(VariableTable const& map) override { m_mapDataRelocationTable = map; }
	virtual void SetFRelocationTable(FunctionTable const& map) override { m_mapRelocaionTable = map; }

	virtual quint32 GetCodeSectionAddress() const override { return m_nCodeSectionAddress; }
	virtual quint32 GetDataSectionAddress() const override { return m_nDataSectionAddress; }
	virtual quint32 GetRelocationTableAddress() const override { return m_nRelocationTableAddress; }

protected:
	virtual QString& GetPath() override { return m_strPath; }
	virtual QByteArray& GetData() override { return m_baData; }
	virtual QStringList& GetResult() override { return m_slResult; }
	//void Colorize(QString& instructionLine);
	//void Relocate(QString& instruction);

	QMap<quint32, quint32> m_mapLineAddresses;
	quint32 m_nSymbolTableAddress;
	quint32 m_nDataSectionAddress;
	quint32 m_nCodeSectionAddress;
	quint32 m_nRelocationTableAddress;
	bool m_bSkipNop;
	virtual void SetPath(QString const& path) override { m_strPath = path; }
	virtual void SetResult(QStringList const& result) override { m_slResult = result; }

protected:
	QString m_strPath;
	QByteArray m_baData;
	QStringList m_slResult;
	VariableTable m_mapDataRelocationTable;
	FunctionTable m_mapRelocaionTable;
};