#pragma once

#include "translator_global.h"

#include <QString>

class TRANSLATOR_EXPORT CAssembler
{
public:
	CAssembler(QString const& path);

	void Assemble();
	void Output(QString path = "");

private:
	QString m_strPath;
	QString m_strData;
	QString m_strDataOutput;
};
