//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___UTILITY_DECLVAL_H
#define _LIBCUDACXX___UTILITY_DECLVAL_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

// Suppress deprecation notice for volatile-qualified return type resulting
// from volatile-qualified types _Tp.
_LIBCUDACXX_SUPPRESS_DEPRECATED_PUSH
template <class _Tp>
_LIBCUDACXX_INLINE_VISIBILITY _Tp&& __declval(int);
template <class _Tp>
_LIBCUDACXX_INLINE_VISIBILITY _Tp __declval(long);
_LIBCUDACXX_SUPPRESS_DEPRECATED_POP

template <class _Tp>
_LIBCUDACXX_INLINE_VISIBILITY decltype(_CUDA_VSTD::__declval<_Tp>(0)) declval() noexcept;

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___UTILITY_DECLVAL_H
