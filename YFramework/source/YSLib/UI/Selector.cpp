﻿/*
	© 2011-2013 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file Selector.cpp
\ingroup UI
\brief 样式相关的图形用户界面选择控件。
\version r670
\author FrankHB <frankhb1989@gmail.com>
\since build 282
\par 创建时间:
	2011-03-22 07:20:06 +0800
\par 修改时间:
	2013-10-21 17:07 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::UI::Selector
*/


#include "YSLib/UI/Selector.h"
#include "YSLib/Service/yblit.h"
#include "YSLib/UI/ygui.h"

namespace YSLib
{

namespace UI
{

namespace
{

void
Diminish(Rect& r, SDst off1 = 1, SDst off2 = 2)
{
	YAssert(r.Width > 2 && r.Height > 2, "Boundary is too small.");

	yunseq(r.X += off1, r.Y += off1, r.Width -= off2, r.Height -= off2);
}

using namespace Drawing;

//! \since build 453
void
RectDrawCheckBox(const PaintContext& pc, const Size& s, bool is_pressed = {},
	bool is_locked = {}, bool is_ticked = {}, bool is_focused = {},
	Color c = Color(85, 184, 163))
{
	const auto& g(pc.Target);

	YAssert(bool(g), "Invalid graphics context found.");

	const auto& pt(pc.Location);
	const auto& bounds(pc.ClipArea);
	Rect r(pt, s);

	DrawRect(g, bounds, r, is_focused ? c : Color(85, 134, 223));
	if(YB_LIKELY(r.Width > 10 && r.Height > 10))
	{
		Rect rt(r);
		Color cs[]{{222, 249, 250}, {177, 223, 253}, {213, 254, 254}};
		// color3 gradient: 207, 236, 253;
	//	u16 h(rgb2hsl(Color2rgb(c)).h);

		if(!(is_locked || is_focused))
			for(auto& c : cs)
			{
				hsl_t tmp(ColorToHSL(c));

				tmp.s /= 4;
				c = HSLToColor(tmp);
			}
		Diminish(rt);
		DrawRect(g, bounds, rt, cs[0]);
		Diminish(rt);
		DrawRect(g, bounds, rt, cs[1]);
		Diminish(rt);
		FillRect(g, bounds, rt, cs[2]);
	}
	if(is_ticked)
	{
		const Color c1(4, 34, 113), c2(108, 166, 208);
		Point p1(r.X + 2, r.Y + r.Height / 2), p2(r.X + r.Width / 2 - 1,
			r.Y + r.Height - 3), p3(r.X + r.Width - 2, r.Y + 1);

		p2 += Vec(0, -1);
		DrawLineSeg(g, bounds, p1 + Vec(1, 0), p2, c2);
		DrawLineSeg(g, bounds, p2, p3 + Vec(-1, 0), c2);
		p2 += Vec(0, 2);
		DrawLineSeg(g, bounds, p1 + Vec(0, 1), p2, c2);
		DrawLineSeg(g, bounds, p2, p3 + Vec(0, 1), c2);
		p2 += Vec(0, -1);
		DrawLineSeg(g, bounds, p1, p2, c1);
		DrawLineSeg(g, bounds, p2, p3, c1);
	}
	if(is_pressed)
		TransformRect(g, bounds & r, transform_pixel_ex<56, 24, 32>);
}

} // unnamed namespace;


CheckBox::CheckBox(const Rect& r)
	: Thumb(r, NoBackgroundTag()),
	bTicked(false)
{
	FetchEvent<Click>(*this) += [this](CursorEventArgs&&){
		bTicked = !bTicked;
		Ticked(TickedArgs(*this, bTicked));
	};
}

void
CheckBox::SetTicked(bool b)
{
	const bool old_tick(bTicked);

	bTicked = b;
	if(bTicked != old_tick)
		Ticked(TickedArgs(*this, b));
}

void
CheckBox::Tick(bool b)
{
	Ticked(TickedArgs(*this, bTicked = b));
}

void
CheckBox::PaintBox(const PaintContext& pc, const Size& s)
{
	RectDrawCheckBox(pc, s, bPressed, IsFocusedByShell(*this), bTicked,
		IsFocused(*this));
}

void
CheckBox::Refresh(PaintEventArgs&& e)
{
	const auto& s(GetSizeOf(*this));

	PaintBox(e, s);
}


CheckButton::CheckButton(const Rect& r)
	: CheckBox(r)
{
	Margin.Top = 0;
}

void
CheckButton::Refresh(PaintEventArgs&& e)
{
	const auto& s(GetSizeOf(*this));

	PaintBox(e, {13, 13});
	Margin.Left += 13;
	DrawText(s, ForeColor, e);
	Margin.Left -= 13;
}

} // namespace UI;

} // namespace YSLib;

