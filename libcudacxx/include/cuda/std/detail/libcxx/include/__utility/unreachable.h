//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___UTILITY_UNREACHABLE_H
#define _LIBCUDACXX___UTILITY_UNREACHABLE_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#include "../cstdlib"

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

_LIBCUDACXX_NORETURN _LIBCUDACXX_INLINE_VISIBILITY
inline void __libcpp_unreachable()
{
  _LIBCUDACXX_UNREACHABLE();
}

#if _LIBCUDACXX_STD_VER > 20

[[noreturn]] _LIBCUDACXX_INLINE_VISIBILITY
inline void unreachable() { _LIBCUDACXX_UNREACHABLE(); }

#endif // _LIBCUDACXX_STD_VER > 20

_LIBCUDACXX_END_NAMESPACE_STD

#endif
