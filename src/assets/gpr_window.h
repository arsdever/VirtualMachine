#pragma once

#include "assets_global.h"
#include <core>

#include <QWidget>
#include <QVector>

class CCPU;
class QLineEdit;

class ASSETS_EXPORT CGRegisterWindow : public QWidget
{

	Q_OBJECT

	IMPLEMENT_BEGIN(CGRegisterWindow, VMInformation)
		virtual void UpdateGRegistersInformation(CCPU*) override;
		virtual void UpdateARegistersInformation(CCPU*) override {}
		virtual void UpdateCallStack(QStringList const&) override {}
		virtual void UpdateMemoryInformation(CRAM*) override {}
	IMPLEMENT_END(VMInformation)

public:
	CGRegisterWindow(QWidget* pParent = nullptr);
	~CGRegisterWindow();

public slots:
	void ToggleView();

private slots:
	void ChangeHandler();

signals:
	void ChangeRegister(quint8, quint32);

public:
	QVector<QLineEdit*> m_arrRegisterValues;
};

