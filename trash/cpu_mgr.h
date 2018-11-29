#pragma once

#include "cpu_global.h"
#include <pluginmgr>
#include <QMap>
#include <interfaces>

class CCPUState;
class CRegisterSet;
class Executer;
class CCPU;

class CPU_EXPORT CCPUManager : public CPluginManager
{

	Q_OBJECT

public:
	CCPUManager();
	~CCPUManager();

public:
	void Run();

	ICPU* Deploy(int nCPUCount = 1);
	ICPU* CurrentCPU() const;

	QString GetName() const override { return "cpu"; }

signals:
	void RegistersUpdated();

public:
	friend class Executor;
	ICPU* m_pCurrentCPU;
};
