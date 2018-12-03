#pragma once

#include <QMainWindow>
#include <ram>
#include <cpu>
#include "../debugger/debugger.h"

class CCPU;
class CDebugger;
class QPlainTextEdit;

class VirtualMachine : public QMainWindow
{
	Q_OBJECT

public:
	VirtualMachine(QWidget *parent = Q_NULLPTR);
	~VirtualMachine() {};

	void LoadProgram(QString const& path);
	void InitMenuBar();
	
protected:
	void UpdateStatusBar();
	
protected slots:
	void OnOpen();

private:
	CCPU m_cCPU;
	CDebugger m_cDebugger;
	QPlainTextEdit* m_pEditor;
};
