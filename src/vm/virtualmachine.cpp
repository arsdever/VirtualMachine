#include "virtualmachine.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <assets>
#include <ram>
#include <QDockWidget>
#include <QMessageBox>
#include <cpu>
#include <QFile>
#include <QStatusBar>
#include <QPlainTextEdit>
#include <QMenuBar>
#include <QFileDialog>

VirtualMachine::VirtualMachine(QWidget *parent)
	: QMainWindow(parent)
	, m_cCPU()
	, m_cDebugger()
	, m_pEditor(new CCodeEditor())
{
	connect(&m_cDebugger, SIGNAL(Update()), this, SLOT(UpdateStatusBar()));
	setStatusBar(new QStatusBar());

	QDockWidget* pMemoryDock = new QDockWidget("Memory", this);
	pMemoryDock->setWidget(m_cDebugger.GetMemoryWidget());
	pMemoryDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	addDockWidget(Qt::BottomDockWidgetArea, pMemoryDock);

	QDockWidget* pRegViewDock = new QDockWidget("Registers", this);
	pRegViewDock->setWidget(m_cDebugger.GetRegisterAreaWidget());
	pRegViewDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	addDockWidget(Qt::RightDockWidgetArea, pRegViewDock);

	QDockWidget* pARegViewDock = new QDockWidget("Address registers", this);
	pARegViewDock->setWidget(m_cDebugger.GetARegisterAreaWidget());
	pARegViewDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	addDockWidget(Qt::RightDockWidgetArea, pARegViewDock);

	QDockWidget* pCallStackDock = new QDockWidget("CallStack", this);
	pCallStackDock->setWidget(m_cDebugger.GetCallStackWidget());
	pCallStackDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	addDockWidget(Qt::RightDockWidgetArea, pCallStackDock);

	/*QWidget* pCentralWidget = new QWidget();
	pCentralWidget->setLayout(new QVBoxLayout());
	pCentralWidget->layout()->addWidget(m_pEditor);
	CEditorNumberArea* pNumberArea = new CEditorNumberArea();
	pNumberArea->SetDrawerWidget((CCodeEditor*)m_pEditor);
	pCentralWidget->layout()->addWidget(pNumberArea);*/
	m_pEditor->setReadOnly(true);
	setCentralWidget(m_pEditor);

	InitMenuBar();
}

void VirtualMachine::UpdateStatusBar()
{
	statusBar()->showMessage(m_cDebugger.GetCurrentInstruction());
}

void VirtualMachine::LoadProgram(QString const& path)
{
	QFile file(path);
	file.open(QIODevice::ReadOnly);
	if (file.isOpen())
	{
		QByteArray content = file.readAll();
		int i = 0;
		for (; i < content.size(); ++i)
			CRAM::instance()->operator[]<quint8>(i) = content[i];
	}
	file.close();

	m_cCPU.m_sState.SF = CRAM::instance()->GetSize();
	m_cCPU.m_sState.SP = CRAM::instance()->GetSize();
	m_cCPU.m_sState.PC = CRAM::instance()->operator[]<quint32>(8);
	m_pEditor->setPlainText(CDisassembler::Disassemble(path));
	m_cDebugger.AttachToProcess(&m_cCPU);
	QString debuggerPath = path;
	debuggerPath.replace(QRegularExpression(".ef$"), QString(".dbg"));
	m_cDebugger.LoadBreakpoints(debuggerPath);
}

void VirtualMachine::InitMenuBar()
{
	QMenuBar* pMenuBar = new QMenuBar();
	QMenu* fileMenu = pMenuBar->addMenu("File");
	fileMenu->addAction(QIcon(":/Resources/folder_16x16.png"), "Open", this, SLOT(OnOpen()), QKeySequence("CTRL+O"));
	m_cDebugger.PopulateMenuBar(pMenuBar);
	setMenuBar(pMenuBar);
}

void VirtualMachine::OnOpen()
{
	QString openFile = QFileDialog::getOpenFileName(this, "Choose file...", "./", "Binary files (*.ef)");

	if (openFile == "")
		return;

	LoadProgram(openFile);
}