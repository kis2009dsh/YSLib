﻿/*
	Copyright (C) by Franksoft 2010 - 2011.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\defgroup YReader YReader
\brief YReader 阅读器。
*/

/*!	\file Shells.h
\ingroup YReader
\brief Shell 框架逻辑。
\version 0.3317;
\author FrankHB<frankhb1989@gmail.com>
\par 创建时间:
	2010-03-06 21:38:16 +0800;
\par 修改时间:
	2011-07-01 00:49 +0800;
\par 字符集:
	UTF-8;
\par 模块名称:
	YReader::Shells;
*/


#ifndef INCLUDED_SHELLS_H_
#define INCLUDED_SHELLS_H_

#include <YSLib/Helper/shlds.h>
#include "DSReader.h"

YSL_BEGIN

//全局常量。
//extern CPATH DEF_DIRECTORY;

using namespace Components;
using namespace Components::Widgets;
using namespace Components::Controls;
using namespace Components::Forms;
using namespace Drawing;

using namespace DS;
using namespace DS::Components;

shared_ptr<Image>&
FetchImage(size_t);


class ShlExplorer : public ShlDS
{
public:
	typedef ShlDS ParentType;

	struct TFormTest : public Form
	{
		Button btnEnterTest, btnMenuTest, btnShowWindow,
			btnPrevBackground, btnNextBackground;

		TFormTest();

		static void
		OnEnter_btnEnterTest(IControl& sender, TouchEventArgs&&);

		static void
		OnLeave_btnEnterTest(IControl& sender, TouchEventArgs&&);

		void
		OnClick_btnMenuTest(TouchEventArgs&&);
	};

	struct TFormExtra : public Form
	{
		Button btnDragTest;
		Button btnTestEx;
		Button btnClose;
		Button btnExit;

		TFormExtra();

		void
		OnMove_btnDragTest(EventArgs&&);

		void
		OnTouchUp_btnDragTest(TouchEventArgs&&);

		void
		OnTouchDown_btnDragTest(TouchEventArgs&&);

		void
		OnClick_btnDragTest(TouchEventArgs&&);

		static void
		OnKeyPress_btnDragTest(IControl& sender, KeyEventArgs&& e);

		void
		OnClick_btnTestEx(TouchEventArgs&&);

		void
		OnClick_btnClose(TouchEventArgs&&);
	};

	Label lblTitle, lblPath;
	FileBox fbMain;
	Button btnTest, btnOK;
	CheckBox chkTest;
	unique_ptr<TFormTest> pWndTest;
	unique_ptr<TFormExtra> pWndExtra;
	Label lblA, lblB;
	MenuHost mhMain;

	ShlExplorer();

	virtual int
	ShlProc(const Message&);

	virtual int
	OnActivated(const Message&);

	virtual int
	OnDeactivated(const Message&);

private:
	IControl*
	GetBoundControlPtr(const KeyCode&);

	void
	ShowString(const String&);
	void
	ShowString(const char*);

	void
	OnClick_btnTest(TouchEventArgs&&);

	void
	OnClick_btnOK(TouchEventArgs&&);

	void
	OnViewChanged_fbMain(EventArgs&&);

	static void
	OnConfirmed_fbMain(IControl&, IndexEventArgs&&);

	static void
	OnClick_ShowWindow(IControl&, TouchEventArgs&&);

	static void
	OnTouchDown_FormExtra(IControl&, TouchEventArgs&&);
};


class ShlReader : public ShlDS
{
public:
	typedef ShlDS ParentType;

	static string path;

	MDualScreenReader Reader;
	TextFile* pTextFile;
	shared_ptr<Image> hUp, hDn;
	bool bgDirty;
	MenuHost mhMain;

	ShlReader();

	virtual int
	ShlProc(const Message&);

	virtual int
	OnActivated(const Message&);

	virtual int
	OnDeactivated(const Message&);

	virtual void
	UpdateToScreen();

private:
	void
	OnClick(TouchEventArgs&&);

	void
	OnKeyDown(KeyEventArgs&&);
/*
	void
	OnPaint_Up(EventArgs&&);

	void
	OnPaint_Down(EventArgs&&);
*/
};

YSL_END;

#endif

