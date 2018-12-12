#pragma once

#include "assets_global.h"
#include <core>

#include <QWidget>
#include <QVector>

class CCPU;
class QLineEdit;

class ASSETS_EXPORT CARegisterWindow : public QWidget
{

	Q_OBJECT

	IMPLEMENT_BEGIN(CARegisterWindow, VMInformation)
		virtual void UpdateGRegistersInformation(CCPU*) override {}
		virtual void UpdateARegistersInformation(CCPU*) override;
		virtual void UpdateCallStack(quint32, CRAM*) override {}
		virtual void UpdateMemoryInformation(CRAM*) override {}
	IMPLEMENT_END(VMInformation)

public:
	CARegisterWindow(QWidget* pParent = nullptr);
	~CARegisterWindow();

public slots:
	void ToggleView();

private:
	QVector<QLineEdit*> m_arrRegisterValues;
};
