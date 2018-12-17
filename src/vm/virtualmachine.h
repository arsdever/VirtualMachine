#pragma once

#include "vm_global.h"

#include <exception>

#include <QObject>
#include <QSet>
#include <QVector>

class CCPU;
class CRAM;
class CDebugger;
class CIOController;
class IIODevice;

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

	CIOController* IOController() const;
	void SetIOController(CIOController* pIOController);

	CDebugger* Debugger() const;
	void SetDebugger(CDebugger* pDebugger);

	void SetDevice(IIODevice* pDevice, qint32 nPrefferedPort = -1);

signals:
	void NewProgramLoaded(QString const&);

public slots:
	void Run();

private:
	QSet<CCPU*> m_setCPU;
	CRAM* m_pRAM;
	CIOController* m_pIOController;
	CDebugger* m_pDebugger;
	QVector<IIODevice*> m_arrDevices;

public:
	struct vm_exception : public std::exception {};
	struct invalid_file : vm_exception {};
};

