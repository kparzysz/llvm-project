//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___TYPE_TRAITS_IS_BASE_OF_H
#define _LIBCPP___TYPE_TRAITS_IS_BASE_OF_H

#include <__config>
#include <__type_traits/integral_constant.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Bp, class _Dp>
struct _LIBCPP_NO_SPECIALIZATIONS is_base_of : integral_constant<bool, __is_base_of(_Bp, _Dp)> {};

#if _LIBCPP_STD_VER >= 17
template <class _Bp, class _Dp>
_LIBCPP_NO_SPECIALIZATIONS inline constexpr bool is_base_of_v = __is_base_of(_Bp, _Dp);
#endif

#if _LIBCPP_STD_VER >= 26
#  if __has_builtin(__builtin_is_virtual_base_of)

template <class _Base, class _Derived>
struct _LIBCPP_NO_SPECIALIZATIONS is_virtual_base_of : bool_constant<__builtin_is_virtual_base_of(_Base, _Derived)> {};

template <class _Base, class _Derived>
_LIBCPP_NO_SPECIALIZATIONS inline constexpr bool is_virtual_base_of_v = __builtin_is_virtual_base_of(_Base, _Derived);

#  endif
#endif

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___TYPE_TRAITS_IS_BASE_OF_H
