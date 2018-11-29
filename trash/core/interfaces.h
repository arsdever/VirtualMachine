#pragma once

#include "core_global.h"

class CRegisterSet;
class CCPUState;

class CORE_EXPORT IUnknown
{
public:
	static const QString UUID;
	virtual ~IUnknown() {}
};

class CORE_EXPORT ICPU : public IUnknown
{
public:
	ICPU() {}
	~ICPU() {}
	virtual void Start() = 0;
	virtual void Run() = 0;
	virtual void Fetch() = 0;
	virtual void Decode() = 0;
	virtual void Execute() = 0;
	virtual CCPUState GetState() const = 0;
	virtual CRegisterSet GetRegisters() const = 0;
	virtual void UpdateRegisters(CRegisterSet const& newState) = 0;
};

class CORE_EXPORT IRegister : public IUnknown
{
public:
	IRegister() {}
	~IRegister() {}
	//QString GetName() const override { return "register"; }
};

class CORE_EXPORT IConsole : public IUnknown
{
public:
	IConsole() {}
	~IConsole() {}
	virtual void PrintMessage(QString const&) = 0;
	virtual void PrintError(QString const&) = 0;
	virtual void PrintWarning(QString const&) = 0;
	virtual void GetInput(QString const&) = 0;
};