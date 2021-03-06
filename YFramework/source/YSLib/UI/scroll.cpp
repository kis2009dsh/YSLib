﻿/*
	© 2011-2013 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file scroll.cpp
\ingroup UI
\brief 样式相关的图形用户界面滚动控件。
\version r3627
\author FrankHB <frankhb1989@gmail.com>
\since build 194
\par 创建时间:
	2011-03-07 20:12:02 +0800
\par 修改时间:
	2013-10-21 17:06 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::UI::Scroll
*/


#include "YSLib/UI/scroll.h"
#include "YSLib/UI/ygui.h"
#include "YSLib/Core/ystorage.hpp"
#include <ystdex/algorithm.hpp>

using namespace ystdex;

namespace YSLib
{

namespace UI
{

namespace
{

pair<bool, bool>
FixScrollBarLayout(Size& d, const Size& s, SDst min_width, SDst min_height)
{
	bool need_h(d.Width < s.Width), need_v(d.Height < s.Height);

	if(need_h)
	{
		if(d.Height < min_height)
			throw GeneralEvent("Scroll bar need more height.");
		d.Height -= min_height;
	}
	if(need_v)
	{
		if(d.Width < min_width)
			throw GeneralEvent("Scroll bar need more width.");
		d.Width -= min_width;
	}
	if(need_h != need_v)
	{
		if(!need_h && d.Width < s.Width)
		{
			need_h = true;
			if(d.Height < min_height)
				throw GeneralEvent("Scroll bar need more height.");
			d.Height -= min_height;
		}
		if(!need_v && d.Height < s.Height)
		{
			need_v = true;
			if(d.Width < min_width)
				throw GeneralEvent("Scroll bar need more width.");
			d.Width -= min_width;
		}
	}
	return pair<bool, bool>(need_h, need_v);
}

const SDst defMinScrollBarWidth(16); //!< 默认最小滚动条宽。
const SDst defMinScrollBarHeight(16); //!< 默认最小滚动条高。

} //unnamed namespace;


ATrack::ATrack(const Rect& r, SDst uMinThumbLength)
	: Control({r.GetPoint(), max<SDst>(defMinScrollBarWidth, r.Width),
	max<SDst>(defMinScrollBarHeight, r.Height)}, NoBackgroundTag()),
	GMRange<ValueType>(0xFF, 0),
	tmbScroll(Size(defMinScrollBarWidth, defMinScrollBarHeight)),
	min_thumb_length(uMinThumbLength), large_delta(min_thumb_length)
{
	SetContainerPtrOf(tmbScroll, this);
	yunseq(
	Background = std::bind(DrawTrackBackground, std::placeholders::_1,
		std::ref(*this)),
	GetThumbDrag() += [this](UIEventArgs&&){
		LocateThumb(0, ScrollCategory::ThumbTrack);
	},
	FetchEvent<TouchHeld>(*this) += OnTouchHeld,
	FetchEvent<TouchDown>(*this) += [this](CursorEventArgs&& e){
		if(e.Strategy == RoutedEventArgs::Direct && &e.GetSender() == this
			&& Rect(GetSizeOf(*this)).Contains(e))
		{
			ScrollCategory t;

			switch(CheckArea(e.GetRef(IsHorizontal())))
			{
			case OnPrev:
				t = ScrollCategory::LargeDecrement;
				break;
			case OnNext:
				t = ScrollCategory::LargeIncrement;
				break;
			case None:
				return;
			default:
				t = ScrollCategory::EndScroll;
			}
			LocateThumb(0, t);
		}
	}
	);
}

void
ATrack::SetThumbLength(SDst l)
{
	RestrictInInterval(l, min_thumb_length, GetTrackLength());

	Size s(GetSizeOf(tmbScroll));
	const bool is_h(IsHorizontal());

	if(l != s.GetRef(is_h))
	{
		Invalidate(tmbScroll);
		s.GetRef(is_h) = l;
		SetSizeOf(tmbScroll, s);
	}
}
void
ATrack::SetThumbPosition(SPos pos)
{
	RestrictInClosedInterval<SPos>(pos, 0, GetScrollableLength());

	Point pt(GetLocationOf(tmbScroll));
	const bool is_h(IsHorizontal());

	if(pos != pt.GetRef(is_h))
	{
		Invalidate(tmbScroll);
		pt.GetRef(is_h) = pos;
		SetLocationOf(tmbScroll, pt);
	}
}
void
ATrack::SetMaxValue(ValueType m)
{
	if(m > 0)
	{
		if(large_delta > m)
			large_delta = m;
		max_value = m;
	}
}
void
ATrack::SetValue(ValueType val)
{
	value = val;
	SetThumbPosition(SPos(round(val * GetScrollableLength() / max_value)));
}
void
ATrack::SetLargeDelta(ValueType val)
{
	large_delta = val;
	SetThumbLength(SDst(round(val * GetTrackLength() / (val + max_value))));
}

ATrack::Area
ATrack::CheckArea(SPos q) const
{
	if(q >= 0)
	{
		yconstexpr Area lst[]{OnPrev, OnThumb, OnNext};
		const SPos a[]{SPos(), SPos(GetThumbPosition()),
			SPos(GetThumbPosition() + GetThumbLength())};
		const auto n(SwitchInterval(q, a, 3));

		if(n < 3)
			return lst[n];
	}
	return None;
}

void
ATrack::LocateThumb(ValueType val, ScrollCategory t)
{
	ValueType old_value(value);

	if(t == ScrollCategory::ThumbTrack)
		value = GetThumbPosition() == GetScrollableLength() ? max_value
			: max_value * GetThumbPosition()
			/ (GetTrackLength() - GetThumbLength());
	else
	{
		if(t == ScrollCategory::LargeDecrement
			|| t == ScrollCategory::LargeIncrement)
			val = GetLargeDelta();
		switch(t)
		{
		case ScrollCategory::SmallDecrement:
		case ScrollCategory::LargeDecrement:
			if(value > val)
			{
				SetValue(value - val);
				break;
			}
		case ScrollCategory::First:
			value = 0;
			SetThumbPosition(0);
			break;
		case ScrollCategory::SmallIncrement:
		case ScrollCategory::LargeIncrement:
			if(value + val < max_value)
			{
				SetValue(value + val);
				break;
			}
		case ScrollCategory::Last:
			value = max_value;
			SetThumbPosition(GetScrollableLength());
		default:
			;
		}
	}
	GetScroll()(ScrollEventArgs(*this, t, value, old_value));
}


void
DrawTrackBackground(PaintEventArgs&& e, ATrack& trk)
{
	const auto& g(e.Target);
	const auto& pt(e.Location);
	const Rect r(pt, GetSizeOf(trk));
	auto& pal(FetchGUIState().Colors);

	FillRect(g, r, pal[Styles::Track]);

#define YSL_UI_ATRACK_PARTIAL_INVALIDATION
	// NOTE: Partial invalidation made no efficiency improved here.
	const auto c(pal[Styles::Light]);
#ifdef YSL_UI_ATRACK_PARTIAL_INVALIDATION
	SPos x(pt.X);
	SPos y(pt.Y);
	SPos xr(x + trk.GetWidth());
	SPos yr(y + trk.GetHeight());
#else
	const SPos xr(pt.X + trk.GetWidth());
	const SPos yr(pt.Y + trk.GetHeight());
#endif

	if(trk.IsHorizontal())
	{
#ifdef YSL_UI_ATRACK_PARTIAL_INVALIDATION
		RestrictInInterval(y, r.Y, r.Y + r.Height),
		RestrictInInterval(yr, r.Y, r.Y + r.Height);
#endif
		DrawHLineSeg(g, r, pt.Y, pt.X, xr, c),
		DrawHLineSeg(g, r, yr, pt.X, xr, c);
	}
	else
	{
#ifdef YSL_UI_ATRACK_PARTIAL_INVALIDATION
		RestrictInInterval(x, r.X, r.X + r.Width),
		RestrictInInterval(xr, r.X, r.X + r.Width);
#endif
		DrawVLineSeg(g, r, pt.X, pt.Y, yr, c),
		DrawVLineSeg(g, r, xr, pt.Y, yr, c);
	}
}


HorizontalTrack::HorizontalTrack(const Rect& r, SDst uMinThumbLength)
	: ATrack(r, uMinThumbLength)
{
	YAssert(GetWidth() > GetHeight(), "Width is not greater than height.");

	FetchEvent<TouchHeld>(tmbScroll) += [this](CursorEventArgs&& e){
		if(e.Strategy == RoutedEventArgs::Direct)
		{
			auto& st(FetchGUIState());

			if(st.CheckDraggingOffset())
			{
				SPos x(st.CursorLocation.X + st.DraggingOffset.X);

				RestrictInClosedInterval<SPos>(x, 0,
					GetWidth() - tmbScroll.GetWidth());
				Invalidate(tmbScroll);
				SetLocationOf(tmbScroll, Point(x, GetLocationOf(tmbScroll).Y));
				GetThumbDrag()(UIEventArgs(*this));
			}
		}
	};
}


VerticalTrack::VerticalTrack(const Rect& r, SDst uMinThumbLength)
	: ATrack(r, uMinThumbLength)
{
	YAssert(GetHeight() > GetWidth(), "height is not greater than width.");

	FetchEvent<TouchHeld>(tmbScroll) += [this](CursorEventArgs&& e){
		if(e.Strategy == RoutedEventArgs::Direct)
		{
			auto& st(FetchGUIState());

			if(st.CheckDraggingOffset())
			{
				SPos y(st.CursorLocation.Y + st.DraggingOffset.Y);

				RestrictInClosedInterval<SPos>(y, 0,
					GetHeight() - tmbScroll.GetHeight());
				Invalidate(tmbScroll);
				SetLocationOf(tmbScroll, Point(GetLocationOf(tmbScroll).X, y));
				GetThumbDrag()(UIEventArgs(*this));
			}
		}
	};
}


AScrollBar::AScrollBar(const Rect& r, SDst uMinThumbSize, Orientation o)
	: Control(r, NoBackgroundTag()),
	pTrack(o == Horizontal
		? static_cast<ATrack*>(new HorizontalTrack(
			Rect(r.Height, 0, r.Width - r.Height * 2, r.Height), uMinThumbSize))
		: static_cast<ATrack*>(new VerticalTrack(
			Rect(0, r.Width, r.Width, r.Height - r.Width * 2), uMinThumbSize))),
	btnPrev(Rect()), btnNext(Rect()), small_delta(2)
{
	SetContainerPtrOf(*pTrack, this),
	SetContainerPtrOf(btnPrev, this);
	SetContainerPtrOf(btnNext, this);
	yunseq(
	FetchEvent<Resize>(*this) += [this](UIEventArgs&&){
		auto& track(GetTrack());
		const bool is_h(track.IsHorizontal());
		const SDst prev_metric(GetSizeOf(btnPrev).GetRef(is_h));
		const SDst sum(prev_metric + GetSizeOf(btnNext).GetRef(is_h));

		YAssert(GetSizeOf(*this).GetRef(is_h) - sum > 0,
			"No enough space for track.");

		const SDst tl(GetSizeOf(*this).GetRef(is_h) - sum);

		yunseq(track.GetView().GetSizeRef().GetRef(is_h) = tl,
			btnNext.GetView().GetLocationRef().GetRef(is_h) = tl + prev_metric);
		// NOTE: No event %(Resize, Move) is raised.
	},
	FetchEvent<KeyUp>(*this) += OnKey_Bound_TouchUp,
	FetchEvent<KeyDown>(*this) += OnKey_Bound_TouchDown,
	FetchEvent<KeyHeld>(*this) += OnKeyHeld,
	FetchEvent<CursorWheel>(*this) += [this](CursorWheelEventArgs&& e){
		if(e.GetDelta() > 0)
			LocateThumb(small_delta, ScrollCategory::SmallDecrement);
		else if(e.GetDelta() < 0)
			LocateThumb(small_delta, ScrollCategory::SmallIncrement);
	},
	FetchEvent<TouchHeld>(btnPrev) += OnTouchHeld,
	FetchEvent<TouchDown>(btnPrev) += [this](CursorEventArgs&&){
		LocateThumb(small_delta, ScrollCategory::SmallDecrement);
	},
	FetchEvent<TouchHeld>(btnNext) += OnTouchHeld,
	FetchEvent<TouchDown>(btnNext) += [this](CursorEventArgs&&){
		LocateThumb(small_delta, ScrollCategory::SmallIncrement);
	}
	);

	Size s(GetSizeOf(*this));
	const bool bHorizontal(o == Horizontal);
	const SDst l(s.GetRef(!bHorizontal));

	s.GetRef(bHorizontal) = l;
	SetSizeOf(btnPrev, s);
	SetSizeOf(btnNext, s);
//	Button.SetLocationOf(btnPrev, Point());
	MoveToBottom(btnNext);
	MoveToRight(btnNext);
}

void
AScrollBar::InitializeArrowPainters(Rotation prev, Rotation next)
{
	using namespace std;
	using namespace placeholders;

	yunseq(
	FetchEvent<Paint>(btnPrev) += bind(DrawArrow, _1, ref(btnPrev), 4, prev,
		ref(ForeColor)),
	FetchEvent<Paint>(btnNext) += bind(DrawArrow, _1, ref(btnNext), 4, next,
		ref(ForeColor))
	);
}


HorizontalScrollBar::HorizontalScrollBar(const Rect& r, SDst uMinThumbLength)
	: AScrollBar(r, uMinThumbLength, Horizontal)
{
	InitializeArrowPainters(RDeg180, RDeg0);
}

IWidget*
HorizontalScrollBar::GetBoundControlPtr(const KeyInput& k)
{
	if(k.count() == 1)
	{
		if(k[KeyCodes::Left])
			return &btnPrev;
		if(k[KeyCodes::Right])
			return &btnNext;
	}
	return nullptr;
}


VerticalScrollBar::VerticalScrollBar(const Rect& r, SDst uMinThumbLength)
	: AScrollBar(r, uMinThumbLength, Vertical)
{
	InitializeArrowPainters(RDeg90, RDeg270);
}

IWidget*
VerticalScrollBar::GetBoundControlPtr(const KeyInput& k)
{
	if(k.count() == 1)
	{
		if(k[KeyCodes::Up])
			return &btnPrev;
		if(k[KeyCodes::Down])
			return &btnNext;
	}
	return nullptr;
}


ScrollableContainer::ScrollableContainer(const Rect& r)
	: Control(r),
	hsbHorizontal(Size(r.Width, defMinScrollBarHeight)),
	vsbVertical(Size(defMinScrollBarWidth, r.Height))
{
	// TODO: Allow user to choose whether background is drawn.
	SetContainerPtrOf(hsbHorizontal, this),
	SetContainerPtrOf(vsbVertical, this),
	MoveToBottom(hsbHorizontal),
	MoveToRight(vsbVertical),
	FetchEvent<CursorWheel>(*this) += [this](CursorWheelEventArgs&& e){
		if(IsVisible(vsbVertical))
			CallEvent<CursorWheel>(vsbVertical, std::move(e));
	};
}

Size
ScrollableContainer::FixLayout(const Size& s)
{
	Size arena(GetSizeOf(*this));

	try
	{
		const pair<bool, bool> p(FixScrollBarLayout(arena, s,
			defMinScrollBarWidth, defMinScrollBarHeight));

		if(p.first && p.second && GetWidth() > defMinScrollBarWidth
			&& GetHeight() > defMinScrollBarHeight)
		{
			hsbHorizontal.SetWidth(GetWidth() - defMinScrollBarWidth);
			vsbVertical.SetHeight(GetHeight() - defMinScrollBarHeight);
		}
		else if(p.first)
		{
			SetSizeOf(hsbHorizontal, Size(GetWidth(),
				hsbHorizontal.GetHeight()));
			MoveToBottom(hsbHorizontal);
		}
		else if(p.second)
		{
			SetSizeOf(vsbVertical, Size(vsbVertical.GetWidth(),
				GetHeight()));
			MoveToRight(vsbVertical);
		}
		SetVisibleOf(hsbHorizontal, p.first);
		SetVisibleOf(vsbVertical, p.second);
	}
	catch(GeneralEvent&)
	{}
	return arena;
}

} // namespace UI;

} // namespace YSLib;

