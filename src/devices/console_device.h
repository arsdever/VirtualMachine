#pragma once

#include <QObject>
#include "devices_global.h"
#include "device_interface.h"

class DEVICES_EXPORT CConsoleDevice : public IIODevice
{
public:
	CConsoleDevice();
	~CConsoleDevice();

	virtual void Process(quint32 mode, quint32 data) override;
	QString GetData() const { return m_strData; }

private:
	QString m_strData;
};

