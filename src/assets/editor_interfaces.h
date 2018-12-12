#pragma once

#include "assets_global.h"
#include <core>
#include <QSize>

class QPaintEvent;
class QWidget;

template <typename INTERFACE_TYPE, typename RETURN_TYPE = void>
struct ASSETS_EXPORT IAssetsFunctor : IFunctor<INTERFACE_TYPE, RETURN_TYPE> {};

class ASSETS_EXPORT IEditorNumberAreaImplementer : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

	GENERATE_FUNCTOR_2(Assets, IEditorNumberAreaImplementer, DrawDecoration, QPaintEvent*, QWidget*)
	RET_GENERATE_FUNCTOR_0(Assets, QSize, IEditorNumberAreaImplementer, GetWidgetSizeHint)

protected:
	virtual void DrawDecoration(QPaintEvent* pEvent, QWidget* pWidget) const = 0;
	virtual QSize GetWidgetSizeHint() const = 0;
};