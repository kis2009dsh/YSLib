﻿/*
	© 2009-2013 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file cstring.cpp
\ingroup YStandardEx
\brief ISO C 标准字符串扩展。
\version r1087
\author FrankHB <frankhb1989@gmail.com>
\since build 245
\par 创建时间:
	2009-12-27 17:31:20 +0800
\par 修改时间:
	2013-09-28 13:40 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YStandardEx::CString
*/


#include "ystdex/cstring.h"
#include <cstdio>

namespace ystdex
{

size_t
strlen_n(const char* s)
{
	return s ? std::strlen(s) : 0;
}

char*
strcpy_n(char* d, const char* s)
{
	return d && s ? std::strcpy(d, s) : nullptr;
}

char*
strcpycat(char* d, const char* s1, const char* s2)
{
	if(YB_UNLIKELY(!d))
		return nullptr;
	if(s1)
		std::strcpy(d, s1);
	if(s2)
		std::strcat(d, s2);
	return d;
}

char*
strcatdup(const char* s1, const char* s2, void*(*fun)(size_t))
{
	auto d(static_cast<char*>(
		fun((strlen(s1) + strlen(s2) + 1) * sizeof(char))));

	return strcpycat(d, s1, s2);
}

} // namespace ystdex;

