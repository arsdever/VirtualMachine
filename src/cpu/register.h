#pragma once

#include "cpu_global.h"
#include <QByteArray>

class CPU_EXPORT CRegisterSet
{
public:
	CRegisterSet(int bytes);
	~CRegisterSet();

	template <typename T>
	void SetValue(int address, T value)
	{
		if (address + sizeof(T) > m_aBuffer.size())
			return;

		char* data = m_aBuffer.data();
		T* dest = (T*)(data + address);
		*dest = value;
	}

	template <typename T>
	T GetValue(int address) const
	{
		if (address + sizeof(T) > m_aBuffer.size())
			return 0;

		const char* data = m_aBuffer.data();
		T* dest = (T*)(data + address);
		return *dest;
	}

private:
	QByteArray m_aBuffer;
};

