#pragma once

#include "assets_global.h"
#include <QWidget>

class QPaintEvent;
class CCodeEditor;

class CEditorBPArea : public QWidget
{
public:
	CEditorBPArea(QWidget* pParent = nullptr);
	~CEditorBPArea();

	void SetDrawerWidget(CCodeEditor* pEditor);
	QSize sizeHint() const override;

protected:
	void paintEvent(QPaintEvent* pEvent);
	void mousePressEvent(QMouseEvent* pEvent);

private:
	CCodeEditor* m_pEditor;
};

