﻿/*
	Copyright by FrankHB 2011-2013.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file scroll.h
\ingroup UI
\brief 样式相关的图形用户界面滚动控件。
\version r2635
\author FrankHB <frankhb1989@gmail.com>
\since build 194
\par 创建时间:
	2011-03-07 20:10:35 +0800
\par 修改时间:
	2013-09-07 02:44 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::UI::Scroll
*/


#ifndef YSL_INC_UI_scroll_h_
#define YSL_INC_UI_scroll_h_ 1

#include "button.h"
#include "yuicont.h"
#include "WidgetIteration.h"
//#include <ystdex/rational.hpp>
//#include "../Core/yres.h"
//#include "ystyle.h"

namespace YSLib
{

namespace UI
{

/*!
\brief 滚动类别。
\since build 261
*/
enum class ScrollCategory
{
	SmallDecrement = 0, //!< 滚动框小距离减量移动。
	SmallIncrement = 1, //!< 滚动框小距离增量移动。
	LargeDecrement = 2, //!< 滚动框大距离减量移动。
	LargeIncrement = 3, //!< 滚动框大距离增量移动。
	ThumbPosition = 4, //!< 滚动框定位（通过直接设置位置）。
	ThumbTrack = 5, //!< 滚动框当前正在移动。
	First = 6, //!< 滚动框移动至最小位置。
	Last = 7, //!< 滚动框移动至最大位置。
	EndScroll = 8 //!< 滚动框移动停止。
};


/*!
\brief 滚动事件参数类。
\since build 193
*/
struct YF_API ScrollEventArgs : public UIEventArgs, protected pair<float, float>
{
public:
	using ValueType = float; //!< 值类型。

	/*!
	\brief 滚动事件类别。
	\since build 271
	*/
	ScrollCategory Category;

	/*!
	\brief 构造：使用指定事件源、滚动事件类型和值。
	\note 值等于旧值。
	*/
	ScrollEventArgs(IWidget& wgt, ScrollCategory t, ValueType val)
		: UIEventArgs(wgt), pair<float, float>(val, float()),
		Category(t)
	{}
	/*!
	\brief 构造：使用指定事件源、滚动事件类型、值和旧值。
	*/
	ScrollEventArgs(IWidget& wgt, ScrollCategory t, ValueType val,
		ValueType old_val)
		: UIEventArgs(wgt), pair<float, float>(val, old_val),
		Category(t)
	{}

	/*!
	\brief 取值。
	\since build 271
	*/
	DefGetter(const ynothrow, ValueType, Value, first)
	/*!
	\brief 取旧值。
	\since build 271
	*/
	DefGetter(const ynothrow, ValueType, OldValue, second)

	/*!
	\brief 设置值。
	\since build 271
	*/
	DefSetter(ValueType, Value, first)
	/*!
	\brief 设置旧值。
	\since build 271
	*/
	DefSetter(ValueType, OldValue, first)
};

DeclDelegate(HScrollEvent, ScrollEventArgs)


/*!
\brief 轨道。
\since build 167
*/
class YF_API ATrack : public Control, public GMRange<ScrollEventArgs::ValueType>
{
public:
	/*!
	\brief 轨道区域。
	\since build 416
	*/
	enum Area
	{
		None = 0,
		OnThumb = 1,
		OnPrev = 2,
		OnNext = 3
	};
	//注意值类型需要和继承的 GMRange 的 ValueType 一致。
	using ValueType = ScrollEventArgs::ValueType; //!< 值类型。
	//! \since build 356
	using Iterator = ystdex::subscriptive_iterator<ATrack, IWidget>;

protected:
	/*!
	\brief 滑块：轨道区域上的滚动框。
	\since build 374
	*/
	Thumb tmbScroll;

private:
	SDst min_thumb_length; //!< 最小滑块长度。
	// MRange 实现滚动事件关联值操作。
	ValueType large_delta; \
		//!< 大距离滚动偏移量：滚动事件关联的滑块位置变化绝对值。
	// MRange::value 实际最大取值为 MRange::max_value - large_delta 。
	DeclEvent(HUIEvent, ThumbDrag) //!< 滑块拖动事件。
	DeclEvent(HScrollEvent, Scroll) //!< 滚动事件。

public:
	/*!
	\brief 构造：使用指定边界和大小。
	\since build 337
	*/
	explicit
	ATrack(const Rect& = {}, SDst = 8);
	inline DefDeMoveCtor(ATrack)

