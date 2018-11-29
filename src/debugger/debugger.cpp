﻿#include "debugger.h"
#include <ram>
#include <cpu>
#include <assets>

#include <QFile>
#include <QPushButton>
#include <QTextEdit>
#include <QLayout>
#include <QMessageBox>
#include <QStyle>
#include <assets>
#include <QDate>

CDebugger::CDebugger()
	: m_pProcess(nullptr)
	, m_bRunning(true)
	, m_pRegView(new CRegisterWindow())
	, m_pARegView(new CARegisterWindow())
	, m_pMemory(new QTextEdit())
{
	QPushButton* continueButton = new QPushButton("Continue");
	QPushButton* stepButton = new QPushButton("Step");
	QPushButton* setBreakpoint = new QPushButton("Set breakpoint");
	QPushButton* setMemoryValue = new QPushButton("Set memory value");
	QPushButton* showMemory = new QPushButton("Show memory");
	QPushButton* gprButton = new QPushButton("Show/hide general porpuse registers");
	QPushButton* arButton = new QPushButton("Show/hide address registers");

	setLayout(new QVBoxLayout());
	layout()->addWidget(continueButton);
	layout()->addWidget(stepButton);
	layout()->addWidget(setBreakpoint);
	layout()->addWidget(setMemoryValue);
	layout()->addWidget(showMemory);
	layout()->addWidget(gprButton);
	layout()->addWidget(arButton);
	connect(continueButton, SIGNAL(clicked()), this, SLOT(Run()));
	connect(stepButton, SIGNAL(clicked()), this, SLOT(Step()));
	connect(setBreakpoint, SIGNAL(clicked()), this, SLOT(SetBreakpoint()));
	connect(setMemoryValue, SIGNAL(clicked()), this, SLOT(SetMemory()));
	connect(showMemory, SIGNAL(clicked()), this, SLOT(ShowMemory()));
	connect(gprButton, SIGNAL(clicked()), this, SLOT(ToggleRegisters()));
	connect(arButton, SIGNAL(clicked()), this, SLOT(ToggleARegisters()));
}

void CDebugger::UpdateInformation()
{
	m_pRegView->UpdateValues(m_pProcess);
	m_pARegView->UpdateValues(m_pProcess);
	UpdateMemory();
}

void CDebugger::UpdateMemory()
{
	CRAM* ram = CRAM::instance();
	QString text = "";
	text.append(QString(13, ' '));
	for (int i = 0; i < 16; ++i)
	{
		text.append(QString("%1 ").arg(i, 2, 16, QChar('0')));
	}
	text.append('\n');
	text.append('\n');
	for (int i = 0; i < ram->GetSize(); ++i)
	{
		if (i % 16 == 0)
			text.append(QString("0x%1   ").arg(i, 8, 16, QChar('0')));
		text.append(QString("%1 ").arg(ram->operator[]<quint8>(i), 2, 16, QChar('0')));
		if ((i + 1) % 16 == 0)
			text.append('\n');
	}
	m_pMemory->setText(text);
	m_pMemory->setWordWrapMode(QTextOption::NoWrap);
}

void CDebugger::HandleBreakpoint()
{
	quint32 pos = --m_pProcess->m_sState.PC;
	CRAM* pRam = CRAM::instance();
	pRam->operator[]<quint8>(pos) = m_mapBreakpoints[pos];
	UpdateInformation();
	m_bRunning = false;
}

void CDebugger::Run()
{
	quint32 pos = m_pProcess->m_sState.PC;

	try
	{
		if (m_mapBreakpoints.contains(pos))
		{
			quint32 size = CCPU::s_mapInstructions[m_mapBreakpoints[pos]].first;
			Step();
			SetBreakpoint(m_pProcess->m_sState.PC - size);
		}

		m_pProcess->Run();
	}
	catch (const CCPU::breakpoint_exception&)
	{
		HandleBreakpoint();
		return;
	}
	catch (const std::exception&)
	{

	}

	UpdateInformation();
	QMessageBox::information(this, "Success", "Execution finished.");
}

