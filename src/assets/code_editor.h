#pragma once

#include "editor_interfaces.h"
#include "assets_global.h"
#include <QPlainTextEdit>
#include "editor_num_area.h"
#include "editor_addr_area.h"
#include "editor_bp_area.h"

class QResizeEvent;
class QPaintEvent;
class QDragEnterEvent;
class QDropEvent;

class ASSETS_EXPORT CCodeEditor : public QPlainTextEdit
{

	Q_OBJECT

	IMPLEMENT_BEGIN(CCodeEditor, Debugger)
		virtual void SetBreakpoint(quint32) override;
		virtual void UnsetBreakpoint(quint32) override;
		virtual void ToggleBreakpoint(quint32) override;
		virtual void SetRunningAddress(quint32) override;
		virtual void ClearBreakpoints() override;
	IMPLEMENT_END(Debugger)

	IMPLEMENT_BEGIN(CCodeEditor, EditorNumberAreaImplementer)
		virtual void DrawDecoration(QPaintEvent* pEvent, QWidget* pWidget) const override;
		virtual QSize GetWidgetSizeHint() const override;
	IMPLEMENT_END(EditorNumberAreaImplementer)

	IMPLEMENT_BEGIN(CCodeEditor, EditorAddressAreaImplementer)
		virtual void DrawDecoration(QPaintEvent* pEvent, QWidget* pWidget) const override;
		virtual QSize GetWidgetSizeHint() const override;
	IMPLEMENT_END(EditorAddressAreaImplementer)

	IMPLEMENT_BEGIN(CCodeEditor, EditorBPAreaImplementer)
		virtual void DrawDecoration(QPaintEvent* pEvent, QWidget* pWidget) const override;
		virtual QSize GetWidgetSizeHint() const override;
		virtual void PressedAt(QMouseEvent*) const override;
	IMPLEMENT_END(EditorBPAreaImplementer)

public:
	CCodeEditor();
	~CCodeEditor();
	const IEditorNumberAreaImplementer* GetNumberAreaDecorator() const { return &m_xEditorNumberAreaImplementer; }
	const IEditorAddressAreaImplementer* GetAddressAreaDecorator() const { return &m_xEditorAddressAreaImplementer; }
	const IEditorBPAreaImplementer* GetBPAreaDecorator() const { return &m_xEditorBPAreaImplementer; }

protected:
	void keyPressEvent(QKeyEvent* event) override;

private slots:
	void updateAreaWidth(int);
	void updateAreas(QRect const&, int);
	void resizeEvent(QResizeEvent* pEvent) override;

private:
	QSize GetDecorationsSize() const { return m_pAddressArea->sizeHint() + m_pBPArea->sizeHint() + m_pNumberArea->sizeHint(); }

	CEditorNumberArea* m_pNumberArea;
	CEditorAddressArea* m_pAddressArea;
	CEditorBPArea* m_pBPArea;
	QList<quint32> m_lstBreakpoints;
	quint32 m_nCurrentAddress;
};

