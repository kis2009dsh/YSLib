﻿/*
	© 2010-2013 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file yexcept.cpp
\ingroup Core
\brief 异常处理模块。
\version r272
\author FrankHB <frankhb1989@gmail.com>
\since 早于 build 132
\par 创建时间:
	2010-06-15 20:30:14 +0800
\par 修改时间:
	2013-09-28 13:37 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::Core::YException
*/


#include "YSLib/Core/yexcept.h"

namespace YSLib
{

LoggedEvent::LoggedEvent(const std::string& s, LevelType l) ynothrow
	: GeneralEvent(s),
	level(l)
{}
LoggedEvent::LoggedEvent(const GeneralEvent& e, LevelType l) ynothrow
	: GeneralEvent(e),
	level(l)
{}


FatalError::FatalError(const char* t, const char* c) ynothrow
	: GeneralEvent(t), content(c)
{}

} // namespace YSLib;

