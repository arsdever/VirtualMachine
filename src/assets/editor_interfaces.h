#pragma once

#include "assets_global.h"
#include <core>
#include <QSize>

class QPaintEvent;
class QMouseEvent;
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

class ASSETS_EXPORT IEditorAddressAreaImplementer : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

	GENERATE_FUNCTOR_2(Assets, IEditorAddressAreaImplementer, DrawDecoration, QPaintEvent*, QWidget*)
		RET_GENERATE_FUNCTOR_0(Assets, QSize, IEditorAddressAreaImplementer, GetWidgetSizeHint)

protected:
	virtual void DrawDecoration(QPaintEvent* pEvent, QWidget* pWidget) const = 0;
	virtual QSize GetWidgetSizeHint() const = 0;
};

class ASSETS_EXPORT IEditorBPAreaImplementer : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

	GENERATE_FUNCTOR_2(Assets, IEditorBPAreaImplementer, DrawDecoration, QPaintEvent*, QWidget*)
	RET_GENERATE_FUNCTOR_0(Assets, QSize, IEditorBPAreaImplementer, GetWidgetSizeHint)
	GENERATE_FUNCTOR_1(Assets, IEditorBPAreaImplementer, PressedAt, QMouseEvent*)

protected:
	virtual void DrawDecoration(QPaintEvent* pEvent, QWidget* pWidget) const = 0;
	virtual QSize GetWidgetSizeHint() const = 0;
	virtual void PressedAt(QMouseEvent*) const = 0;
};