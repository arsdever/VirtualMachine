#pragma once

#include "devices_global.h"
#include <QWidget>
#include <core>
#include <devices>
#include <QTextEdit>

class QEventLoop;

class DEVICES_EXPORT CConsoleWindow : public QTextEdit
{

	Q_OBJECT

	IMPLEMENT_BEGIN(CConsoleWindow, IODevice)
		virtual void In(char&) override;
		virtual void Out(quint32) override;
		virtual void Out(char) override;
	IMPLEMENT_END(IODevice)

public:
	CConsoleWindow(QWidget* pParent = nullptr);
	~CConsoleWindow();

	IIODevice* GetDevice() const { return (IIODevice*)&m_xIODevice; }

protected:
	void keyPressEvent(QKeyEvent* pEvent) override;

private:
	char m_cInput;
	QEventLoop* m_pLoop;
};

