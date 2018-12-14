#include "console_window.h"

CConsoleWindow::CConsoleWindow(QWidget* pParent)
	: QTextEdit(pParent)
	, m_pDevice(nullptr)
{
}

CConsoleWindow::~CConsoleWindow()
{
}

void CConsoleWindow::OnDeviceChanged()
{
	if (m_pDevice == nullptr)
		return;

	setText(m_pDevice->GetData());
}

void CConsoleWindow::SetDevice(CConsoleDevice * pDevice)
{
	disconnect(m_pDevice, SIGNAL(SigDeviceChanged()), this, SLOT(OnDeviceChanged()));
	m_pDevice = pDevice;
	connect(m_pDevice, SIGNAL(SigDeviceChanged()), this, SLOT(OnDeviceChanged()));
}
