#pragma once

#include "vm_global.h"
#include <QTextEdit>
#include <devices>

class VM_EXPORT CConsoleWindow : public QTextEdit
{

	Q_OBJECT

public:
	CConsoleWindow(QWidget* pParent = nullptr);
	~CConsoleWindow();

	void SetDevice(CConsoleDevice* pDevice);

private slots:
	void OnDeviceChanged();

private:
	CConsoleDevice* m_pDevice;
};

