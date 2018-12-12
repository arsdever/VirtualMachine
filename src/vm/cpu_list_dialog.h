#pragma once

#include "vm_global.h"
#include <QDialog>

class CVirtualMachine;

class VM_EXPORT CCPUListDialog : public QDialog
{

	Q_OBJECT

public:
	CCPUListDialog(CVirtualMachine* pVM, QWidget* pParent = nullptr);
	~CCPUListDialog();

	QString GetSelection() const;

private slots:
	void Accept();
	void Cancel();
	void SelectionChanged();

private:
	QString m_strSelection;
};