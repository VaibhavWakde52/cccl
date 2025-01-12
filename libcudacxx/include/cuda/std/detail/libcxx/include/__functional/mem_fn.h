// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___FUNCTIONAL_MEM_FN_H
#define _LIBCUDACXX___FUNCTIONAL_MEM_FN_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#include "../__functional/binary_function.h"
#include "../__functional/invoke.h"
#include "../__functional/weak_result_type.h"
#include "../__utility/forward.h"

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

template <class _Tp>
class __mem_fn : public __weak_result_type<_Tp>
{
public:
    // types
    typedef _Tp type;
private:
    type __f_;

public:
    _LIBCUDACXX_INLINE_VISIBILITY _LIBCUDACXX_CONSTEXPR_AFTER_CXX17
    __mem_fn(type __f) noexcept : __f_(__f) {}

    // invoke
    template <class... _ArgTypes>
    _LIBCUDACXX_INLINE_VISIBILITY _LIBCUDACXX_CONSTEXPR_AFTER_CXX17
    typename __invoke_return<type, _ArgTypes...>::type
    operator() (_ArgTypes&&... __args) const {
        return _CUDA_VSTD::__invoke(__f_, _CUDA_VSTD::forward<_ArgTypes>(__args)...);
    }
};

template<class _Rp, class _Tp>
inline _LIBCUDACXX_INLINE_VISIBILITY _LIBCUDACXX_CONSTEXPR_AFTER_CXX17
__mem_fn<_Rp _Tp::*>
mem_fn(_Rp _Tp::* __pm) noexcept
{
    return __mem_fn<_Rp _Tp::*>(__pm);
}

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___FUNCTIONAL_MEM_FN_H
