#pragma once

#include "vm_global.h"

#include <exception>

#include <QObject>
#include <QSet>

class CCPU;
class CRAM;
class CDebugger;

class VM_EXPORT CVirtualMachine : public QObject
{

	Q_OBJECT

public:
	CVirtualMachine(quint32 nRamSize = 0xffff, quint32 nCoreCount = 1, QObject* pParent = nullptr);
	~CVirtualMachine();

	void DeployCPU();
	CCPU* GetCPU() const;
	CCPU* GetCPUByID(QString const& id) const;
	QStringList GetCPUIDList() const;
	void LoadProgram(QString const& path);

	CRAM* RAM() const;
	void SetRAM(CRAM* pRAM);

	CDebugger* Debugger() const;
	void SetDebugger(CDebugger* pDebugger);

signals:
	void NewProgramLoaded(QString const&);

public slots:
	void Run();

private:
	QSet<CCPU*> m_setCPU;
	CRAM* m_pRAM;
	CDebugger* m_pDebugger;

public:
	struct vm_exception : public std::exception {};
	struct invalid_file : vm_exception {};
};

