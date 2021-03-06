﻿/*
	Copyright by FrankHB 2009 - 2013.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\defgroup Core Core
\ingroup YSLib
\brief YSLib 核心模块。
*/

/*!	\file ysdef.h
\ingroup Core
\brief 宏定义和类型描述。
\version r1580
\author FrankHB <frankhb1989@gmail.com>
\since 早于 build 132
\par 创建时间:
	2009-12-24 15:29:11 +0800
\par 修改时间:
	2013-08-05 20:57 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::Core::YShellDefinition
*/


#ifndef YSL_INC_Core_ysder_h_
#define YSL_INC_Core_ysder_h_ 1

//适配器模块。
#include "../Adaptor/yadaptor.h"

//间接访问类模块。
#include "../Adaptor/yref.hpp"

namespace YSLib
{

//前向声明。

namespace Shells
{
class Shell;
} // namespace Shells;

namespace Text
{
class String;
} // namespace Text;

class Application;

using Shells::Shell;

using Text::String;


//平台无关的类型定义。
//


/*!	\defgroup GlobalObjects Global Objects
\brief 全局对象。
*/

/*!	\defgroup DefaultGlobalConstants Default Global Constants
\ingroup GlobalObjects
\brief 默认全局常量。
*/
//@{
//@}

/*!	\defgroup DefaultGlobalVariables Default Global Variables
\ingroup GlobalObjects
\brief 默认全局变量。
*/
//@{
//@}

/*!	\defgroup DefaultGlobalObjectMapping Default Global Object Mapping
\ingroup GlobalObjects
\brief 默认全局变量映射。

访问全局程序实例对象。
*/
//@{
//@}

} // namespace YSLib;

#endif

