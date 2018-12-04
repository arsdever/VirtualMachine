#pragma once

#include "debugger_global.h"
#include <QMainWindow>
#include <QMap>
#include <core>

class QPushButton;
class CCPU;
class CRegisterWindow;
class CARegisterWindow;
class CCallStackView;
class QTextEdit;
class QMenuBar;

class DEBUGGER_EXPORT CDebugger : public QWidget
{

	Q_OBJECT

	REGISTER_INTERFACE(CDebugger, Debugger)
		virtual void SetBreakpoint(quint32) override;
		virtual void UnsetBreakpoint(quint32) override;
		virtual void ToggleBreakpoint(quint32) override;
		virtual void SetRunningAddress(quint32) override;
		virtual void ClearBreakpoints() override;
		REGISTER_INTERFACE_END(Debugger)

public:
	CDebugger();
	
	void HandleBreakpoint();
	void LoadBreakpoints(QString const& path);

	void AttachToProcess(CCPU* pCPU);
	void Detach();

	CRegisterWindow* GetRegisterAreaWidget() const { return m_pRegView; }
	CARegisterWindow* GetARegisterAreaWidget() const { return m_pARegView; }
	CCallStackView* GetCallStackWidget() const { return m_pCallStack; }
	QTextEdit* GetMemoryWidget() const { return m_pMemory; }
	QString GetCurrentInstruction() const { return m_strCurrentInstruction; }
	void PopulateMenuBar(QMenuBar* pMenuBar);
	QStringList CollectCallStack();

	void UpdateInformation();
	void UpdateMemory();

public slots:
	void Run();
	void Step();
	void StepInto(bool b = true);
	void SetBreakpoint(quint32 address);
	void ToggleBreakpoint();
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
	CCallStackView* m_pCallStack;
	QString m_strCurrentInstruction;
	bool m_bRunning;
};