void CDebugger::ShowMemory()
{
	int m_nMemoryShowSize = 16;
	QDialog* memory = new QDialog(this);
	memory->setLayout(new QVBoxLayout());
	memory->layout()->addWidget(m_pMemory);
	memory->setWindowTitle("Memory");
	memory->show();
	memory->setFixedWidth((m_nMemoryShowSize * 3 + 12) * QFontMetrics(qApp->font()).width('9') + 60);
}

void CDebugger::Step()
{
	quint32 size = CCPU::s_mapInstructions[m_pProcess->m_sState.PC].first;
	quint32 pos = m_pProcess->GetState().PC + size;
	SetBreakpoint(pos);

	try
	{
		m_pProcess->Run();
	}
	catch (const CCPU::breakpoint_exception&)
	{
		HandleBreakpoint();
		return;
	}
	catch (const std::exception&)
	{

	}

	UpdateInformation();
	QMessageBox::information(this, "Success", "Execution finished.");
}

void CDebugger::SetBreakpoint()
{
	QDialog* pDialog = new QDialog(this);
	pDialog->setWindowTitle("Set breakpoint");
	QLineEdit* pLineNumber = new QLineEdit(pDialog);
	QPushButton* pSubmit = new QPushButton("Set", pDialog);
	pDialog->setLayout(new QHBoxLayout());
	pDialog->layout()->addWidget(pLineNumber);
	pDialog->layout()->addWidget(pSubmit);
	connect(pSubmit, SIGNAL(clicked()), pDialog, SLOT(close()));
	pDialog->exec();
	bool ok = false;
	quint32 nLineNumber = pLineNumber->text().toInt(&ok, 16);

	if (!ok)
		return;

	SetBreakpoint(nLineNumber);
	delete pDialog;
}

void CDebugger::SetMemory()
{
	quint32 address;
	quint64 data;
	switch(CMemoryChangeDialog::GetDataToChange(address, data))
	{
	case -1:
		return;
	case 0:
		CRAM::instance()->operator[]<quint8>(address) = (quint8)data;
		break;
	case 1:
		CRAM::instance()->operator[]<quint16>(address) = (quint16)data;
		break;
	case 2:
		CRAM::instance()->operator[]<quint32>(address) = (quint32)data;
		break;
	case 3:
		CRAM::instance()->operator[]<quint64>(address) = (quint64)data;
		break;
	}

	UpdateMemory();
}

void CDebugger::SetBreakpoint(quint32 nPosition)
{
	CRAM* pRam = CRAM::instance();
	m_mapBreakpoints[nPosition] = CRAM::instance()->operator[]<quint8>(nPosition);
	pRam->operator[]<quint8>(nPosition) = 0x02;

	QString bpList = "";
	QList<quint32> lst = m_mapBreakpoints.keys();
	for (QList<quint32>::iterator it = lst.begin(); it != lst.end(); ++it)
	{
		if (it != lst.begin())
			bpList.append('\n');

		bpList.append(QString("0x%1").arg(*it, 8, 16, QChar('0')));
	}

	findChildren<QPushButton*>().at(2)->setToolTip(bpList);
}

void CDebugger::LoadBreakpoints(QString const& path)
{
	QFile bps(path);
	bps.open(QIODevice::ReadOnly);

	if (!bps.isOpen())
		return;

	QByteArray data = bps.readAll();
	for (quint32 i = 0; i < data.size() / 4; ++i)
	{
		quint32 pos = (quint32)data[i * 4];
		SetBreakpoint(pos);
	}
}

void CDebugger::AttachToProcess(CCPU* pCPU)
{
	m_pProcess = pCPU;
	UpdateInformation();
}

void CDebugger::Detach()
{
	m_pProcess = nullptr;
}

void CDebugger::ToggleRegisters()
{
	if(m_pRegView->isHidden())
		m_pRegView->show();
	else
		m_pRegView->hide();
}

void CDebugger::ToggleARegisters()
{
	if (m_pARegView->isHidden())
		m_pARegView->show();
	else
		m_pARegView->hide();
}