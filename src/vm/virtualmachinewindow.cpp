#include "virtualmachinewindow.h"
#include "virtualmachine.h"

#include <assets>
//#include <debugger>
#include <devices>

#include <QLibrary>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDockWidget>
#include <QMessageBox>
#include <QFile>
#include <QStatusBar>
#include <QPlainTextEdit>
#include <QMenuBar>
#include <QFileDialog>
#include <QRegularExpression>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>

CVirtualMachineWindow::CVirtualMachineWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_pVM(new CVirtualMachine(0xffff, 1, this))
	, m_pEditor(new CCodeEditor())
{
	setAcceptDrops(true);
	setStatusBar(new QStatusBar());
	InitMenuBar();

	LoadPlugin("debugger");
	CConsoleWindow* pConsoleWindow = new CConsoleWindow();
	m_pVM->SetDevice(pConsoleWindow->GetDevice(), 0);
	QDockWidget* pConsoleDock = new QDockWidget("Console", this);
	pConsoleDock->setWidget(pConsoleWindow);
	pConsoleDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	addDockWidget(Qt::RightDockWidgetArea, pConsoleDock);
	/*QWidget* pCentralWidget = new QWidget();
	pCentralWidget->setLayout(new QVBoxLayout());
	pCentralWidget->layout()->addWidget(m_pEditor);
	CEditorNumberArea* pNumberArea = new CEditorNumberArea();
	pNumberArea->SetDrawerWidget((CCodeEditor*)m_pEditor);
	pCentralWidget->layout()->addWidget(pNumberArea);*/
	m_pEditor->setAcceptDrops(false);
	setCentralWidget(m_pEditor);

}

CVirtualMachine* CVirtualMachineWindow::GetVM() const
{
	return m_pVM;
}

void CVirtualMachineWindow::LoadPlugin(QString const& plgPath)
{
	typedef bool (*InitializerFPTR)(QMainWindow*);
	qDebug() << QDir::currentPath();
	InitializerFPTR initializer = (InitializerFPTR)QLibrary::resolve(plgPath
#ifdef _DEBUG
		+ "d"
#endif
	, "LoadPlugin");
	if (initializer == nullptr)
		return;

	initializer(this);
}

void CVirtualMachineWindow::LoadProgram(QString const& path)
{
	try
	{
		m_pVM->LoadProgram(path);
	}
	catch (CVirtualMachine::invalid_file const&)
	{
		QMessageBox::critical(this, "Invalid file", "File was modified or not supported.");
		return;
	}
	catch (std::exception const&)
	{
		QMessageBox::critical(this, "Error", "Unknown error occured.");
		return;
	}

	typedef void(*Disassembler)(QString const&, QString&);
	Disassembler disassemble = (Disassembler)QLibrary::resolve(QString("debugger")
#ifdef _DEBUG
		+ "d"
#endif
	, "Disassemble");

	if (disassemble == nullptr)
		return;

	QString disassembled;
	disassemble(path, disassembled);
	m_pEditor->setPlainText(disassembled);
	//m_pDebugger->AttachToProcess(CVirtualMachine::GlobalInstance()->GetCPU());
	//QString debuggerPath = path;
	//debuggerPath.replace(QRegularExpression(".ef$"), QString(".dbg"));
	//m_pDebugger->LoadBreakpoints(debuggerPath);
}

void CVirtualMachineWindow::InitMenuBar()
{
	QMenuBar* pMenuBar = new QMenuBar();
	QMenu* VMMenu = pMenuBar->addMenu("VM");
	VMMenu->addAction(QIcon(":/Resources/folder_16x16.png"), "Open program", this, SLOT(OnOpen()), QKeySequence("CTRL+O"));
	VMMenu->addAction(QIcon(":/Resources/play_icon_16x16.png"), "Run", m_pVM, SLOT(Run()), QKeySequence("CTRL+R"));
	setMenuBar(pMenuBar);
}

void CVirtualMachineWindow::OnOpen()
{
	QString openFile = QFileDialog::getOpenFileName(this, "Choose file...", "./", "Binary files (*.ef)");

	if (openFile == "")
		return;

	LoadProgram(openFile);
}

void CVirtualMachineWindow::dragEnterEvent(QDragEnterEvent* pEvent)
{
	qDebug() << "dragenter";
	pEvent->accept();
}

void CVirtualMachineWindow::dropEvent(QDropEvent* pEvent)
{
	QStringList urls = QString(pEvent->mimeData()->data("text/uri-list")).split("\r\n");
	urls.pop_back();
	LoadProgram(urls.back().remove(QRegularExpression("^file:///")));
	pEvent->accept();
}