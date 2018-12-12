#include <QApplication>
#include <vm>
#include <QFont>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFont font("Ubuntu Mono", 10);
	font.setStyleHint(QFont::Monospace);
	a.setFont(font);

	CVirtualMachineWindow w;
	w.show();
	return a.exec();
}
