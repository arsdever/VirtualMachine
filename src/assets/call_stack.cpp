#include "call_stack.h"

CCallStackView::CCallStackView(QWidget* pParent)
	: QListWidget(pParent)
{
}

CCallStackView::~CCallStackView()
{
}

void CCallStackView::Clear()
{
	clear();
}