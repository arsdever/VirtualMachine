#include "debugger.h"
#include <ram>
#include <cpu>
#include <vm>
#include <assets>
#include "call_stack.h"

#include <QFile>
#include <QPushButton>
#include <QTextEdit>
#include <QLayout>
#include <QMessageBox>
#include <QStyle>
#include <assets>
#include <QDate>
#include <QMenuBar>
#include <QLineEdit>
#include <QLibrary>

CDebugger::CDebugger(CVirtualMachineWindow* pParent)
	: QObject(pParent)
	, m_pProcess(nullptr)
	, m_bRunning(true)
	, m_pMemory(new QTextEdit())
	, m_strCurrentInstruction("")
{
	m_lstActions.push_back(new QAction("Attach to process"));
	connect(m_lstActions.back(), SIGNAL(triggered()), this, SLOT(AttachToProcess()));
	m_lstActions.push_back(new QAction(QIcon(":/Resources/play_icon_16x16.png"), "Run"));
	m_lstActions.back()->setShortcut(QKeySequence("F5"));
	m_lstActions.back()->setDisabled(true);
	connect(m_lstActions.back(), SIGNAL(triggered()), this, SLOT(Run()));
	m_lstActions.push_back(new QAction(QIcon(":/Resources/step_icon_16x16.png"), "Step"));
	m_lstActions.back()->setShortcut(QKeySequence("F10"));
	connect(m_lstActions.back(), SIGNAL(triggered()), this, SLOT(Step()));
	m_lstActions.back()->setDisabled(true);
	m_lstActions.push_back(new QAction(QIcon(":/Resources/step_into_icon_16x16.png"), "Step into"));
	m_lstActions.back()->setShortcut(QKeySequence("F11"));
	connect(m_lstActions.back(), SIGNAL(triggered()), this, SLOT(StepInto()));
	m_lstActions.back()->setDisabled(true);
	m_lstActions.push_back(new QAction(QIcon(":/Resources/breakpoint_icon_16x16.png"), "Toggle breakpoint"));
	m_lstActions.back()->setShortcut(QKeySequence("F9"));
	connect(m_lstActions.back(), SIGNAL(triggered()), this, SLOT(SetBreakpoint()));
	m_lstActions.back()->setDisabled(true);
	m_lstActions.push_back(new QAction(QIcon(":/Resources/into_memory_icon_16x16.png"), "Set memory value"));
	connect(m_lstActions.back(), SIGNAL(triggered()), this, SLOT(SetMemory()));
	m_lstActions.back()->setDisabled(true);
	m_lstActions.push_back(new QAction(QIcon(":/Resources/memory_icon_16x16.png"), "Show memory"));
	m_lstActions.back()->setShortcut(QKeySequence("CTRL+M"));
	connect(m_lstActions.back(), SIGNAL(triggered()), this, SLOT(ShowMemory()));
	m_lstActions.back()->setDisabled(true);
	connect(pParent->GetVM(), SIGNAL(NewProgramLoaded(QString const&)), this, SLOT(OnProgramLoaded(QString const&)));
}

void CDebugger::UpdateInformation()
{/*
	typedef QString(*Disassembler)(QString const&);
	Disassembler disassemble = (Disassembler)QLibrary::resolve("debugger", "Disassemble");

	if (disassemble == nullptr)
		return;*/

	CallFunction<IDebugger>(IDebugger::SetRunningAddressFunctor(m_pProcess->GetState().PC));
	CallFunction<IVMInformation>(IVMInformation::UpdateGRegistersInformationFunctor(m_pProcess));
	CallFunction<IVMInformation>(IVMInformation::UpdateARegistersInformationFunctor(m_pProcess));
	CallFunction<IVMInformation>(IVMInformation::UpdateMemoryInformationFunctor(m_pProcess->Ram()));
	CallFunction<IVMInformation>(IVMInformation::UpdateCallStackFunctor(m_pProcess->GetState().SF, m_pProcess->Ram()));
	//m_strCurrentInstruction = disassemble(&m_pProcess->Ram()->operator[]<quint8>(m_pProcess->GetState().PC));

	// to be removed
	UpdateMemory();
}

