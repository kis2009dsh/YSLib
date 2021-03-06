﻿/*
	© 2011-2013 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file uicontx.cpp
\ingroup UI
\brief 样式无关的 GUI 附加容器。
\version r244
\author FrankHB <frankhb1989@gmail.com>
\since build 192
\par 创建时间:
	2011-02-21 09:01:13 +0800
\par 修改时间:
	2013-10-17 20:50 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::UI::UIContainerEx
*/


#include "YSLib/UI/uicontx.h"
#include "YSLib/UI/ygui.h"
#include "YSLib/UI/Border.h"

namespace YSLib
{

namespace UI
{

DialogBox::DialogBox(const Rect& r)
	: Control(r),
	btnClose(Rect(GetWidth() - 20, 4, 16, 16), 330)
{
	DecorateAsCloseButton(btnClose),
	SetContainerPtrOf(btnClose, this),
	FetchEvent<Paint>(*this).Add(BorderBrush(), BackgroundPriority);
}


DialogPanel::DialogPanel(const Rect& r)
	: Panel(r),
	btnClose(Rect(GetWidth() - 20, 4, 16, 16), 330),
	btnOK(Rect(GetWidth() - 40, 4, 16, 16), 120)
{
	AddWidgets(*this, btnClose, btnOK),
	DecorateAsCloseButton(btnClose),
	yunseq(
	FetchEvent<Paint>(*this).Add(BorderBrush(), BackgroundPriority),
	FetchEvent<Click>(btnOK) += [this](CursorEventArgs&&){
		Close(*this);
	},
	FetchEvent<Paint>(btnOK) += [this](PaintEventArgs&& e){
		DrawCircle(e.Target, e.ClipArea, {e.Location.X + 8, e.Location.Y + 8},
			4, IsEnabled(btnOK) ? btnOK.ForeColor
			: FetchGUIState().Colors[Styles::Workspace]);
	}
	);
}

} // namespace UI;

} // namespace YSLib;

