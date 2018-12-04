#include "editor_bp_area.h"
#include "code_editor.h"
#include <QPaintEvent>

CEditorBPArea::CEditorBPArea(QWidget* pParent)
	: QWidget(pParent)
	, m_pEditor(nullptr)
{
}

CEditorBPArea::~CEditorBPArea()
{
}

void CEditorBPArea::SetDrawerWidget(CCodeEditor* pEditor)
{
	m_pEditor = pEditor;
}

QSize CEditorBPArea::sizeHint() const
{
	if (m_pEditor == 0)
		return QSize(0, 0);
	return m_pEditor->GetBPAreaDecorator()->GetWidgetSizeHint();
}

void CEditorBPArea::paintEvent(QPaintEvent* pEvent)
{
	m_pEditor->GetBPAreaDecorator()->DrawDecoration(pEvent, this);
}

void CEditorBPArea::mousePressEvent(QMouseEvent* pEvent)
{
	m_pEditor->GetBPAreaDecorator()->PressedAt(pEvent);
}