﻿/*
	Copyright by FrankHB 2013.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file HostedUI.cpp
\ingroup Helper
\brief 宿主环境支持的用户界面。
\version r152
\author FrankHB <frankhb1989@gmail.com>
\since build 389
\par 创建时间:
	2013-03-17 10:22:36 +0800
\par 修改时间:
	2013-09-14 03:21 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	Helper::HostedUI
*/


#include "Helper/HostedUI.h"
#include "Helper/HostRenderer.h"
#include <YSLib/UI/ycontrol.h> // for UI::FetchEvent;
#include <YSLib/UI/ygui.h> // for FetchGUIState;

namespace YSLib
{

using namespace Drawing;
using namespace UI;

#if YCL_HOSTED
namespace Host
{

Window&
WaitForHostWindow(UI::IWidget& wgt)
{
	auto& renderer(dynamic_cast<HostRenderer&>(wgt.GetRenderer()));
	Host::Window* p_wnd{};

	while(!p_wnd)
		p_wnd = renderer.GetWindowPtr();
	return *p_wnd;
}


void
DragWindow(Window& wnd, UI::CursorEventArgs&& e)
{
	if(e.Strategy == RoutedEventArgs::Direct && !e.Handled)
	{
		auto& st(FetchGUIState());

		if(st.CheckDraggingOffset())
		{
			const auto offset(st.CursorLocation + st.DraggingOffset);

			wnd.Move(wnd.GetLocation() + offset);
			st.CursorLocation -= offset - GetLocationOf(e.GetSender());
		}
	}
}

#	if YCL_MinGW32

void
ShowTopLevel(UI::Widget& wgt, ::DWORD wstyle, ::DWORD wstyle_ex,
	const wchar_t* title)
{
	WrapRenderer(wgt, CreateNativeWindow, WindowClassName, GetSizeOf(wgt),
		title, wstyle, wstyle_ex);

	WaitForHostWindow(wgt);
}
#	endif

void
ShowTopLevelDraggable(UI::Widget& wgt)
{
#	if YCL_MinGW32
	ShowTopLevel(wgt, WS_POPUP);
#else
#	error "Currently only Windows is supported."
#endif
	UI::FetchEvent<UI::TouchHeld>(wgt) += std::bind(Host::DragWindow,
		std::ref(WaitForHostWindow(wgt)), std::placeholders::_1);
}

} // namespace Host;
#endif

} // namespace YSLib;

