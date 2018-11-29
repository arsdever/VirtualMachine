#include "core.h"
#include <pluginmgr>

void CCore::InstallManager(CPluginManager* pManager)
{
	for (auto mgr : m_lstManagers)
		if (mgr == pManager)
			return;

	m_lstManagers.push_back(pManager);
}

void CCore::UninstallManager(CPluginManager* pManager)
{
	for (auto mgr : m_lstManagers)
		if (mgr == pManager)
		{
			m_lstManagers.removeOne(mgr);
			return;
		}
}

CPluginManager* CCore::GetManager(QString const& strMgrName)
{
	for (auto mgr : m_lstManagers)
	{
		CPluginManager* pManager = (CPluginManager*)mgr;
		if (pManager != nullptr && pManager->GetName().toLower() == strMgrName.toLower())
			return pManager;
	}

	return nullptr;
}

CCore* CCore::s_pUniqueInstance = new CCore();

CORE_EXPORT CCore* GetCore()
{
	return CCore::s_pUniqueInstance;
}