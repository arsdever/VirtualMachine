#pragma once

#include "devices_global.h"
#include <QWidget>
#include <devices>

class QTextEdit;
class QLineEdit;

class DEVICES_EXPORT CConsoleWindow : public QWidget
{

	Q_OBJECT

private:
	class XConsoleDevice : public IIODevice
	{
	public:
		virtual void Process(quint32, quint32) override;
		friend class CConsoleWindow;

	private:
		XConsoleDevice(CConsoleWindow* pParent);
		CConsoleWindow* m_pThis;
	}* m_pDevice;

public:
	CConsoleWindow(QWidget* pParent = nullptr);
	~CConsoleWindow();

	IIODevice* GetDevice() const { return m_pDevice; }

private:
	QTextEdit* m_pOutput;
	QLineEdit* m_pInput;
};

