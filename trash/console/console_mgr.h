#pragma once

#include <pluginmgr>
#include "console_global.h"
#include <interfaces>

class CONSOLE_EXPORT CConsoleManager : public CPluginManager
{

	Q_OBJECT

public:
	CConsoleManager(QObject* pParent = nullptr);
	~CConsoleManager();

	void PrintMessage(QString const& msg);
	void PrintWarning(QString const& msg);
	void PrintError(QString const& msg);
	void GetInput(QString const&);

	QString GetName() const override { return "console"; }
};
