#pragma once

#include <QObject>
#include "plugin_global.h"
#include <core>

class IUnknown;

class PLUGIN_EXPORT CPluginManager : public QObject
{

	Q_OBJECT

public:
	CPluginManager(QObject* pParent = nullptr);
	virtual ~CPluginManager();

	void RegisterInterface(IUnknown* pPlugin);
/*
	template <typename INTERFACE>
	IUnknown* QueryInterface()
	{
		for (auto it : m_lstPlugins)
		{
			INTERFACE* pPlugin = dynamic_cast<INTERFACE*>(*it);
			if (pPlugin != nullptr)
				return pPlugin;
		}

		return nullptr;
	}*/

	virtual QString GetName() const = 0;

signals:
	void SigUpdate();

protected:
	QList<IUnknown*> m_lstPlugins;
};