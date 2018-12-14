#include "call_stack.h"
#include <cpu>
#include <ram>
#include "disassembler.h"

CCallStackView::CCallStackView(QWidget* pParent)
	: QListWidget(pParent)
{
}

CCallStackView::~CCallStackView()
{
}

void CCallStackView::XVMInformation::UpdateCallStack(QStringList const& data)
{
	m_pThis->clear();
	m_pThis->addItems(data);
}