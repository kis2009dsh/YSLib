﻿/*
	Copyright by FrankHB 2010 - 2013.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file ShellHelper.cpp
\ingroup Helper
\brief Shell 助手模块。
\version r461
\author FrankHB <frankhb1989@gmail.com>
\since build 278
\par 创建时间:
	2010-04-04 13:42:15 +0800
\par 修改时间:
	2013-08-05 21:19 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	Helper::ShellHelper
*/


#include "Helper/ShellHelper.h"
#include <cstdio> // for std::sprintf;

namespace YSLib
{

#ifndef NDEBUG
DebugTimer::DebugTimer(const std::string& str)
	: event_info(str), base_tick()
{
	std::printf("Start tick of [%s] :\n", event_info.c_str());
	base_tick = Timers::HighResolutionClock::now();
}
DebugTimer::~DebugTimer()
{
	const double t((Timers::HighResolutionClock::now() - base_tick).count()
		/ 1e6);

	std::printf("Performed [%s] in: %f milliseconds.\n",
		event_info.c_str(), t);
}
#endif


void
RemoveGlobalTasks()
{
	auto& app(FetchGlobalInstance());

	app.Queue.Remove(app.UIResponseLimit);
}


namespace
{

//! \since build 264
inline void
snftime(char* buf, size_t /*n*/, const std::tm& tm,
	const char* format = DefaultTimeFormat)
{
	// FIXME: correct behavior for time with BC date(i.e. tm_year < -1900);
	///*std*/::snprintf(buf, n, format, tm.tm_year + 1900,
	//	tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	// FIXME: use std::snprintf;
	std::sprintf(buf, format, tm.tm_year + 1900,
		tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

} // unnamed namespace;


const char*
TranslateTime(const std::tm& tm, const char* format)
{
	static char str[80];

	// NOTE: 'std::strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm)'
	//	is correct but it makes the object file too large.
	snftime(str, 80, tm, format);
	return str;
}
const char*
TranslateTime(const std::time_t& t, const char* format) ythrow(GeneralEvent)
{
	auto p(std::localtime(&t));

	if(YB_UNLIKELY(!p))
		throw GeneralEvent("Get broken-down time object failed"
			" @ TranslateTime#2;");
	return TranslateTime(*p, format);
}


shared_ptr<UI::TextList::ListType>
FetchFontFamilyNames()
{
	const auto& mFamilies(FetchDefaultFontCache().GetFamilyIndices());

	return ystdex::make_shared<UI::TextList::ListType>(mFamilies.cbegin()
		| ystdex::get_key, mFamilies.cend() | ystdex::get_key);
}


FPSCounter::FPSCounter(std::chrono::nanoseconds s)
	: last_tick(GetHighResolutionTicks()), now_tick(), refresh_count(1),
	MinimalInterval(s)
{}

size_t
FPSCounter::Refresh()
{
	const std::chrono::nanoseconds tmp_tick(GetHighResolutionTicks());

	if(YB_UNLIKELY(last_tick + MinimalInterval < tmp_tick))
	{
		last_tick = now_tick;

		const u32 r(1000000000000ULL * refresh_count
			/ ((now_tick = tmp_tick) - last_tick).count());

		refresh_count = 1;
		return r;
	}
	else
		++refresh_count;
	return 0;
}

namespace UI
{

void
SwitchVisible(IWidget& wgt)
{
	SetVisibleOf(wgt, !IsVisible(wgt));
	Invalidate(wgt);
}

} // namespace UI;

} // namespace YSLib;

