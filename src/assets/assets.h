#pragma once

#include "assets_global.h"
#include <QWidget>
#include "./GeneratedFiles/ui_registerwindow.h"
#include "./GeneratedFiles/ui_aregisterwindow.h"

class CCPU;

class ASSETS_EXPORT CRegisterWindow : public QWidget
{

	Q_OBJECT

public:
	CRegisterWindow(QWidget* pParent = nullptr);
	~CRegisterWindow();

public slots:
	void UpdateValues(CCPU* pCPU);

private slots:
	void ChangeHandler();

signals:
	void ChangeRegister(quint8, quint32);

public:
	Ui_RegWindow form;
};

class ASSETS_EXPORT CARegisterWindow : public QWidget
{

	Q_OBJECT

public:
	CARegisterWindow(QWidget* pParent = nullptr);
	~CARegisterWindow();

public slots:
	void UpdateValues(CCPU* pCPU);

public:
	Ui_ARegWindow form;
};