	DefPred(const ynothrow, Horizontal, GetOrientation() == Horizontal)
	DefPred(const ynothrow, Vertical, GetOrientation() == Vertical)

	//! \since build 356
	DefWidgetMemberIteration(tmbScroll)

	DefEventGetter(ynothrow, HUIEvent, ThumbDrag, ThumbDrag) //!< 滑块拖动事件。
	DefEventGetter(ynothrow, HScrollEvent, Scroll, Scroll) //!< 滚动事件。
	DefGetter(const ynothrow, SDst, MinThumbLength, min_thumb_length)
	DeclIEntry(Orientation GetOrientation() const) //!< 取轨道方向。
	DefGetter(const ynothrow, SDst, ScrollableLength,
		GetTrackLength() - GetThumbLength()) //!< 取可滚动区域长度。
	DefGetter(const ynothrow, SDst, ThumbLength,
		GetSizeOf(tmbScroll).GetRef(IsHorizontal())) //!< 取轨道方向上的滑块长度。
	DefGetter(const ynothrow, SDst, ThumbPosition,
		GetLocationOf(tmbScroll).GetRef(IsHorizontal())) //!< 取滑块位置。
	virtual DefGetter(const ynothrow, SDst, TrackLength,
		GetSizeOf(*this).GetRef(IsHorizontal())) //!< 取轨道方向上的轨道长度。
	DefGetter(const ynothrow, ValueType, LargeDelta, large_delta)

	/*!
	\brief 设置轨道方向上的滑块长度。
	*/
	virtual void
	SetThumbLength(SDst);
	/*!
	\brief 设置滑块位置。
	*/
	void
	SetThumbPosition(SPos);
	/*!
	\brief 设置滚动事件关联值最大取值。
	\note 当指定值非正值时无效。
	\note 约束 large_delta 不大于指定值。
	*/
	void
	SetMaxValue(ValueType);
	/*!
	\brief 设置滚动事件关联值。
	\note 同步按滑块长度和最大取值的比例设置滑块位置。
	*/
	void
	SetValue(ValueType);
	/*!
	\brief 设置大距离滚动偏移量。
	\note 同步按滑块长度和最大取值的比例设置滑块位置。
	*/
	void
	SetLargeDelta(ValueType);

protected:
	/*!
	\brief 检查轨道方向指定位置所在的区域。
	*/
	Area
	CheckArea(SPos) const;

public:
	/*!
	\brief 定位滑块。
	\note 指定滚动事件关联值设置滑块位置并触发对应事件。
	\note 当滚动类别为 <tt>ScrollCategory::ThumbPosition</tt> 值参数为设置指定值；
		当滚动类别为 <tt>ScrollCategory::SmallDecrement</tt>
		或 <tt>ScrollCategory::SmallIncrement</tt> 时参数为值变化的绝对值；
		其它情况无效。
	*/
	void
	LocateThumb(ValueType, ScrollCategory = ScrollCategory::ThumbPosition);
};


/*!
\brief 绘制指定色调的基本按钮背景。
\since build 359
*/
YF_API void
DrawTrackBackground(PaintEventArgs&& e, ATrack&);


/*!
\brief 水平轨道。
\since build 205
*/
class YF_API HorizontalTrack : public ATrack
{
public:
	/*!
	\brief 构造：使用指定边界和最小滑块长。
	\pre 断言：宽大于长。
	\since build 337
	*/
	explicit
	HorizontalTrack(const Rect& = {}, SDst = 8);
	inline DefDeMoveCtor(HorizontalTrack)

	ImplI(ATrack)
	DefGetter(const ynothrow, Orientation, Orientation, Horizontal)
};


/*!
\brief 竖直轨道。
\since build 205
*/
class YF_API VerticalTrack : public ATrack
{
public:
	/*!
	\brief 构造：使用指定边界和最小滑块长。
	\pre 断言：长大于宽。
	\since build 337
	*/
	explicit
	VerticalTrack(const Rect& = {}, SDst = 8);
	inline DefDeMoveCtor(VerticalTrack)

