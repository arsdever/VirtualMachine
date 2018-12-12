#pragma once

#include "editor_interfaces.h"
#include "assets_global.h"
#include <QPlainTextEdit>
#include "editor_num_area.h"
#include "editor_addr_area.h"
#include "editor_bp_area.h"

class QResizeEvent;
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

private:
	class CEditorAddressAreaImplementer
	{
		CEditorAddressAreaImplementer()
		{
			m_pThis = (CCodeEditor*)((unsigned long long)this - (unsigned long long)(&((CCodeEditor*)0)->m_xEditorAddressAreaImplementer));
		}
		friend class CEditorAddressArea;
		friend class CCodeEditor;

		void DrawDecoration(QPaintEvent* pEvent, QWidget* pWidget) const;
		QSize GetWidgetSizeHint() const;
		CCodeEditor* m_pThis;
	} m_xEditorAddressAreaImplementer;

	class CEditorBPAreaImplementer
	{
		CEditorBPAreaImplementer()
		{
			m_pThis = (CCodeEditor*)((unsigned long long)this - (unsigned long long)(&((CCodeEditor*)0)->m_xEditorBPAreaImplementer));
		}
		friend class CEditorBPArea;
		friend class CCodeEditor;

		void DrawDecoration(QPaintEvent* pEvent, QWidget* pWidget) const;
		QSize GetWidgetSizeHint() const;
		void PressedAt(QMouseEvent* pEvent) const;
		CCodeEditor* m_pThis;
	} m_xEditorBPAreaImplementer;

public:
	CCodeEditor();
	~CCodeEditor();
	const IEditorNumberAreaImplementer* GetNumberAreaDecorator() const { return &m_xEditorNumberAreaImplementer; }
	const CEditorAddressAreaImplementer* GetAddressAreaDecorator() const { return &m_xEditorAddressAreaImplementer; }
	const CEditorBPAreaImplementer* GetBPAreaDecorator() const { return &m_xEditorBPAreaImplementer; }

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

