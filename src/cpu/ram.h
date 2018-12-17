#pragma once

#include "cpu_global.h"
#include <QObject>
#include <QByteArray>
#include <exception>

class CPU_EXPORT CRAM : public QObject
{

	Q_OBJECT

public:
	CRAM(quint32 nSize = 0x10000);
	~CRAM();

	template <typename INT_TYPE>
	INT_TYPE& operator[](qint32 address)
	{
		if (address + sizeof(INT_TYPE) > m_nSize)
		{
			throw index_out_of_bounds_exception();
		}

		return (INT_TYPE&)(*(quint8*)(m_pMemory + address));
	}

	quint32 GetSize() const { return m_nSize; }

private:
	quint32 m_nSize;
	quint8* m_pMemory;

public:
	struct ram_exception : public std::exception {};
	struct index_out_of_bounds_exception : ram_exception {};
};
