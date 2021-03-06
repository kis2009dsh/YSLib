﻿/*
	Copyright by FrankHB 2009 - 2013.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\defgroup YCLib YCommonLib
\brief YSLib 基础库。
*/

/*!	\file ycommon.h
\ingroup YCLib
\brief 平台相关的公共组件无关函数与宏定义集合。
\version r3485
\author FrankHB <frankhb1989@gmail.com>
\since 早于 build 132
\par 创建时间:
	2009-11-12 22:14:28 +0800
\par 修改时间:
	2013-08-05 20:14 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YCLib::YCommon
*/


#ifndef YCL_INC_ycommon_h_
#define YCL_INC_ycommon_h_ 1

//平台定义。
#include "Platform.h"

//平台中立部分。
#include <ydef.h>
#include <ystdex/cassert.h>
#include <ystdex/cstdio.h>
#include <cstdlib>
#include <string>
#include "YBaseMacro.h"

//平台相关部分。
//#include <unistd.h>

//! \brief 默认平台命名空间。
namespace platform
{

/*!
\brief 异常终止函数。
\since build 319
*/
YB_NORETURN YF_API void
terminate() ynothrow;


/*!
\brief 平台描述空间。
\since build 432
*/
namespace Discriptions
{

/*!
\brief 记录等级。
\since build 432
*/
enum RecordLevel : std::uint8_t
{
	Emergent = 0x00,
	Alert = 0x20,
	Critical = 0x40,
	Err = 0x60,
	Warning = 0x80,
	Notice = 0xA0,
	Informative = 0xC0,
	Debug = 0x70
};

} // namespace Discriptions;


#if YB_Use_YTrace
/*!
\brief YCLib 默认调试跟踪等级阈值。
\note 默认仅小于此等级的调试跟踪信息不忽略。
\since build 432
*/
#	ifndef YF_TraceLevel
#		define YF_TraceLevel platform::Discriptions::Informative
#	endif

/*!
\brief YCLib 默认调试跟踪。
\note 使用默认的调试跟踪级别。
\sa YF_TraceLevel
\sa ystdex::ytrace
*/
#	define YTraceDe(_lv, ...) \
	ystdex::ytrace(stderr, _lv, YF_TraceLevel, __FILE__, __LINE__, __VA_ARGS__)
#else
#	define YTraceDe(...)
#endif

} // namespace platform;

/*!
\brief 平台扩展命名空间。
\note 为便于移植，部分平台扩展接口保留至平台中立文件，依赖性按后者处理。
*/
namespace platform_ex
{

#if YCL_DS

/*!
\brief 设置允许设备进入睡眠的标识状态。
\return 旧状态。
\note 默认状态为 true 。
\warning 非线程安全。
\since build 278
*/
YF_API bool
AllowSleep(bool);

#elif YCL_MinGW32

#else
#	error "Unsupported platform found."
#endif

} // namespace platform_ex;

#endif

