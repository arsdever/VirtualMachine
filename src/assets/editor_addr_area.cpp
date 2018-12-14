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
	QMap<void*, QSize> resultMap = CallFunctionAndReturn<IEditorAddressAreaImplementer>(IEditorAddressAreaImplementer::GetWidgetSizeHintFunctor());
	return *resultMap.begin();
}

void CEditorAddressArea::paintEvent(QPaintEvent* pEvent)
{
	CallFunction<IEditorAddressAreaImplementer>(IEditorAddressAreaImplementer::DrawDecorationFunctor(pEvent, this));
}