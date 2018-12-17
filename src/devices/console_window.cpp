#include "console_window.h"

#include <QTextEdit>
#include <QLineEdit>
#include <QLayout>

CConsoleWindow::CConsoleWindow(QWidget* pParent)
	: QWidget(pParent)
	, m_pInput(new QLineEdit)
	, m_pOutput(new QTextEdit)
	, m_pDevice(new XConsoleDevice(this))
{
	setLayout(new QVBoxLayout());
	layout()->addWidget(m_pOutput);
	layout()->addWidget(m_pInput);
	m_pOutput->setReadOnly(true);
}

CConsoleWindow::~CConsoleWindow()
{
}

CConsoleWindow::XConsoleDevice::XConsoleDevice(CConsoleWindow* pParent)
	: m_pThis(pParent) {}

void CConsoleWindow::XConsoleDevice::Process(quint32 mode, quint32 data)
{
	if (mode == Output)
	{
		m_pThis->m_pOutput->setText(m_pThis->m_pOutput->toPlainText() + (char)data);
	}
	else if (mode == Input)
	{
		m_pThis->m_pOutput->setText(m_pThis->m_pOutput->toPlainText() + (char)data);
	}
}