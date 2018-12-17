#pragma once

#include "cpu_global.h"
#include <QVector>

class IIODevice;

class CPU_EXPORT CIOController
{
public:
	CIOController();
	virtual ~CIOController();

	IIODevice* operator[] (quint32 nPort);
	void RegisterDevice(IIODevice* pDevice, qint32 nPrefferedPort = -1);

	QVector<IIODevice*> m_arrController;
};