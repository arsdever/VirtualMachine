#pragma once

#include <pluginmgr>
#include "console_global.h"

class CONSOLE_EXPORT CConsoleManager : public CPluginManager
{

	Q_OBJECT

public:
	CConsoleManager(QObject* pParent = nullptr);
	~CConsoleManager();

	void PrintInfoMessage(QString const& msg);
	void PrintWarningMessage(QString const& msg);
	void PrintErrorMessage(QString const& msg);
};
