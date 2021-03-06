﻿/*
	Copyright by FrankHB 2011 - 2013.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file HexBrowser.h
\ingroup YReader
\brief 十六进制浏览器。
\version r449
\author FrankHB <frankhb1989@gmail.com>
\since build 253
\par 创建时间:
	2011-10-14 18:13:04 +0800
\par 修改时间:
	2013-08-05 21:56 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YReader::HexBrowser
*/


#ifndef INC_YReader_HexBrowser_h_
#define INC_YReader_HexBrowser_h_ 1

#include <YSLib/UI/scroll.h>
#include <YSLib/Service/yfile.h>
#include <YSLib/Service/TextLayout.h>
#include <Helper/Initialization.h>

namespace YSLib
{

namespace UI
{

/*!
\brief 十六进制模型：十六进制视图区域数据源。
\warning 非虚析构。
\since build 257
*/
class HexModel
{
private:
	/*!
	\brief 文件数据源。
	\note 保证非空。
	\since build 270
	*/
	unique_ptr<File> pSource;

public:
	HexModel()
		: pSource(new File())
	{}
	//! \since build 412
	HexModel(const char* path)
		: pSource(new File(path))
	{}
	DefDelCopyCtor(HexModel)
	DefDeMoveCtor(HexModel)

	DefDeMoveAssignment(HexModel)
	HexModel&
	operator=(unique_ptr<File>&& file_ptr)
	{
		pSource = std::move(file_ptr);
		return *this;
	}

	DefPred(const ynothrow, Valid, bool(GetSource()))

private:
	DefGetter(const ynothrow, File&, Source, *pSource)

public:
	DefGetterMem(const ynothrow, FILE*, Ptr, GetSource())

	DefGetterMem(const ynothrow, ptrdiff_t, Position, GetSource())
	DefGetterMem(const ynothrow, size_t, Size, GetSource())

	DefFwdTmpl(const, ptrdiff_t, SetPosition,
		GetSource().Seek(args...))

	DefFwdFn(const, int, CheckEOF, GetSource().CheckEOF())
};


/*!
\brief 十六进制视图。
\warning 非虚析构。
\since build 257
*/
class HexView
{
public:
	static yconstexpr size_t ItemPerLine = 8; //!< 每行数据总数（字节）。

	using IndexType = std::uintptr_t; //!< 索引类型。
	using DataType = vector<char>; //!< 显示数据类型。

protected:
	Drawing::TextState TextState; //!< 文本状态。

private:
	IndexType item_num; //!< 行数。

protected:
	/*!
	\brief 当前显示的数据。
	\since 356
	*/
	DataType datCurrent;

public:
	//! \since build 396
	HexView(FontCache& = FetchDefaultFontCache());

public:
	/*!
	\brief 取项目行高。
	*/
	DefGetter(const ynothrow, SDst, ItemHeight,
		GetTextLineHeightExOf(TextState))
	DefGetter(const ynothrow, IndexType, ItemNum, item_num)

protected:
	PDefH(IndexType, UpdateItemNum, SDst h)
		ImplRet(item_num = FetchResizedLineN(TextState, h))
};


/*!
\brief 十六进制视图区域。
\since build 253
*/
class HexViewArea : public ScrollableContainer,
	protected HexView
{
public:
	using HexView::ItemPerLine;
	using HexView::IndexType;
	using HexView::DataType;
	/*!
	\brief 视图参数类型。
	\note bool 参数表示主动定位视图。
	\since build 269
	*/
	using ViewArgs = GValueEventArgs<bool>;
	/*!
	\brief 视图事件委托类型。
	\since build 269
	*/
	DeclDelegate(HViewEvent, ViewArgs)

private:
	/*!
	\brief 模型。
	*/
	HexModel model;

public:
	/*!
	\brief 视图变更事件。
	\since build 269
	*/
	DeclEvent(HViewEvent, ViewChanged)

	//! \since build 396
	explicit
	HexViewArea(const Drawing::Rect& = {},
		FontCache& = FetchDefaultFontCache());

	using HexView::GetItemHeight;
	using HexView::GetItemNum;
	DefGetter(const ynothrow, const HexModel&, Model, model)

	//! \since build 412
	void
	Load(const char*);

	/*!
	\brief 定位视图顶端至指定竖直位置（行数）。
	*/
	void
	LocateViewPosition(u32);

	/*!
	\brief 刷新：按指定参数绘制界面并更新状态。
	\since build 294
	*/
	void
	Refresh(PaintEventArgs&&) override;

	void
	Reset();

	void
	UpdateData(u32);

	/*!
	\brief 更新视图。

	调用视图变更事件、调整视图长度后无效化自身。
	\note 参数表示是否确定仅变更视图位置。
	\since build 269
	*/
	void
	UpdateView(bool = {});
};

} // namespace UI;

} // namespace YSLib;

#endif

