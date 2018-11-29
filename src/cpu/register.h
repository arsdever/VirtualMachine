#pragma once

#include "cpu_global.h"
#include <QByteArray>
#include <exception>

class CPU_EXPORT CRegisterSet
{
public:
	CRegisterSet(int bytes);
	~CRegisterSet();

	template <typename T>
	void SetValue(int address, T value)
	{
		if (address + sizeof(T) > m_aBuffer.size())
			throw invalid_cast_exception();

		char* data = m_aBuffer.data();
		T* dest = (T*)(data + address);
		*dest = value;
	}

	template <typename T>
	T GetValue(int address) const
	{
		if (address + sizeof(T) > m_aBuffer.size())
			throw invalid_cast_exception();

		const char* data = m_aBuffer.data();
		T* dest = (T*)(data + address);
		return *dest;
	}

private:
	QByteArray m_aBuffer;

public:
	class register_exception : public std::exception {};
	class invalid_cast_exception : public register_exception {};
};

