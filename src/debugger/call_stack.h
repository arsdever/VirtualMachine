#pragma once

#include "debugger_global.h"
#include <QListWidget>
#include <core>

class DEBUGGER_EXPORT CCallStackView : public QListWidget
{

	IMPLEMENT_BEGIN(CCallStackView, VMInformation)
		virtual void UpdateGRegistersInformation(CCPU*) override {}
		virtual void UpdateARegistersInformation(CCPU*) override {}
		virtual void UpdateCallStack(QStringList const&) override;
		virtual void UpdateMemoryInformation(CRAM*) override {}
	IMPLEMENT_END(VMInformation)

public:
	CCallStackView(QWidget* pParent = nullptr);
	~CCallStackView();
};