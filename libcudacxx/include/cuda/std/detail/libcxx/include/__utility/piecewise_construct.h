//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___UTILITY_PIECEWISE_CONSTRUCT_H
#define _LIBCUDACXX___UTILITY_PIECEWISE_CONSTRUCT_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

struct _LIBCUDACXX_TEMPLATE_VIS piecewise_construct_t { explicit piecewise_construct_t() = default; };
#if defined(_LIBCUDACXX_BUILDING_LIBRARY)
extern _LIBCUDACXX_EXPORTED_FROM_ABI const piecewise_construct_t piecewise_construct;// = piecewise_construct_t();
#else
/* _LIBCUDACXX_INLINE_VAR */ constexpr piecewise_construct_t piecewise_construct = piecewise_construct_t();
#endif

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___UTILITY_PIECEWISE_CONSTRUCT_H
