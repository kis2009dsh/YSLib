﻿/*
	Copyright by FrankHB 2010 - 2013.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file cast.hpp
\ingroup YStandardEx
\brief C++ 转换模板。
\version r858
\author FrankHB <frankhb1989@gmail.com>
\since build 175
\par 创建时间:
	2010-12-15 08:13:18 +0800
\par 修改时间:
	2013-09-23 12:13 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YStandardEx::Cast
*/


#ifndef YB_INC_ystdex_cast_hpp_
#define YB_INC_ystdex_cast_hpp_ 1

#include "type_op.hpp"
#include "cassert.h"
#include <memory>
#include <typeinfo> // for dynamic_cast;
#include <initializer_list> // for std::initialize_list;

namespace ystdex
{

/*!	\defgroup cast Cast
\brief 显式类型转换。
\since build 243
*/


/*!
\ingroup cast
\brief 使用匿名联合体进行的类型转换。
\tparam _tSrc 源类型。
\tparam _tDst 目标类型。
\pre <tt>is_pod<_tDst>::value && sizeof<_tSrc> == sizeof<_tDst></tt> 。
\since build 297
*/
template<typename _tDst, typename _tSrc>
inline _tDst
union_cast(_tSrc x) ynothrow
{
	static_assert(is_pod<_tDst>::value, "Non-POD destination type found.");
	static_assert(sizeof(_tSrc) == sizeof(_tDst), "Incompatible types found.");

	union
	{
		_tSrc x;
		_tDst y;
	} u = {x};
	return u.y;
}


/*!
\brief 初值符列表转换。
\tparam _tSrc 源类型。
\tparam _tDst 目标类型。
\since build 304
*/
template<typename _tDst, typename... _tSrc>
yconstfn std::initializer_list<_tDst>
initializer_cast(_tSrc&&... x)
{
	return {_tDst(yforward(x))...};
}


/*!
\ingroup cast
\brief 多态类指针类型转换。
\tparam _tSrc 源类型。
\tparam _pDst 目标类型。
\pre 静态断言： _tSrc 是多态类。
\pre 静态断言： _pDst 是内建指针。
\throw std::bad_cast dynamic_cast 失败。
\since build 175
*/
template <typename _pDst, class _tSrc>
inline _pDst
polymorphic_cast(_tSrc* x)
{
	static_assert(is_polymorphic<_tSrc>::value, "Non-polymorphic class found.");
	static_assert(is_pointer<_pDst>::value, "Non-pointer destination found.");

	const auto tmp(dynamic_cast<_pDst>(x));

	if(!tmp)
		throw std::bad_cast();
	return tmp;
}

/*!
\ingroup cast
\brief 多态类指针向派生类指针转换。
\tparam _tSrc 源类型。
\tparam _pDst 目标类型。
\pre 静态断言： _tSrc 是多态类。
\pre 静态断言： _pDst 是内建指针。
\pre 静态断言： _tSrc 是 _pDst 指向的类去除修饰符后的基类。
\pre 断言： dynamic_cast 成功。
\since build 175
*/
template <typename _pDst, class _tSrc>
inline _pDst
polymorphic_downcast(_tSrc* x)
{
	static_assert(is_polymorphic<_tSrc>::value, "Non-polymorphic class found.");
	static_assert(is_pointer<_pDst>::value, "Non-pointer destination found.");
	static_assert(is_base_of<_tSrc, remove_cv_t<
		remove_pointer_t<_pDst>>>::value, "Wrong destination type found.");

	yassume(dynamic_cast<_pDst>(x) == x);

	return static_cast<_pDst>(x);
}
/*!
\ingroup cast
\brief 多态类引用向派生类引用转换。
\tparam _tSrc 源类型。
\tparam _rDst 目标类型。
\pre 静态断言： _rDst 是引用。
\since build 175
\todo 扩展接受右值引用参数。
*/
template <typename _rDst, class _tSrc>
yconstfn _rDst&
polymorphic_downcast(_tSrc& x)
{
	static_assert(is_reference<_rDst>::value,
		"Non-reference destination found.");

	return *ystdex::polymorphic_downcast<remove_reference_t<_rDst>*>(
		std::addressof(x));
}

/*!
\ingroup cast
\brief 多态类指针交叉转换。
\tparam _tSrc 源类型。
\tparam _pDst 目标类型。
\pre 静态断言： _tSrc 是多态类。
\pre 静态断言： _pDst 是内建指针。
\pre 断言： dynamic_cast 成功。
\return 非空结果。
\note 空指针作为参数一定失败。
\since build 179
*/
template <typename _pDst, class _tSrc>
inline _pDst
polymorphic_crosscast(_tSrc* x)
{
	static_assert(is_polymorphic<_tSrc>::value, "Non-polymorphic class found.");
	static_assert(is_pointer<_pDst>::value, "Non-pointer destination found.");

	auto p(dynamic_cast<_pDst>(x));

	yassume(p);
	return p;
}
/*!
\ingroup cast
\brief 多态类引用交叉转换。
\tparam _tSrc 源类型。
\tparam _rDst 目标类型。
\pre 静态断言： _rDst 是引用。
\since build 179
\todo 扩展接受右值引用参数。
*/
template <typename _rDst, class _tSrc>
yconstfn _rDst&
polymorphic_crosscast(_tSrc& x)
{
	static_assert(is_reference<_rDst>::value,
		"Non-reference destination found.");

	return *ystdex::polymorphic_crosscast<remove_reference_t<_rDst>*>(
		std::addressof(x));
}


namespace details
{

template<typename _tFrom, typename _tTo, bool _bNonVirtualDownCast>
struct _general_polymorphic_cast_helper
{
	static yconstfn _tTo
	cast(_tFrom x)
	{
		return ystdex::polymorphic_downcast<_tTo>(x);
	}
};
template<typename _tFrom, typename _tTo>
struct _general_polymorphic_cast_helper<_tFrom, _tTo, false>
{
	static yconstfn _tTo
	cast(_tFrom x)
	{
		return dynamic_cast<_tTo>(x);
	}
};

template<typename _tFrom, typename _tTo, bool _bUseStaticCast>
struct _general_cast_helper
{
	static yconstfn _tTo
	cast(_tFrom x)
	{
		return _tTo(x);
	}
};
template<typename _tFrom, typename _tTo>
struct _general_cast_helper<_tFrom, _tTo, false>
{
	static yconstfn _tTo
	cast(_tFrom x)
	{
		return _general_polymorphic_cast_helper<_tFrom, _tTo, (is_base_of<
			_tFrom, _tTo>::value && !has_common_nonempty_virtual_base<typename
			remove_rp<_tFrom>::type, typename remove_rp<_tTo>::type>::value)
		>::cast(x);
	}
};
template<typename _type>
struct _general_cast_helper<_type, _type, true>
{
	static inline _type
	cast(_type x)
	{
		return x;
	}
};
template<typename _type>
struct _general_cast_helper<_type, _type, false>
{
	static yconstfn _type
	cast(_type x)
	{
		return x;
	}
};

template<typename _tFrom, typename _tTo>
struct _general_cast_type_helper
	: integral_constant<bool, is_convertible<_tFrom, _tTo>::value>
{};

} // namespace details;

/*!
\ingroup cast
\brief 一般类型转换。
\tparam _tSrc 源类型。
\tparam _tDst 目标类型。
\since build 175
\todo 扩展接受右值引用参数。

能确保安全隐式转换时使用 static_cast ；
除此之外非虚基类向派生类转换使用 polymophic_downcast；
否则使用 dynamic_cast。
*/
//@{
template<typename _tDst, typename _tSrc>
yconstfn _tDst
general_cast(_tSrc* x)
{
	return details::_general_cast_helper<_tSrc*, _tDst,
		details::_general_cast_type_helper<_tSrc*, _tDst>::value>::cast(x);
}
template<typename _tDst, typename _tSrc>
yconstfn _tDst
general_cast(_tSrc& x)
{
	return details::_general_cast_helper<_tSrc&, _tDst,
		details::_general_cast_type_helper<_tSrc&, _tDst>::value>::cast(x);
}
template<typename _tDst, typename _tSrc>
yconstfn const _tDst
general_cast(const _tSrc& x)
{
	return details::_general_cast_helper<const _tSrc&, _tDst, details
		::_general_cast_type_helper<const _tSrc&, const _tDst>::value>::cast(x);
}
//@}

} // namespace ystdex;

#endif

