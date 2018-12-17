#pragma once

#include "core_global.h"
#include "macros.h"
#include <QString>

class CCPU;
class CRAM;

class CORE_EXPORT IUnknown
{
public:
	virtual QString const& GetUUID() const = 0;
	virtual ~IUnknown() = 0;
};

template <typename INTERFACE_TYPE, typename RETURN_TYPE = void>
struct IFunctor
{
	virtual RETURN_TYPE operator() (INTERFACE_TYPE* plugin) = 0;
};

template <typename INTERFACE_TYPE, typename RETURN_TYPE = void>
struct CORE_EXPORT ICoreFunctor : public IFunctor<INTERFACE_TYPE, RETURN_TYPE> {};

class CORE_EXPORT ILogger : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

	GENERATE_FUNCTOR_1(Core, ILogger, Info, QString const&)
	GENERATE_FUNCTOR_1(Core, ILogger, Warning, QString const&)
	GENERATE_FUNCTOR_1(Core, ILogger, Error, QString const&)

public:
	virtual void Info(QString const&) = 0;
	virtual void Warning(QString const&) = 0;
	virtual void Error(QString const&) = 0;
};

class CORE_EXPORT IVMInformation : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

	GENERATE_FUNCTOR_1(Core, IVMInformation, UpdateGRegistersInformation, CCPU*)
	GENERATE_FUNCTOR_1(Core, IVMInformation, UpdateARegistersInformation, CCPU*)
	GENERATE_FUNCTOR_1(Core, IVMInformation, UpdateMemoryInformation, CRAM*)
	GENERATE_FUNCTOR_2(Core, IVMInformation, UpdateCallStack, quint32, CRAM*)

public:
	virtual void UpdateGRegistersInformation(CCPU*) = 0;
	virtual void UpdateARegistersInformation(CCPU*) = 0;
	virtual void UpdateMemoryInformation(CRAM*) = 0;
	virtual void UpdateCallStack(quint32, CRAM*) = 0;
};

class CORE_EXPORT IDebugger : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

	GENERATE_FUNCTOR_1(Core, IDebugger, SetBreakpoint, quint32)
	GENERATE_FUNCTOR_1(Core, IDebugger, UnsetBreakpoint, quint32)
	GENERATE_FUNCTOR_1(Core, IDebugger, ToggleBreakpoint, quint32)
	GENERATE_FUNCTOR_1(Core, IDebugger, SetRunningAddress, quint32)
	GENERATE_FUNCTOR_0(Core, IDebugger, ClearBreakpoints)

public:
	virtual void SetBreakpoint(quint32) = 0;
	virtual void UnsetBreakpoint(quint32) = 0;
	virtual void ToggleBreakpoint(quint32) = 0;
	virtual void SetRunningAddress(quint32) = 0;
	virtual void ClearBreakpoints() = 0;
};

class CORE_EXPORT IIODevice : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

public:
	virtual void In(char&) = 0;
	virtual void Out(quint32) = 0;
	virtual void Out(char) = 0;
};