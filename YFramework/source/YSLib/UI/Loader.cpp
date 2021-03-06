﻿/*
	© 2013 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file Loader.cpp
\ingroup UI
\brief 动态 GUI 加载。
\version r219
\author FrankHB <frankhb1989@gmail.com>
\since build 433
\par 创建时间:
	2013-08-01 20:39:49 +0800
\par 修改时间:
	2013-10-12 03:17 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::UI::Loader
*/


#include "YSLib/UI/Loader.h"
#include <sstream>
#include "YSLib/UI/ypanel.h"
#include "NPL/SContext.h"
#include "NPL/Configuration.h"

namespace YSLib
{

namespace UI
{

Rect
ParseRect(const string& str)
{
	std::istringstream iss(str);
	int buf[4];

	for(size_t i(0); i < 4; ++i)
		if(iss)
			iss >> buf[i];
		else
			throw std::invalid_argument("Parse 'Rect' failed: bad state.");
	// FIXME: Complete max value checking.
	if(buf[2] < 0 || buf[3] < 0)
		throw std::invalid_argument("Parse 'Rect' failed: underflow.");

	Rect res(buf[0], buf[1], buf[2], buf[3]);

	YTraceDe(Informative, "ParseRect: %s.\n", to_string(res).c_str());

	return res;
}


IWidget&
AccessWidget(const ValueNode& node)
{
	const auto& p(AccessChild<shared_ptr<IWidget>>(node, "$pointer"));

	YAssert(bool(p), "Null pointer found.");

	return *p;
}


unique_ptr<IWidget>
WidgetLoader::DetectWidgetNode(const ValueNode& node)
{
	try
	{
		const auto& type_str(AccessChild<string>(node, "$type"));

		if(const auto* p_bounds_str = AccessChildPtr<string>(node, "$bounds"))
			try
			{
				const Rect& bounds(ParseRect(*p_bounds_str));

				return Bounds.CreateWidget(type_str, bounds);
			}
			catch(std::invalid_argument&)
			{}
		return Default.CreateWidget(type_str);
	}
	catch(ystdex::bad_any_cast&)
	{}
	return {};
}

ValueNode
WidgetLoader::LoadUILayout(const string& str)
{
	return TransformUILayout(NPL::LoadNPLA1(NPL::SContext::Analyze(
		NPL::Session(str))));
}

ValueNode
WidgetLoader::TransformUILayout(const ValueNode& node)
{
	if(unique_ptr<IWidget> p_new_widget{DetectWidgetNode(node)})
	{
		ValueNode res(0, node.GetName());

		if(auto p_pnl = dynamic_cast<Panel*>(p_new_widget.get()))
		{
			auto p_cont(make_unique<ValueNode::Container>());

			for(const auto& vn : node)
				if(CheckChildName(vn.GetName()))
					try
					{
						if(ValueNode child{TransformUILayout(vn)})
						{
							auto& p_wgt(*AccessChild<shared_ptr<IWidget>>(child,
								"$pointer"));

							if(p_cont->insert(std::move(child)).second)
								*p_pnl += p_wgt;
						}
					}
					catch(ystdex::bad_any_cast&)
					{}
			res += ValueNode{0, "$children", std::move(p_cont), PointerTag()};
		}
		res += ValueNode{0, "$pointer",
			shared_ptr<IWidget>(std::move(p_new_widget))};
		return std::move(res);
	}
	return {};
}

} // namespace UI;

} // namespace YSLib;

