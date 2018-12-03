#pragma once

#include "debugger_global.h"
#include <QMainWindow>
#include <QMap>

class QPushButton;
class CCPU;
class CRegisterWindow;
class CARegisterWindow;
class QTextEdit;
class QMenuBar;

class DEBUGGER_EXPORT CDebugger : public QWidget
{

	Q_OBJECT

public:
	CDebugger();
	
	void HandleBreakpoint();
	void LoadBreakpoints(QString const& path);

	void AttachToProcess(CCPU* pCPU);
	void Detach();

	CRegisterWindow* GetRegisterAreaWidget() const { return m_pRegView; }
	CARegisterWindow* GetARegisterAreaWidget() const { return m_pARegView; }
	QString GetCurrentInstruction() const { return m_strCurrentInstruction; }
	void PopulateMenuBar(QMenuBar* pMenuBar);

	void UpdateInformation();
	void UpdateMemory();
	void SetBreakpoint(quint32 nPosition);

public slots:
	void Run();
	void Step();
	void StepInto(bool b = true);
	void SetBreakpoint();
	void SetMemory();
	void ShowMemory();
	void SetRegisterValue(quint8, quint32);

	void ToggleRegisters();
	void ToggleARegisters();

signals:
	void Update();

private:
	QMap<quint32, quint8> m_mapBreakpoints;
	CCPU* m_pProcess;
	CRegisterWindow* m_pRegView;
	CARegisterWindow* m_pARegView;
	QTextEdit* m_pMemory;
	QString m_strCurrentInstruction;
	bool m_bRunning;
};
