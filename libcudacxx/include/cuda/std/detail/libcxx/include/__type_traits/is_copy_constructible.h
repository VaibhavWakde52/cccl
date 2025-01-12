//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___TYPE_TRAITS_IS_COPY_CONSTRUCTIBLE_H
#define _LIBCUDACXX___TYPE_TRAITS_IS_COPY_CONSTRUCTIBLE_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#include "../__type_traits/add_const.h"
#include "../__type_traits/add_lvalue_reference.h"
#include "../__type_traits/is_constructible.h"

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

template <class _Tp>
struct _LIBCUDACXX_TEMPLATE_VIS is_copy_constructible
    : public is_constructible<_Tp, __add_lvalue_reference_t<typename add_const<_Tp>::type>> {};

#if _LIBCUDACXX_STD_VER > 11 && !defined(_LIBCUDACXX_HAS_NO_VARIABLE_TEMPLATES)
template <class _Tp>
_LIBCUDACXX_INLINE_VAR constexpr bool is_copy_constructible_v = is_copy_constructible<_Tp>::value;
#endif

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___TYPE_TRAITS_IS_COPY_CONSTRUCTIBLE_H
