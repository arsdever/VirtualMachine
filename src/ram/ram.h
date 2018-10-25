#pragma once

#include "ram_global.h"
#include <pluginmgr>

class QByteArray;

class RAM_EXPORT CRAMManager : public CPluginManager
{

	Q_OBJECT

public:
	CRAMManager();
	~CRAMManager();

	CRAMManager& WriteMemory(unsigned int address, int data);
	CRAMManager& WriteMemory(unsigned int address, QByteArray const& data);
	CRAMManager& WriteMemory(unsigned int address, const char* data, unsigned int size);

	CRAMManager& ReadMemory(unsigned int address, int& data);
	CRAMManager& ReadMemory(unsigned int address, QByteArray& buffer);
	CRAMManager& ReadMemory(unsigned int address, char* data, unsigned int size);

	operator bool() { return m_bOk; }

private:
	QByteArray* m_pMemory;
	bool m_bOk;
};
