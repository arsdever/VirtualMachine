#include "console_mgr.h"
#include <core>
#include <interfaces>

CConsoleManager::CConsoleManager(QObject* pParent)
	: CPluginManager(pParent)
{
}

CConsoleManager::~CConsoleManager()
{
}

void CConsoleManager::PrintMessage(QString const& msg)
{
	for (auto it : m_lstPlugins)
	{
		IConsole* pConsole = dynamic_cast<IConsole*>((IUnknown*)it);

		if (pConsole == nullptr)
			continue;

		pConsole->PrintMessage(msg);
	}
}

void CConsoleManager::PrintWarning(QString const& msg)
{
	for (auto it : m_lstPlugins)
	{
		IConsole* pConsole = dynamic_cast<IConsole*>(it);

		if (pConsole == nullptr)
			continue;

		pConsole->PrintWarning(msg);
	}
}

void CConsoleManager::PrintError(QString const& msg)
{
	for (auto it : m_lstPlugins)
	{
		IConsole* pConsole = dynamic_cast<IConsole*>(it);

		if (pConsole == nullptr)
			continue;

		pConsole->PrintError(msg);
	}
}

void CConsoleManager::GetInput(QString const&)
{

}