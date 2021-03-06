﻿/*
	© 2013 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file Animation.h
\ingroup UI
\brief 样式无关的动画实现。
\version r295
\author FrankHB <frankhb1989@gmail.com>
\since build 448
\par 创建时间:
	2013-10-06 22:11:33 +0800
\par 修改时间:
	2013-11-06 19:30 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YSLib::UI::Animation
*/


#ifndef YSL_INC_UI_Animation_h_
#define YSL_INC_UI_Animation_h_ 1

#include "ywidget.h"
#include "../Core/Task.h"

namespace YSLib
{

namespace UI
{

/*!
\brief 默认背景动画任务消息优先级。
\since build 450
*/
yconstexpr Messaging::Priority AnimationPriority(0x20);


//! \since build 455
//@{
//! \brief 按更新条件和优先级通过消息队列部署动画任务。
template<typename _fCallable>
void
AnimateTask(_fCallable update,
	Messaging::Priority prior = UI::AnimationPriority)
{
	Messaging::Renew(update, prior);
}

//! \brief 按指定的连接对象更新动画任务。
template<typename _fCallable>
void
AnimateConnection(const shared_ptr<_fCallable>& conn,
	Messaging::Priority prior = UI::AnimationPriority)
{
	YAssert(bool(conn), "Null pointer found.");

	AnimateTask([=]{
		return (*conn)();
	}, prior);
}

//! \brief 按指定的可调用对象初始化为连接对象更新动画任务。
template<typename _fCallable>
void
Animate(_fCallable&& f, Messaging::Priority prior = UI::AnimationPriority)
{
	using Callable = ystdex::decay_t<_fCallable>;

	AnimateConnection(ystdex::make_shared<Callable>(yforward(f)), prior);
}


//! \brief 动画会话。
template<typename _tCallable = std::function<bool()>>
class GAnimationSession final
{
public:
	using Connection = ystdex::decay_t<_tCallable>;
	using ConnectionPtr = shared_ptr<Connection>;

private:
	ConnectionPtr conn;

public:
	DefDeCtor(GAnimationSession)
	//! \brief 使用 ADL 调用 ResetState 函数复位连接对象状态以确保安全。
	~GAnimationSession()
	{
		if(conn)
			ResetState(*conn);
	}

	template<typename... _tParams>
	void
	Reset(_tParams&&... args)
	{
		conn.reset(new Connection(yforward(args)...));
	}

	//! \brief 使用 ADL 调用 AnimateConnection 开始动画。
	PDefH(void, Start, ) const
		ImplExpr(AnimateConnection(conn))

	DefGetter(const ynothrow, Connection&, ConnectionRef,
		(YAssert(bool(conn), "Null pointer found."), *conn))
	DefGetter(const ynothrow, const ConnectionPtr&, ConnectionPtr, conn)
};


//! \brief 状态更新器。
class YF_API InvalidationUpdater
{
public:
	IWidget* WidgetPtr;
	bool Ready;

private:
	//! \brief 最后持续状态：保存更新函数的最后结果。
	mutable bool last = {};

public:
	InvalidationUpdater(IWidget* p_wgt = {}, bool ready = {})
		: WidgetPtr(p_wgt), Ready(ready)
	{}

	DefPred(const ynothrow, Last, last)

	//! \brief 更新函数：无效化后根据成员指定是否需要发送 SM_Task 消息。
	bool
	operator()() const;
};

//! \relates InvalidationUpdater
inline PDefH(void, ResetState, InvalidationUpdater& updater)
	ImplExpr(updater.WidgetPtr = {})
//@}

} // namespace UI;

} // namespace YSLib;

#endif

