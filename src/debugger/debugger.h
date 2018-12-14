#pragma once

#include "debugger_global.h"
#include <QMap>
#include <core>
#include <QList>

class QPushButton;
class CCPU;
class QTextEdit;
class QMenuBar;
class CCodeEditor;
class IDisassembler;
class QAction;
class CVirtualMachineWindow;

class DEBUGGER_EXPORT CDebugger : public QObject
{

	Q_OBJECT

	IMPLEMENT_BEGIN(CDebugger, Debugger)
		virtual void SetBreakpoint(quint32) override;
		virtual void UnsetBreakpoint(quint32) override;
		virtual void ToggleBreakpoint(quint32) override;
		virtual void SetRunningAddress(quint32) override {}
		virtual void ClearBreakpoints() override;
	IMPLEMENT_END(Debugger)

public:
	CDebugger(CVirtualMachineWindow* pParent = nullptr);
	
	void HandleBreakpoint();
	void LoadBreakpoints(QString const& path);

	QList<QAction*> GetActions() const;

	void Detach();

	QTextEdit* GetMemoryWidget() const { return m_pMemory; }
	QString GetCurrentInstruction() const { return m_strCurrentInstruction; }

	void UpdateMemory();

public slots:

	// CPU manipulators

	void Run();
	void Step();
	void StepInto(bool b = true);

	// Management

	void SetBreakpoint(quint32 address);
	void SetBreakpoint();
	void SetCodeEditor(CCodeEditor* editor);
	void SetMemory();
	void AttachToProcess();
	void SetRegisterValue(quint8, quint32);
	void UpdateInformation();
	void OnProgramLoaded(QString const& strNewPath);

private:
	QMap<quint32, quint8> m_mapBreakpoints;
	CCPU* m_pProcess;
	QTextEdit* m_pMemory;
	QString m_strCurrentInstruction;
	bool m_bRunning;
	bool m_bAttached;
	CCodeEditor* m_pCodeEditor;
	IDisassembler* disassembler;

	QList<QAction*> m_lstActions;
};
