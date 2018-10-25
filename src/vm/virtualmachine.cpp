#include "virtualmachine.h"
#include <console>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <core>
#include <cpumgr>
#include <assets>
#include <QDockWidget>

VirtualMachine::VirtualMachine(QWidget *parent)
	: QMainWindow(parent)
{
	QHBoxLayout* pCLILayout = new QHBoxLayout();
	QVBoxLayout* pLayout = new QVBoxLayout();
	pLayout->addWidget(new CConsole());
	pCLI = new QLineEdit();
	pButton = new QPushButton();
	pCLILayout->addWidget(pCLI);
	pCLILayout->addWidget(pButton);
	pLayout->addLayout(pCLILayout);
	QWidget* pCentralWidget = new QWidget();
	pCentralWidget->setLayout(pLayout);
	setCentralWidget(pCentralWidget);

	QDockWidget* pDock = new QDockWidget(this);
	pDock->setWidget(new CRegisterWindow());
	addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, pDock);

	connect(pButton, SIGNAL(clicked()), this, SLOT(ProcessCommand()));
}

void VirtualMachine::ProcessCommand()
{
	QString command = pCLI->text();
	QStringList arguments = command.split(' ');

	if (arguments[0] == "run")
	{
		CCPUManager* pCPU = GetCore()->GetManager<CCPUManager>();
		pCPU->Run();
	}
}
