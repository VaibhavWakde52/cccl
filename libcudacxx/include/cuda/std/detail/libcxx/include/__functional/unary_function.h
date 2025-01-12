//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___FUNCTIONAL_UNARY_FUNCTION_H
#define _LIBCUDACXX___FUNCTIONAL_UNARY_FUNCTION_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

#if _LIBCUDACXX_STD_VER <= 14 || defined(_LIBCUDACXX_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION)

template <class _Arg, class _Result>
struct _LIBCUDACXX_TEMPLATE_VIS _LIBCUDACXX_DEPRECATED_IN_CXX11 unary_function
{
    typedef _Arg    argument_type;
    typedef _Result result_type;
};

#endif // _LIBCUDACXX_STD_VER <= 14

template <class _Arg, class _Result> struct __unary_function_keep_layout_base {
#if _LIBCUDACXX_STD_VER <= 17 || defined(_LIBCUDACXX_ENABLE_CXX20_REMOVED_BINDER_TYPEDEFS)
  using argument_type _LIBCUDACXX_DEPRECATED_IN_CXX17 = _Arg;
  using result_type _LIBCUDACXX_DEPRECATED_IN_CXX17 = _Result;
#endif
};

#if _LIBCUDACXX_STD_VER <= 14 || defined(_LIBCUDACXX_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION)
_LIBCUDACXX_DIAGNOSTIC_PUSH
_LIBCUDACXX_CLANG_DIAGNOSTIC_IGNORED("-Wdeprecated-declarations")
_LIBCUDACXX_GCC_DIAGNOSTIC_IGNORED("-Wdeprecated-declarations")
template <class _Arg, class _Result>
using __unary_function = unary_function<_Arg, _Result>;
_LIBCUDACXX_DIAGNOSTIC_POP
#else
template <class _Arg, class _Result>
using __unary_function = __unary_function_keep_layout_base<_Arg, _Result>;
#endif

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___FUNCTIONAL_UNARY_FUNCTION_H
