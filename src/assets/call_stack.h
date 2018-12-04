#pragma once

#include "assets_global.h"
#include <QListWidget>

class ASSETS_EXPORT CCallStackView : public QListWidget
{
public:
	CCallStackView(QWidget* pParent = nullptr);
	~CCallStackView();
	void Clear();
};

