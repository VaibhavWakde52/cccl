// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___ITERATOR_DISTANCE_H
#define _LIBCUDACXX___ITERATOR_DISTANCE_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#include "../__iterator/iterator_traits.h"

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

template <class _InputIter>
inline _LIBCUDACXX_INLINE_VISIBILITY _LIBCUDACXX_CONSTEXPR_AFTER_CXX14
typename iterator_traits<_InputIter>::difference_type
__distance(_InputIter __first, _InputIter __last, input_iterator_tag)
{
    typename iterator_traits<_InputIter>::difference_type __r(0);
    for (; __first != __last; ++__first)
        ++__r;
    return __r;
}

template <class _RandIter>
inline _LIBCUDACXX_INLINE_VISIBILITY _LIBCUDACXX_CONSTEXPR_AFTER_CXX14
typename iterator_traits<_RandIter>::difference_type
__distance(_RandIter __first, _RandIter __last, random_access_iterator_tag)
{
    return __last - __first;
}

template <class _InputIter>
inline _LIBCUDACXX_INLINE_VISIBILITY _LIBCUDACXX_CONSTEXPR_AFTER_CXX14
typename iterator_traits<_InputIter>::difference_type
distance(_InputIter __first, _InputIter __last)
{
    return __distance(__first, __last, typename iterator_traits<_InputIter>::iterator_category());
}


_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___ITERATOR_DISTANCE_H
