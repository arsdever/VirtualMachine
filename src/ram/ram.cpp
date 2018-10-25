#include "ram.h"
#include <QByteArray>

CRAMManager::CRAMManager()
	: m_pMemory(new QByteArray(0xffff, 0))
{
}

CRAMManager::~CRAMManager()
{
}

CRAMManager& CRAMManager::WriteMemory(unsigned int address, int data)
{
	if (address + sizeof(data) > m_pMemory->size())
	{
		m_bOk = false;
		return *this;
	}

	char* dataCell = m_pMemory->data() + address;
	*((int*)dataCell) = data;

	m_bOk = true;
	return *this;
}

CRAMManager& CRAMManager::WriteMemory(unsigned int address, QByteArray const& data)
{
	if (address + data.size() > m_pMemory->size())
	{
		m_bOk = false;
		return *this;
	}

	m_pMemory->replace(address, data);

	m_bOk = true;
	return *this;
}

CRAMManager& CRAMManager::WriteMemory(unsigned int address, const char* data, unsigned int size)
{
	if (address + size > m_pMemory->size())
	{
		m_bOk = false;
		return *this;
	}

	char* dataCell = m_pMemory->data() + address;
	for (unsigned int i = 0; i < size; ++i, ++dataCell, ++data)
	{
		*dataCell = *data;
	}

	return *this;
}

CRAMManager& CRAMManager::ReadMemory(unsigned int address, int& data)
{
	if (address + sizeof(data) > m_pMemory->size())
	{
		m_bOk = false;
		return *this;
	}

	char* dataCell = m_pMemory->data() + address;
	data = *((int*)dataCell);

	return *this;
}

CRAMManager& CRAMManager::ReadMemory(unsigned int address, QByteArray& buffer)
{
	if (address + buffer.size() > m_pMemory->size())
	{
		m_bOk = false;
		return *this;
	}

	buffer = m_pMemory->mid(address, buffer.size());

	m_bOk = true;
	return *this;
}

CRAMManager& CRAMManager::ReadMemory(unsigned int address, char* data, unsigned int size)
{
	if (address + size > m_pMemory->size())
	{
		m_bOk = false;
		return *this;
	}

	char* dataCell = m_pMemory->data() + address;
	for (unsigned int i = 0; i < size; ++i, ++dataCell, ++data)
	{
		*data = *dataCell;
	}

	return *this;
}
