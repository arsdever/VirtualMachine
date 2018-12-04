#pragma once

#include "assets_global.h"
#include <QWidget>

class QPaintEvent;
class CCodeEditor;

class CEditorAddressArea : public QWidget
{
public:
	CEditorAddressArea(QWidget* pParent = nullptr);
	~CEditorAddressArea();

	void SetDrawerWidget(CCodeEditor* pEditor);
	QSize sizeHint() const override;

protected:
	void paintEvent(QPaintEvent* pEvent);

private:
	CCodeEditor* m_pEditor;
};

