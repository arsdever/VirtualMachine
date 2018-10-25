#pragma once

#include <QMainWindow>

class QLineEdit;
class QPushButton;

class VirtualMachine : public QMainWindow
{
	Q_OBJECT

public:
	VirtualMachine(QWidget *parent = Q_NULLPTR);
	~VirtualMachine() {};


public slots:
	void ProcessCommand();

private:
	QLineEdit* pCLI;
	QPushButton* pButton;
};
