#include <QApplication>
#include "virtualmachine.h"
#include <QFont>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFont font("Courier New", 10);
	font.setStyleHint(QFont::Monospace);
	a.setFont(font);

	VirtualMachine w;
	w.show();
	return a.exec();
}
