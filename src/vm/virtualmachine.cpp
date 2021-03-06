#include "virtualmachine.h"
#include <cpu>
#include <ram>

#include <QFile>
#include <QMessageBox>

CVirtualMachine::CVirtualMachine(quint32 nRamSize, quint32 nCoreCount, QObject* pParent)
	: QObject(pParent)
	, m_pRAM(new CRAM(nRamSize))
{
	while (nCoreCount > 0)
	{
		DeployCPU();
		--nCoreCount;
	}
}

CVirtualMachine::~CVirtualMachine()
{
}

void CVirtualMachine::LoadProgram(QString const& path)
{
	QFile file(path);
	file.open(QIODevice::ReadOnly);
	if (file.isOpen())
	{
		if (file.read(4) != "vmef")
		{
			file.close();
			throw invalid_file();
		}

		file.seek(0);
		QByteArray content = file.readAll();
		int i = 0;
		for (; i < content.size(); ++i)
			RAM()->operator[]<quint8>(i) = content[i];
	}
	file.close();

	for (CCPU* cpu : m_setCPU)
		if (cpu != nullptr)
			cpu->Restart();

	emit NewProgramLoaded(path);
}

void CVirtualMachine::DeployCPU()
{
	CCPU* pCPU = new CCPU(m_pRAM);
	pCPU->Restart();
	m_setCPU.insert(pCPU);
}

CCPU* CVirtualMachine::GetCPU() const
{
	return m_setCPU.toList().back();
}

CCPU* CVirtualMachine::GetCPUByID(QString const& id) const
{
	for (CCPU* cpu : m_setCPU)
	{
		if (cpu != nullptr && cpu->GetUUID() == id)
			return cpu;
	}

	return nullptr;
}

QStringList CVirtualMachine::GetCPUIDList() const
{
	QStringList result;
	for (CCPU* cpu : m_setCPU)
		if (cpu != nullptr)
			result.push_back(cpu->GetUUID());
	return result;
}

CRAM* CVirtualMachine::RAM() const
{
	return m_pRAM;
}

void CVirtualMachine::SetRAM(CRAM* pRAM)
{
	m_pRAM = pRAM;
}

CDebugger* CVirtualMachine::Debugger() const
{
	return m_pDebugger;
}

void CVirtualMachine::SetDebugger(CDebugger* pDebugger)
{
	m_pDebugger = pDebugger;
}

void CVirtualMachine::Run()
{
	for (CCPU* pCPU : m_setCPU)
	{
		pCPU->Run();

		if (!pCPU->m_sState.RUN)
			QMessageBox::information(nullptr, pCPU->GetUUID(), "Program execution finished.");
	}
}