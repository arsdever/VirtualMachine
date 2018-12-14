#pragma once

#include "debugger_global.h"
#include "disassembler.h"

class DEBUGGER_EXPORT IDisassemblerDecorations : public IDisassembler
{
public:
	IDisassemblerDecorations(IDisassembler* pDisassembler);
	virtual ~IDisassemblerDecorations() override;

	virtual void Process() override;

	virtual QString GetPath() const override { return m_pCoreDisassembler->GetPath(); }
	virtual QByteArray GetData() const override { return m_pCoreDisassembler->GetData(); }
	virtual QStringList GetResult() const override { return m_pCoreDisassembler->GetResult(); }

	virtual VariableTable const& GetVRelocationTable() const override { return m_pCoreDisassembler->GetVRelocationTable(); }
	virtual FunctionTable const& GetFRelocationTable() const override { return m_pCoreDisassembler->GetFRelocationTable(); }
	virtual void SetVRelocationTable(VariableTable const& map) override { m_pCoreDisassembler->SetVRelocationTable(map); }
	virtual void SetFRelocationTable(FunctionTable const& map) override { m_pCoreDisassembler->SetFRelocationTable(map); }

	virtual void SetPath(QString const& path) { m_pCoreDisassembler->SetPath(path); }
	virtual void SetResult(QStringList const& result) { m_pCoreDisassembler->SetResult(result); }

	virtual quint32 GetCodeSectionAddress() const override { return m_pCoreDisassembler->GetCodeSectionAddress(); }
	virtual quint32 GetDataSectionAddress() const override { return m_pCoreDisassembler->GetDataSectionAddress(); }
	virtual quint32 GetRelocationTableAddress() const override { return m_pCoreDisassembler->GetRelocationTableAddress(); }

protected:
	virtual QString& GetPath() override { return m_pCoreDisassembler->GetPath(); }
	virtual QByteArray& GetData() override { return m_pCoreDisassembler->GetData(); }
	virtual QStringList& GetResult() override { return m_pCoreDisassembler->GetResult(); }

private:
	IDisassembler* m_pCoreDisassembler;
};

class DEBUGGER_EXPORT CSymbolizer : public IDisassemblerDecorations
{
public:
	CSymbolizer(IDisassembler* pDisassembler);
	virtual ~CSymbolizer() override = default;

	virtual void Process() override;

protected:
	bool HasSymbol(quint32 address) const;
	QString SymbolAt(quint32 address) const;
};

class DEBUGGER_EXPORT CColorizer : public IDisassemblerDecorations
{
public:
	CColorizer(IDisassembler* pDisassembler);
	virtual ~CColorizer() override = default;

	virtual void Process() override;

protected:/*
	bool HasSymbol(quint32 address) const;
	QString SymbolAt(quint32 address) const;*/
};