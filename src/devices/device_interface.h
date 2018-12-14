#pragma once

#include "devices_global.h"
#include <QObject>

class DEVICES_EXPORT IIODevice : public QObject
{

	Q_OBJECT

public:
	enum EOperationMode
	{
		Input = 1,
		Output = 2
	};

public:
	virtual ~IIODevice() = default;
	virtual void Process(quint32, quint32) = 0;

signals:
	void SigDeviceChanged();
};

