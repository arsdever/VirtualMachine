#pragma once

#include "assets_global.h"
#include <QWidget>
#include "ui_registerwindow.h"

class ASSETS_EXPORT CRegisterWindow : public QWidget
{

	Q_OBJECT

public:
	CRegisterWindow(QWidget* pParent = nullptr);
	~CRegisterWindow();

protected slots:
	void ChangeRegisters();
	void UpdateValues();

public:
	Ui_Form form;
};
