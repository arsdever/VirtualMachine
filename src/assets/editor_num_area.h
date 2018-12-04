#pragma once

#include "assets_global.h"
#include <QWidget>

class QPaintEvent;
class CCodeEditor;

class CEditorNumberArea : public QWidget
{
public:
	CEditorNumberArea(QWidget* pParent = nullptr);
	~CEditorNumberArea();

	void SetDrawerWidget(CCodeEditor* pEditor);
	QSize sizeHint() const override;

protected:
	void paintEvent(QPaintEvent* pEvent);

private:
	CCodeEditor* m_pEditor;
};