void CDebugger::UpdateMemory()
{
	CRAM* ram = m_pProcess->Ram();
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
	CRAM* pRam = m_pProcess->Ram();
	pRam->operator[]<quint8>(pos) = m_mapBreakpoints[pos];
	UpdateInformation();
	m_bRunning = false;
}

void CDebugger::Run()
{
	if (!m_pProcess->GetState().RUN)
		m_pProcess->Restart();

	try
	{
		quint32 pos = m_pProcess->GetState().PC;
		if (m_mapBreakpoints.contains(pos))
		{
			m_pProcess->Step();
			SetBreakpoint(pos);
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
	//QMessageBox::information(this, "Success", "Execution finished.");
}

void CDebugger::StepInto(bool b)
{
	if (!m_pProcess->GetState().RUN)
		m_pProcess->Restart();

	try
	{
		quint32 pos = m_pProcess->GetState().PC;
		m_pProcess->Step();
		if (m_mapBreakpoints.contains(pos))
			SetBreakpoint(pos);
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
	if (!m_pProcess->GetState().RUN)
		m_pProcess->Restart();

	quint32 size = CCPU::s_mapInstructions[m_pProcess->Ram()->operator[]<quint8>(m_pProcess->m_sState.PC)].first;
	quint32 pos = m_pProcess->GetState().PC + size;
	m_bRunning = true;

	while (m_pProcess->GetState().PC != pos && m_pProcess->m_sState.RUN && m_bRunning)
		StepInto(false);

	UpdateInformation();
}

void CDebugger::SetBreakpoint()
{
	QDialog* pDialog = new QDialog();
	pDialog->setWindowTitle("Toggle breakpoint");
	QLineEdit* pLineNumber = new QLineEdit(pDialog);
	QPushButton* pSubmit = new QPushButton("Toggle", pDialog);
	pDialog->setLayout(new QHBoxLayout());
	pDialog->layout()->addWidget(pLineNumber);
	pDialog->layout()->addWidget(pSubmit);
	connect(pSubmit, SIGNAL(clicked()), pDialog, SLOT(close()));
	pDialog->exec();
	bool ok = false;
	quint32 nLineNumber = pLineNumber->text().toInt(&ok, 16);

	if (!ok)
		return;

	CallFunction(IDebugger::ToggleBreakpointFunctor(nLineNumber));
	QAction* pAction = static_cast<QAction*>(sender());
	delete pDialog;
}

void CDebugger::OnProgramLoaded(QString const& strNewPath)
{
	QString debuggerPath = strNewPath;
	debuggerPath.replace(QRegularExpression(".ef$"), QString(".dbg"));
	CVirtualMachine* pVM = dynamic_cast<CVirtualMachine*>(sender());
	if (pVM == nullptr)
		return;

	if (m_pProcess == nullptr || !pVM->GetCPUIDList().contains(m_pProcess->GetUUID()))
		return;

	LoadBreakpoints(debuggerPath);
	UpdateInformation();
}

void CDebugger::SetBreakpoint(quint32 address)
{
	if (m_pProcess == nullptr)
		throw process_not_attached();

	CRAM * pRam = m_pProcess->Ram();
	m_mapBreakpoints[address] = m_pProcess->Ram()->operator[]<quint8>(address);
	pRam->operator[]<quint8>(address) = CCPU::InstructionCode::BRK;
	UpdateMemory();
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
		m_pProcess->Ram()->operator[]<quint8>(address) = (quint8)data;
		break;
	case 1:
		m_pProcess->Ram()->operator[]<quint16>(address) = (quint16)data;
		break;
	case 2:
		m_pProcess->Ram()->operator[]<quint32>(address) = (quint32)data;
		break;
	case 3:
		m_pProcess->Ram()->operator[]<quint64>(address) = (quint64)data;
		break;
	}

	UpdateMemory();
}

void CDebugger::LoadBreakpoints(QString const& path)
{
	CallFunction<IDebugger>(IDebugger::ClearBreakpointsFunctor());

	QFile bps(path);
	bps.open(QIODevice::ReadOnly);

	if (!bps.isOpen())
		return;

	QByteArray data = bps.readAll();
	for (quint32 i = 0; i < data.size() / 4; ++i)
	{
		quint32 pos = *(quint32*)(data.data() + i * 4);
		CallFunction<IDebugger>(IDebugger::SetBreakpointFunctor(pos));
	}
}

QList<QAction*> CDebugger::GetActions() const
{
	return m_lstActions;
}

void CDebugger::AttachToProcess()
{
	CVirtualMachineWindow* parentWidget = dynamic_cast<CVirtualMachineWindow*>(parent());
	CVirtualMachine* vm = parentWidget->GetVM();
	CCPUListDialog dialog(vm);

	dialog.exec();

	if (dialog.GetSelection() != "")
		m_pProcess = vm->GetCPUByID(dialog.GetSelection());

	for (QAction* action : GetActions())
		action->setEnabled(true);

	UpdateInformation();
}

void CDebugger::Detach()
{
	m_pProcess = nullptr;
}

// IDebugger implementation --------------------------------------------------------------------------------------------

void CDebugger::XDebugger::SetBreakpoint(quint32 address)
{
	if (m_pThis->m_mapBreakpoints.contains(address))
		return;

	m_pThis->SetBreakpoint(address);
}

void CDebugger::XDebugger::UnsetBreakpoint(quint32 address)
{
	if (!m_pThis->m_mapBreakpoints.contains(address))
		return;

	m_pThis->m_pProcess->Ram()->operator[]<quint8>(address) = m_pThis->m_mapBreakpoints[address];
	m_pThis->m_mapBreakpoints.remove(address);
	m_pThis->UpdateMemory();
}

void CDebugger::XDebugger::ClearBreakpoints()
{
	m_pThis->m_mapBreakpoints = QMap<quint32, quint8>();
}

void CDebugger::XDebugger::ToggleBreakpoint(quint32 address)
{
	if (m_pThis->m_mapBreakpoints.contains(address))
		UnsetBreakpoint(address);
	else
		SetBreakpoint(address);
}

//----------------------------------------------------------------------------------------------------------------------

#include <QDockWidget>

extern "C" DEBUGGER_EXPORT bool LoadPlugin(QMainWindow* pMainWindow)
{
	CVirtualMachineWindow* pVMWindow = dynamic_cast<CVirtualMachineWindow*>(pMainWindow);
	if (pVMWindow == nullptr)
		return false;

	CDebugger* pDebugger = new CDebugger(pVMWindow);

	QDockWidget* pMemoryDock = new QDockWidget("Memory", pMainWindow);
	pMemoryDock->setWidget(pDebugger->GetMemoryWidget());
	pMemoryDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	pMainWindow->addDockWidget(Qt::BottomDockWidgetArea, pMemoryDock);

	CGRegisterWindow* pGPRWindow = new CGRegisterWindow();
	CARegisterWindow* pARWindow = new CARegisterWindow();
	CCallStackView* pCallStack = new CCallStackView();

	QDockWidget* pRegViewDock = new QDockWidget("Registers", pMainWindow);
	pRegViewDock->setWidget(pGPRWindow);
	pRegViewDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	pMainWindow->addDockWidget(Qt::LeftDockWidgetArea, pRegViewDock);

	QDockWidget* pARegViewDock = new QDockWidget("Address registers", pMainWindow);
	pARegViewDock->setWidget(pARWindow);
	pARegViewDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	pMainWindow->addDockWidget(Qt::RightDockWidgetArea, pARegViewDock);

	QDockWidget* pCallStackDock = new QDockWidget("CallStack", pMainWindow);
	pCallStackDock->setWidget(pCallStack);
	pCallStackDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	pMainWindow->addDockWidget(Qt::RightDockWidgetArea, pCallStackDock);

	QMenuBar* pMenuBar = pMainWindow->menuBar();
	if (pMenuBar == nullptr)
		return true;
	QMenu* pMenu = pMenuBar->addMenu("Debug");
	for (QAction* action : pDebugger->GetActions())
		pMenu->addAction(action);
	/*
	pMenu->addAction("Show/hide general porpuse registers"), pGPRWindow, SLOT(ToggleView());
	pMenu->addAction("Show/hide address registers"), pARWindow, SLOT(ToggleView());
	*/

	return true;
}