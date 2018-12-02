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

VirtualMachine::VirtualMachine(QWidget *parent)
	: QMainWindow(parent)
	, m_cCPU()
	, m_cDebugger()
{
	QFile file("./fibonachi.exe");
	file.open(QIODevice::ReadOnly);
	if (file.isOpen())
	{
		QByteArray content = file.readAll();
		int i = 0;
		for (; i < content.size(); ++i)
			CRAM::instance()->operator[]<quint8>(i) = content[i];

		m_cCPU.m_sState.SF = CRAM::instance()->GetSize();
		m_cCPU.m_sState.SP = CRAM::instance()->GetSize();
		m_cCPU.m_sState.PC = CRAM::instance()->operator[]<quint32>(8);
	}
	file.close();
	m_cDebugger.AttachToProcess(&m_cCPU);
	m_cDebugger.LoadBreakpoints("./debug.pdb");

	QVBoxLayout* pLayout = new QVBoxLayout();
	pLayout->addWidget(&m_cDebugger);
	QWidget* pCentralWidget = new QWidget();
	pCentralWidget->setLayout(pLayout);
	setCentralWidget(pCentralWidget);

}