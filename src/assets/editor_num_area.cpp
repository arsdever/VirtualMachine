#include "editor_num_area.h"
#include "code_editor.h"
#include <QPaintEvent>

CEditorNumberArea::CEditorNumberArea(QWidget* pParent)
	: QWidget(pParent)
	, m_pEditor(nullptr)
{
}

CEditorNumberArea::~CEditorNumberArea()
{
}

void CEditorNumberArea::SetDrawerWidget(CCodeEditor* pEditor)
{
	m_pEditor = pEditor;
}

QSize CEditorNumberArea::sizeHint() const
{
	if (m_pEditor == 0)
		return QSize(0, 0);
	return m_pEditor->GetNumberAreaDecorator()->GetWidgetSizeHint();
}

void CEditorNumberArea::paintEvent(QPaintEvent* pEvent)
{
	m_pEditor->GetNumberAreaDecorator()->DrawDecoration(pEvent, this);
}