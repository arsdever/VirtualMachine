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
	, m_pEditor(new QPlainTextEdit())
{
	connect(&m_cDebugger, SIGNAL(Update()), this, SLOT(UpdateStatusBar()));
	//connect(&m_cDebugger, SIGNAL(Update()), this, SLOT(UpdateStatusBar()));
	setStatusBar(new QStatusBar());

	m_cDebugger.LoadBreakpoints("./debug.pdb");
	m_cDebugger.AttachToProcess(&m_cCPU);

	QDockWidget* pRegViewDock = new QDockWidget(this);
	pRegViewDock->setWidget(m_cDebugger.GetRegisterAreaWidget());
	pRegViewDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	addDockWidget(Qt::BottomDockWidgetArea, pRegViewDock);

	QDockWidget* pARegViewDock = new QDockWidget(this);
	pARegViewDock->setWidget(m_cDebugger.GetARegisterAreaWidget());
	pARegViewDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	addDockWidget(Qt::RightDockWidgetArea, pARegViewDock);

	QDockWidget* pDebuggerDock = new QDockWidget(this);
	pDebuggerDock->setWidget(&m_cDebugger);
	pDebuggerDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	addDockWidget(Qt::BottomDockWidgetArea, pDebuggerDock);

	
	m_pEditor->setReadOnly(true);

	InitMenuBar();

	setCentralWidget(m_pEditor);
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
	m_cDebugger.UpdateInformation();
	m_pEditor->setPlainText(CDisassembler::Disassemble(path));
}

void VirtualMachine::InitMenuBar()
{
	QMenuBar* pMenuBar = new QMenuBar();
	QMenu* fileMenu = pMenuBar->addMenu("File");
	fileMenu->addAction(QIcon(":/Resources/folder_16x16.png"), "Open", this, SLOT(OnOpen()));
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