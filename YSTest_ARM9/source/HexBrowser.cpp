﻿/*
	Copyright by FrankHB 2011-2013.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file HexBrowser.cpp
\ingroup YReader
\brief 十六进制浏览器。
\version r570
\author FrankHB <frankhb1989@gmail.com>
\since build 253
\par 创建时间:
	2011-10-14 18:12:20 +0800
\par 修改时间:
	2013-09-07 02:38 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YReader::HexBrowser
*/


#include "HexBrowser.h"

namespace YSLib
{

using namespace Drawing;
using namespace Text;

namespace UI
{

HexView::HexView(FontCache& fc)
	: TextState(fc), item_num(0), datCurrent()
{
	TextState.Color = ColorSpace::Black;
}


HexViewArea::HexViewArea(const Rect& r, FontCache& fc)
	: ScrollableContainer(r), HexView(fc),
	model()
{
	SetVisibleOf(hsbHorizontal, false);
	SetVisibleOf(vsbVertical, true);
	vsbVertical.SetSmallDelta(1);
	yunseq(
	vsbVertical.GetTrack().GetScroll() += [this](ScrollEventArgs&& e){
		LocateViewPosition(round(e.GetValue()));
	},
	FetchEvent<KeyDown>(*this) += [this](KeyEventArgs&& e){
		using namespace KeyCodes;

		const auto& k(e.GetKeys());

		if(k.count() != 1)
			return;

		ScrollCategory t(ScrollCategory::SmallDecrement);

		if(k[Down])
			t = ScrollCategory::SmallIncrement;
		else if(k[PgUp])
			t = ScrollCategory::LargeDecrement;
		else if(k[PgDn])
			t = ScrollCategory::LargeIncrement;
		else if(!k[Up])
			return;
		vsbVertical.LocateThumb(k[Up] || k[Down] ? vsbVertical.GetSmallDelta()
			: vsbVertical.GetLargeDelta(), t);
		RequestFocus(*this);
		e.Handled = true;
	},
	FetchEvent<KeyHeld>(*this) += OnKeyHeld
	);
	Reset();
}

void
HexViewArea::Load(const char* path)
{
	Reset();
	model = make_unique<File>(path);

	const IndexType n_total_ln((model.GetSize() + ItemPerLine - 1)
		/ ItemPerLine);

	if(n_total_ln > GetItemNum())
	{
		vsbVertical.SetMaxValue(n_total_ln - GetItemNum());
		vsbVertical.SetLargeDelta(GetItemNum());
	}
	else
		SetVisibleOf(vsbVertical, false);
}

void
HexViewArea::LocateViewPosition(u32 line)
{
	UpdateData(ItemPerLine * line);
	UpdateView(true);
}

void
HexViewArea::Refresh(PaintEventArgs&& e)
{
	using namespace Text;

	// TODO: Refresh for 'rect' properly.
//	Background(PaintEventArgs(*this, e.Target, e.Location, Rect(e.Location,
//		GetWidth(), GetHeight())));
//	Background(PaintEventArgs(*this, e));
	ScrollableContainer::Refresh(std::move(e));
	TextState.ResetPen();

	// NOTE: It seems there is a bug in linker for checking odr-using.
	yconstexpr auto ItemPerLine(HexViewArea::ItemPerLine);
	auto& y(TextState.Pen.Y);
	const SDst lh(GetItemHeight()), h(GetHeight()),
		w_all(GetWidth() - vsbVertical.GetWidth()
			- GetHorizontalOf(TextState.Margin)),
		w_blank(w_all / (10 + ItemPerLine * 3)),
		w_ch((w_all - w_blank * (1 + ItemPerLine)) / (8 + ItemPerLine * 2)),
		w_addr(w_ch * 8 + w_blank),
		w_item(w_ch * 2 + w_blank);
	const int fsize(model.GetSize());
	auto& pen_x(TextState.Pen.X);
	TextRenderer tr(TextState, e.Target);
	auto pos(model.GetPosition());
	auto i_data(datCurrent.begin());

	while(y < h && pos < fsize && i_data < datCurrent.end())
	{
		pen_x = TextState.Margin.Left;

		auto x(pen_x);

		{
			char straddr[(32 >> 2) + 1];

			std::sprintf(straddr, "%08X", pos);
			PutLine(tr, straddr);
		}
		x += w_addr;

		const auto n(min<IndexType>(fsize - pos, ItemPerLine));

		for(IndexType j(0); j < n; yunseq(++j, i_data += 2, x += w_item))
		{
			pen_x = x;
			PutLine(tr, &*i_data, &*i_data + 2);
		}
		yunseq(y += lh + TextState.LineGap, pos += ItemPerLine);
	}
}

void
HexViewArea::Reset()
{
	vsbVertical.SetValue(0);
	datCurrent.clear();
	UpdateItemNum(GetHeight());
	UpdateView();
}

void
HexViewArea::UpdateData(u32 pos)
{
	if(model.IsValid() && pos < model.GetSize())
	{
		const DataType::size_type n(ItemPerLine * GetItemNum() * 2);

		model.SetPosition(pos, SEEK_SET);
		datCurrent.resize(n);

		auto b(datCurrent.begin());
		const auto e(datCurrent.end());

		while(!model.CheckEOF() && b != e)
		{
			byte c(std::fgetc(model.GetPtr()));
			char h, l;

			yunseq(h = (c >> 4 & 0x0F) + '0', l = (c & 0x0F) + '0');
			*b++ = h > '9' ? h + 'A' - '9' - 1 : h;
			*b++ = l > '9' ? l + 'A' - '9' - 1 : l;
		}
	//	vsbVertical.SetValue(pos / ItemPerLine);
		datCurrent.resize(b - datCurrent.begin());
		model.SetPosition(pos, SEEK_SET); // Refresh 需要据此判断接近文件结尾。
	}
}

void
HexViewArea::UpdateView(bool is_active)
{
	ViewChanged(ViewArgs(*this, is_active));
	Invalidate(*this);
}

} // namespace UI;

} // namespace YSLib;

