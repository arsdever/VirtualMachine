#pragma once

#include "plugin_global.h"

class PLUGIN_EXPORT CPlugin
{
public:
	CPlugin();
	virtual ~CPlugin();

protected:
	virtual void Register() = 0;
};