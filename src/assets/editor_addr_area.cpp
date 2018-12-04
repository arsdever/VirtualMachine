#include "editor_addr_area.h"
#include "code_editor.h"
#include <QPaintEvent>

CEditorAddressArea::CEditorAddressArea(QWidget* pParent)
	: QWidget(pParent)
	, m_pEditor(nullptr)
{
}

CEditorAddressArea::~CEditorAddressArea()
{
}

void CEditorAddressArea::SetDrawerWidget(CCodeEditor* pEditor)
{
	m_pEditor = pEditor;
}

QSize CEditorAddressArea::sizeHint() const
{
	if (m_pEditor == 0)
		return QSize(0, 0);
	return m_pEditor->GetAddressAreaDecorator()->GetWidgetSizeHint();
}

void CEditorAddressArea::paintEvent(QPaintEvent* pEvent)
{
	m_pEditor->GetAddressAreaDecorator()->DrawDecoration(pEvent, this);
}