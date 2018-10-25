#pragma once

#include "core_global.h"
#include <QString>

class CRegisterSet;

class CORE_EXPORT IUnknown
{
public:
	static const QString UUID;
	virtual ~IUnknown();
};

class CORE_EXPORT ICPU : public IUnknown
{
public:
	virtual void Fetch() = 0;
	virtual void Decode() = 0;
	virtual void Execute() = 0;
	//virtual void UpdateRegisters(CRegisterSet const&) = 0;
	//virtual CRegisterSet GetRegisters() const = 0;
};

class CORE_EXPORT IRegister : public IUnknown
{
public:
};

class CORE_EXPORT IConsole : public IUnknown
{
public:
	virtual void PrintMessage(QString const&) = 0;
	virtual void PrintError(QString const&) = 0;
	virtual void PrintWarning(QString const&) = 0;
	virtual void GetInput(QString const&) = 0;
};