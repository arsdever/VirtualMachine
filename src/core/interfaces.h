#pragma once

#include "core_global.h"
#include "macros.h"
#include <QString>

class CORE_EXPORT IUnknown
{
public:
	virtual QString const& GetUUID() const = 0;
	virtual ~IUnknown() = 0;
};

struct IFunction{};

template <typename INTERFACE_TYPE, typename RETURN_TYPE = void>
struct CORE_EXPORT IFunctor : public IFunction
{
	virtual RETURN_TYPE operator() (INTERFACE_TYPE* plugin) = 0;
};

class CORE_EXPORT ILogger : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

	GENERATE_FUNCTOR_1(ILogger, Info, QString const&)
		GENERATE_FUNCTOR_1(ILogger, Warning, QString const&)
		GENERATE_FUNCTOR_1(ILogger, Error, QString const&)

protected:
	virtual void Info(QString const&) = 0;
	virtual void Warning(QString const&) = 0;
	virtual void Error(QString const&) = 0;
};

class CORE_EXPORT IDebugger : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

	GENERATE_FUNCTOR_1(IDebugger, SetBreakpoint, quint32)
	GENERATE_FUNCTOR_1(IDebugger, UnsetBreakpoint, quint32)
	GENERATE_FUNCTOR_1(IDebugger, ToggleBreakpoint, quint32)
	GENERATE_FUNCTOR_1(IDebugger, SetRunningAddress, quint32)
	GENERATE_FUNCTOR_0(IDebugger, ClearBreakpoints)

protected:
	virtual void SetBreakpoint(quint32) = 0;
	virtual void UnsetBreakpoint(quint32) = 0;
	virtual void ToggleBreakpoint(quint32) = 0;
	virtual void SetRunningAddress(quint32) = 0;
	virtual void ClearBreakpoints() = 0;
};