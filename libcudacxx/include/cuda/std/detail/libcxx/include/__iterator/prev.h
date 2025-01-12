// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___ITERATOR_PREV_H
#define _LIBCUDACXX___ITERATOR_PREV_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#include "../__assert"
#include "../__iterator/advance.h"
#include "../__iterator/iterator_traits.h"
#include "../__type_traits/enable_if.h"

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

template <class _InputIter>
inline _LIBCUDACXX_INLINE_VISIBILITY _LIBCUDACXX_CONSTEXPR_AFTER_CXX14
__enable_if_t
<
    __is_cpp17_input_iterator<_InputIter>::value,
    _InputIter
>
prev(_InputIter __x,
     typename iterator_traits<_InputIter>::difference_type __n = 1)
{
    _LIBCUDACXX_ASSERT(__n <= 0 || __is_cpp17_bidirectional_iterator<_InputIter>::value,
                       "Attempt to prev(it, +n) on a non-bidi iterator");
    _CUDA_VSTD::advance(__x, -__n);
    return __x;
}

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___ITERATOR_PREV_H
