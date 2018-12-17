#pragma once

#include "vm_global.h"
#include <QMainWindow>

class CVirtualMachine;
class CDebugger;
class QPlainTextEdit;

class VM_EXPORT CVirtualMachineWindow : public QMainWindow
{
	Q_OBJECT

public:
	CVirtualMachineWindow(QWidget *parent = Q_NULLPTR);
	~CVirtualMachineWindow() {};

	void LoadPlugin(QString const& plgPath);
	CVirtualMachine* GetVM() const;

	void LoadProgram(QString const& path);
	void InitMenuBar();
	
protected:
	void dragEnterEvent(QDragEnterEvent* pEvent) override;
	void dropEvent(QDropEvent* pEvent) override;
	
protected slots:
	void OnOpen();

private:
	CVirtualMachine* m_pVM;
	QPlainTextEdit* m_pEditor;
};
