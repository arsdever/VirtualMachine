#pragma once

#include "debugger_global.h"
#include <QWidget>
#include <QMap>

class QPushButton;
class CCPU;
class CRegisterWindow;
class CARegisterWindow;
class QTextEdit;

class DEBUGGER_EXPORT CDebugger : public QWidget
{

	Q_OBJECT

public:
	CDebugger();
	
	void HandleBreakpoint();
	void LoadBreakpoints(QString const& path);

	void AttachToProcess(CCPU* pCPU);
	void Detach();

	void UpdateInformation();
	void UpdateMemory();
	void SetBreakpoint(quint32 nPosition);

public slots:
	void Run();
	void Step();
	void SetBreakpoint();
	void SetMemory();
	void ShowMemory();
	void SetRegisterValue(quint8, quint32);

	void ToggleRegisters();
	void ToggleARegisters();

signals:
	void UpdateValues();

private:
	QMap<quint32, quint8> m_mapBreakpoints;
	CCPU* m_pProcess;
	CRegisterWindow* m_pRegView;
	CARegisterWindow* m_pARegView;
	QTextEdit* m_pMemory;
	bool m_bRunning;
};
