#include "console_window.h"

#include <QEventLoop>
#include <QKeyEvent>

CConsoleWindow::CConsoleWindow(QWidget* pParent)
	: QTextEdit(pParent)
	, m_pLoop(new QEventLoop)
{
	setReadOnly(true);
	setDisabled(true);
}

CConsoleWindow::~CConsoleWindow()
{
}

void CConsoleWindow::keyPressEvent(QKeyEvent* pEvent)
{
	m_cInput = pEvent->text().toLatin1()[0] - '0';
	m_pLoop->quit();
}

void CConsoleWindow::XIODevice::In(char& pData)
{
	m_pThis->setDisabled(false);
	m_pThis->setFocus();
	m_pThis->m_pLoop->exec();
	pData = m_pThis->m_cInput;
	m_pThis->setDisabled(true);
}

void CConsoleWindow::XIODevice::Out(quint32 pData)
{
	m_pThis->setText(m_pThis->toPlainText() + QString::number(pData));
}

void CConsoleWindow::XIODevice::Out(char pData)
{
	m_pThis->setText(m_pThis->toPlainText() + pData);
}