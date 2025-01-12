// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___ITERATOR_INSERT_ITERATOR_H
#define _LIBCUDACXX___ITERATOR_INSERT_ITERATOR_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#include "../__iterator/iterator_traits.h"
#include "../__iterator/iterator.h"
#include "../__memory/addressof.h"
#include "../__utility/move.h"
#include "../cstddef"

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

_LIBCUDACXX_SUPPRESS_DEPRECATED_PUSH
template <class _Container>
class _LIBCUDACXX_TEMPLATE_VIS insert_iterator
#if _LIBCUDACXX_STD_VER <= 14 || !defined(_LIBCUDACXX_ABI_NO_ITERATOR_BASES)
    : public iterator<output_iterator_tag,
                      void,
                      void,
                      void,
                      void>
#endif
{
_LIBCUDACXX_SUPPRESS_DEPRECATED_POP
protected:
    _Container* container;
    typename _Container::iterator iter;
public:
    typedef _Container container_type;

    _LIBCUDACXX_INLINE_VISIBILITY insert_iterator(_Container& __x, typename _Container::iterator __i)
        : container(_CUDA_VSTD::addressof(__x)), iter(__i) {}
    _LIBCUDACXX_INLINE_VISIBILITY insert_iterator& operator=(const typename _Container::value_type& __value_)
        {iter = container->insert(iter, __value_); ++iter; return *this;}
    _LIBCUDACXX_INLINE_VISIBILITY insert_iterator& operator=(typename _Container::value_type&& __value_)
        {iter = container->insert(iter, _CUDA_VSTD::move(__value_)); ++iter; return *this;}
    _LIBCUDACXX_INLINE_VISIBILITY insert_iterator& operator*()        {return *this;}
    _LIBCUDACXX_INLINE_VISIBILITY insert_iterator& operator++()       {return *this;}
    _LIBCUDACXX_INLINE_VISIBILITY insert_iterator& operator++(int)    {return *this;}
};

template <class _Container>
inline _LIBCUDACXX_INLINE_VISIBILITY
insert_iterator<_Container>
inserter(_Container& __x, typename _Container::iterator __i)
{
    return insert_iterator<_Container>(__x, __i);
}

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___ITERATOR_INSERT_ITERATOR_H
