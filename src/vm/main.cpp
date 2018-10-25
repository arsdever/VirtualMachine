#include <QApplication>
#include "virtualmachine.h"
#include <cpumgr>
#include <ram>
#include <QFile>
#include <consolemgr>

int main(int argc, char *argv[])
{
	new CCPUManager();
	new CRAMManager();
	new CConsoleManager();

	QFile file("./input.exe");
	file.open(QIODevice::ReadOnly);
	if (file.isOpen())
	{
		QByteArray content = file.readAll();
		CRAMManager* pRam = GetCore()->GetManager<CRAMManager>();
		if (pRam != nullptr)
		{
			pRam->WriteMemory(0, content);
		}
	}
	file.close();

	QApplication a(argc, argv);
	VirtualMachine w;
	w.show();
	return a.exec();
}
