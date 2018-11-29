#pragma once

#include "core.h"
#include <QString>

class CORE_EXPORT IUnknown
{
public:
	virtual QString const& GetUUID() = 0;
	virtual ~IUnknown() = 0;
};

class CORE_EXPORT IRAM : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() { return UUID; }
};

class CORE_EXPORT IDebugger : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() { return UUID; }

	virtual void HandleBreakpoint() = 0;
};