﻿/*
	Copyright (C) by Franksoft 2012.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file YBrush.h
\ingroup UI
\brief 图形用户界面画刷。
\version r1218;
\author FrankHB<frankhb1989@gmail.com>
\since build 293 。
\par 创建时间:
	2012-01-10 19:55:30 +0800;
\par 修改时间:
	2012-03-28 21:48 +0800;
\par 文本编码:
	UTF-8;
\par 模块名称:
	YSLib::UI::YBrush;
*/


#ifndef YSL_INC_UI_BORDER_H_
#define YSL_INC_UI_BORDER_H_

#include "ywgtevt.h"
#include "../Service/yres.h"

YSL_BEGIN

YSL_BEGIN_NAMESPACE(Components)

/*!
\brief 单色画刷。
\since build 293 。
*/
class SolidBrush
{
public:
	Drawing::Color Color;

	SolidBrush(Drawing::Color);

	void
	operator()(PaintEventArgs&&);
};

inline
SolidBrush::SolidBrush(Drawing::Color c)
	: Color(c)
{}


/*!
\brief 图像画刷。
\since build 294 。
*/
class ImageBrush
{
public:
	mutable shared_ptr<Drawing::Image> Image;

	yconstfn DefDeCtor(ImageBrush)
	ImageBrush(const shared_ptr<Drawing::Image>&);
	ImageBrush(shared_ptr<Drawing::Image>&&);
	DefDeCopyCtor(ImageBrush)
	DefDeMoveCtor(ImageBrush)

	DefDeCopyAssignment(ImageBrush)
	DefDeMoveAssignment(ImageBrush)

	void
	operator()(PaintEventArgs&&);
};

inline
ImageBrush::ImageBrush(const shared_ptr<Drawing::Image>& h)
	: Image(h)
{}
inline
ImageBrush::ImageBrush(shared_ptr<Drawing::Image>&& h)
	: Image(std::move(h))
{}


/*!
\brief 边框样式。
\warning 非虚析构。
\since build 276 。
*/
class BorderStyle
{
public:
	Color ActiveColor, InactiveColor;

	BorderStyle();
};


/*!
\brief 边框画刷。
\warning 非虚析构。
\since build 284 。
*/
class BorderBrush
{
public:
	/*!
	\brief 边框样式。
	\since build 295 。
	*/
	BorderStyle Style;

	DefDeCtor(BorderBrush)
	/*!
	\brief 复制构造：默认实现。
	\since build 297 。
	*/
	DefDeCopyCtor(BorderBrush)
	/*!
	\brief 初始化；使用指定边框样式。
	\since build 295 。
	*/
	BorderBrush(const BorderStyle&);

	inline DefDeCopyAssignment(BorderBrush)

	void
	operator()(PaintEventArgs&&);
};

inline
BorderBrush::BorderBrush(const BorderStyle& style)
	: Style(style)
{}

YSL_END_NAMESPACE(Components)

YSL_END

#endif
