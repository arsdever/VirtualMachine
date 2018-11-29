#pragma once

#include <QMainWindow>
#include <ram>
#include <cpu>
#include "../debugger/debugger.h"

class CCPU;
class CDebugger;

class VirtualMachine : public QMainWindow
{
	Q_OBJECT

public:
	VirtualMachine(QWidget *parent = Q_NULLPTR);
	~VirtualMachine() {};
	
private:
	CCPU m_cCPU;
	CDebugger m_cDebugger;
};
