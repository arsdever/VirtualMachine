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
	QPalette palette = w.palette();
	palette.setColor(QPalette::Active, QPalette::Background, QColor(23, 23, 23));
	palette.setColor(QPalette::Active, QPalette::WindowText, QColor(191, 191, 191));
	palette.setColor(QPalette::Active, QPalette::Text, QColor(191, 191, 191));
	palette.setColor(QPalette::Active, QPalette::Foreground, QColor(191, 191, 191));
	palette.setColor(QPalette::Active, QPalette::Button, QColor(32, 32, 32));
	palette.setColor(QPalette::Active, QPalette::ButtonText, QColor(32, 32, 32));
	palette.setColor(QPalette::Active, QPalette::ToolTipBase, QColor(32, 32, 32));
	palette.setColor(QPalette::Active, QPalette::ToolTipText, QColor(191, 191, 191));
	palette.setColor(QPalette::Active, QPalette::Base, QColor(32, 32, 32));
	palette.setColor(QPalette::Active, QPalette::AlternateBase, QColor(32, 32, 32));

	palette.setColor(QPalette::Inactive, QPalette::Background, QColor(32,32,32));
	palette.setColor(QPalette::Inactive, QPalette::WindowText, QColor(191, 191, 191));
	palette.setColor(QPalette::Inactive, QPalette::Text, QColor(191, 191, 191));
	palette.setColor(QPalette::Inactive, QPalette::Foreground, QColor(191, 191, 191));
	palette.setColor(QPalette::Inactive, QPalette::Button, QColor(48,48,48));
	palette.setColor(QPalette::Inactive, QPalette::ButtonText, QColor(48,48,48));
	palette.setColor(QPalette::Inactive, QPalette::ToolTipBase, QColor(48,48,48));
	palette.setColor(QPalette::Inactive, QPalette::ToolTipText, QColor(191, 191, 191));
	palette.setColor(QPalette::Inactive, QPalette::Base, QColor(48,48,48));
	palette.setColor(QPalette::Inactive, QPalette::AlternateBase, QColor(48,48,48));
	w.setPalette(palette);
	w.show();
	return a.exec();
}
