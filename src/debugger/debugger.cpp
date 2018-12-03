#include "debugger.h"
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
#include <QMenuBar>

CDebugger::CDebugger()
	: m_pProcess(nullptr)
	, m_bRunning(true)
	, m_pRegView(new CRegisterWindow())
	, m_pARegView(new CARegisterWindow())
	, m_pMemory(new QTextEdit())
{
	setLayout(new QVBoxLayout());
	connect(m_pRegView, SIGNAL(ChangeRegister(quint8, quint32)), this, SLOT(SetRegisterValue(quint8, quint32)));
}

void CDebugger::UpdateInformation()
{
	m_pRegView->UpdateValues(m_pProcess);
	m_pARegView->UpdateValues(m_pProcess);
	m_strCurrentInstruction = CDisassembler::Disassemble(&CRAM::instance()->operator[]<quint8>(m_pProcess->GetState().PC));
	UpdateMemory();
	emit Update();
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

void CDebugger::SetRegisterValue(quint8 regNum, quint32 regValue)
{
	(quint32&)m_pProcess->m_sState.GR[regNum] = regValue;
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

void CDebugger::StepInto(bool b)
{
	try
	{
		m_pProcess->Step();
	}
	catch (const CCPU::breakpoint_exception&)
	{
		HandleBreakpoint();
		return;
	}
	catch (const std::exception&)
	{

	}

	if(b)
		UpdateInformation();
}

void CDebugger::Step()
{
	quint32 size = CCPU::s_mapInstructions[CRAM::instance()->operator[]<quint8>(m_pProcess->m_sState.PC)].first;
	quint32 pos = m_pProcess->GetState().PC + size;

	while (m_pProcess->GetState().PC != pos)
		StepInto(false);

	UpdateInformation();
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
	pRam->operator[]<quint8>(nPosition) = CCPU::InstructionCode::BRK;

	QString bpList = "";
	QList<quint32> lst = m_mapBreakpoints.keys();
	for (QList<quint32>::iterator it = lst.begin(); it != lst.end(); ++it)
	{
		if (it != lst.begin())
			bpList.append('\n');

		bpList.append(QString("0x%1").arg(*it, 8, 16, QChar('0')));
	}

	findChildren<QPushButton*>().at(2)->setToolTip(bpList);
	UpdateMemory();
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
		quint32 pos = *(quint32*)(data.data() + i * 4);
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

void CDebugger::PopulateMenuBar(QMenuBar* pMenuBar)
{
	QMenu* pMenu = pMenuBar->addMenu("Debug");
	pMenu->addAction(QIcon(":/Resources/play_icon_16x16.png"), "Run", this, SLOT(Run()), QKeySequence("F5"));
	pMenu->addAction(QIcon(":/Resources/step_icon_16x16.png"), "Step", this, SLOT(Step()), QKeySequence("F10"));
	pMenu->addAction(QIcon(":/Resources/step_into_icon_16x16.png"), "Step into", this, SLOT(StepInto()), QKeySequence("F11"));
	pMenu->addAction(QIcon(":/Resources/breakpoint_icon_16x16.png"), "Set breakpoint", this, SLOT(SetBreakpoint()), QKeySequence("F9"));
	pMenu->addAction(QIcon(":/Resources/into_memory_icon_16x16.png"), "Set memory value", this, SLOT(SetMemory()));
	pMenu->addAction(QIcon(":/Resources/memory_icon_16x16.png"), "Show memory", this, SLOT(ShowMemory()), QKeySequence("CTRL+M"));
	pMenu->addAction("Show/hide general porpuse registers"), this, SLOT(ToggleRegisters());
	pMenu->addAction("Show/hide address registers"), this, SLOT(ToggleARegisters());
}