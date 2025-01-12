// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___FUNCTIONAL_COMPOSE_H
#define _LIBCUDACXX___FUNCTIONAL_COMPOSE_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#include "../__functional/invoke.h"
#include "../__functional/perfect_forward.h"
#include "../__type_traits/decay.h"
#include "../__utility/forward.h"

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

#if _LIBCUDACXX_STD_VER > 14

struct __compose_op {
    template<class _Fn1, class _Fn2, class ..._Args>
    _LIBCUDACXX_HIDE_FROM_ABI _LIBCUDACXX_INLINE_VISIBILITY
    constexpr auto operator()(_Fn1&& __f1, _Fn2&& __f2, _Args&&... __args) const
        noexcept(noexcept(_CUDA_VSTD::invoke(_CUDA_VSTD::forward<_Fn1>(__f1), _CUDA_VSTD::invoke(_CUDA_VSTD::forward<_Fn2>(__f2), _CUDA_VSTD::forward<_Args>(__args)...))))
        -> decltype(      _CUDA_VSTD::invoke(_CUDA_VSTD::forward<_Fn1>(__f1), _CUDA_VSTD::invoke(_CUDA_VSTD::forward<_Fn2>(__f2), _CUDA_VSTD::forward<_Args>(__args)...)))
        { return          _CUDA_VSTD::invoke(_CUDA_VSTD::forward<_Fn1>(__f1), _CUDA_VSTD::invoke(_CUDA_VSTD::forward<_Fn2>(__f2), _CUDA_VSTD::forward<_Args>(__args)...)); }
};

template <class _Fn1, class _Fn2>
struct __compose_t : __perfect_forward<__compose_op, _Fn1, _Fn2> {
    using __perfect_forward<__compose_op, _Fn1, _Fn2>::__perfect_forward;
};

template <class _Fn1, class _Fn2>
_LIBCUDACXX_HIDE_FROM_ABI _LIBCUDACXX_INLINE_VISIBILITY
constexpr auto __compose(_Fn1&& __f1, _Fn2&& __f2)
    noexcept(noexcept(__compose_t<decay_t<_Fn1>, decay_t<_Fn2>>(_CUDA_VSTD::forward<_Fn1>(__f1), _CUDA_VSTD::forward<_Fn2>(__f2))))
    -> decltype(      __compose_t<decay_t<_Fn1>, decay_t<_Fn2>>(_CUDA_VSTD::forward<_Fn1>(__f1), _CUDA_VSTD::forward<_Fn2>(__f2)))
    { return          __compose_t<decay_t<_Fn1>, decay_t<_Fn2>>(_CUDA_VSTD::forward<_Fn1>(__f1), _CUDA_VSTD::forward<_Fn2>(__f2)); }

#endif // _LIBCUDACXX_STD_VER > 14

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___FUNCTIONAL_COMPOSE_H
