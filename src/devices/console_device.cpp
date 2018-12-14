#include "console_device.h"

CConsoleDevice::CConsoleDevice()
{
}

CConsoleDevice::~CConsoleDevice()
{
}

void CConsoleDevice::Process(quint32 mode, quint32 data)
{
	if (mode == Output)
	{
		m_strData += (char)data;
		emit SigDeviceChanged();
	}
}
