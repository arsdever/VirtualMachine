#include "editor_num_area.h"
#include "editor_interfaces.h"
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
	QMap<void*, QSize> resultMap = CallFunctionAndReturn<IEditorNumberAreaImplementer>(IEditorNumberAreaImplementer::GetWidgetSizeHintFunctor());
	return *resultMap.begin();
}

void CEditorNumberArea::paintEvent(QPaintEvent* pEvent)
{
	CallFunction<IEditorNumberAreaImplementer>(IEditorNumberAreaImplementer::DrawDecorationFunctor(pEvent, this));
}