	ImplI(ATrack)
	DefGetter(const ynothrow, Orientation, Orientation, Vertical)
};


/*!
\brief 滚动条。
\since build 162
*/
class YF_API AScrollBar : public Control
{
public:
	using ValueType = ATrack::ValueType; //!< 值类型。
	//! \since build 356
	using Iterator = ystdex::subscriptive_iterator<AScrollBar, IWidget>;

private:
	unique_ptr<ATrack> pTrack; //轨道。

protected:
	/*!
	\brief 前滚动条按钮。
	\note 可响应按键持续。
	\since build 276
	*/
	Thumb btnPrev;
	/*!
	\brief 后滚动条按钮。
	\note 可响应按键持续。
	\since build 276
	*/
	Thumb btnNext;

private:
	ValueType small_delta; \
		//!< 小距离滚动偏移量：滚动事件关联的滑块位置变化绝对值。

public:
	/*!
	\brief 构造：使用指定边界、大小和方向。
	\since build 337
	*/
	explicit
	AScrollBar(const Rect& = {}, SDst = 8, Orientation = Horizontal);
	inline DefDeMoveCtor(AScrollBar)

protected:
	//! \since build 443
	void
	InitializeArrowPainters(Rotation, Rotation);

public:
	//! \since build 356
	DefWidgetMemberIteration(btnPrev, btnNext, *pTrack)

	/*!
	\brief 取轨道引用。
	\note 断言： bool(pTrack) 。
	*/
	ATrack&
	GetTrack() const ynothrow
	{
		YAssert(bool(pTrack), "Null pointer found.");

		return *pTrack;
	}
	DefGetterMem(const ynothrow, ValueType, MaxValue, GetTrack())
	DefGetterMem(const ynothrow, ValueType, Value, GetTrack())
	DefGetterMem(const ynothrow, ValueType, LargeDelta, GetTrack())
	DefGetter(const ynothrow, ValueType, SmallDelta, small_delta)

	DefSetterMem(ValueType, MaxValue, GetTrack())
	DefSetterMem(ValueType, Value, GetTrack())
	DefSetterMem(ValueType, LargeDelta, GetTrack())
	DefSetter(ValueType, SmallDelta, small_delta)

	/*!
	\brief 定位滑块。
	\note 调用 GetTrack() 的同名函数指定滚动事件关联值设置滑块位置并触发对应事件。
	*/
	void
	LocateThumb(ValueType val,
		ScrollCategory t = ScrollCategory::ThumbPosition)
	{
		return GetTrack().LocateThumb(val, t);
	}
};


/*!
\brief 水平滚动条。
\since build 205
*/
class YF_API HorizontalScrollBar : public AScrollBar
{
public:
	//! \since build 337
	explicit
	HorizontalScrollBar(const Rect& = {}, SDst = 8);
	inline DefDeMoveCtor(HorizontalScrollBar)

public:
	ImplI(ATrack) DefGetter(const ynothrow, Orientation, Orientation,
		Horizontal)

protected:
	IWidget*
	GetBoundControlPtr(const KeyInput&) override;
};


/*!
\brief 竖直滚动条。
\since build 205
*/
class YF_API VerticalScrollBar : public AScrollBar
{
public:
	//! \since build 337
	explicit
	VerticalScrollBar(const Rect& = {}, SDst = 8);
	inline DefDeMoveCtor(VerticalScrollBar)

public:
	ImplI(ATrack) DefGetter(const ynothrow, Orientation, Orientation, Vertical)

protected:
	IWidget*
	GetBoundControlPtr(const KeyInput&) override;
};


/*!
\brief 带滚动条的容器。
\since build 192
*/
class YF_API ScrollableContainer : public Control
{
public:
	//! \since build 356
	using Iterator
		= ystdex::subscriptive_iterator<ScrollableContainer, IWidget>;

protected:
	/*!
	\brief 控制水平可视区域的水平滚动条。
	\since build 315
	*/
	HorizontalScrollBar hsbHorizontal;
	/*!
	\brief 控制竖直可视区域的竖直滚动条。
	\since build 315
	*/
	VerticalScrollBar vsbVertical;

public:
	//! \since build 337
	explicit
	ScrollableContainer(const Rect& = {});
	inline DefDeMoveCtor(ScrollableContainer)

	//! \since build 356
	DefWidgetMemberIteration(hsbHorizontal, vsbVertical)

protected:
	/*!
	\brief 固定布局。
	\return 调整后的视图大小。
	*/
	Size
	FixLayout(const Size&);
};

} // namespace UI;

} // namespace YSLib;

#endif

