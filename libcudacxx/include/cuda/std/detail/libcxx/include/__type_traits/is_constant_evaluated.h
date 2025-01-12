//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___TYPE_TRAITS_IS_CONSTANT_EVALUATED_H
#define _LIBCUDACXX___TYPE_TRAITS_IS_CONSTANT_EVALUATED_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

#if defined(_LIBCUDACXX_IS_CONSTANT_EVALUATED)
#if defined(__cuda_std__) || _LIBCUDACXX_STD_VER > 17
_LIBCUDACXX_INLINE_VISIBILITY
inline constexpr bool is_constant_evaluated() noexcept {
  return _LIBCUDACXX_IS_CONSTANT_EVALUATED();
}
#endif

inline constexpr _LIBCUDACXX_INLINE_VISIBILITY
bool __libcpp_is_constant_evaluated() noexcept { return _LIBCUDACXX_IS_CONSTANT_EVALUATED(); }
#else
inline constexpr _LIBCUDACXX_INLINE_VISIBILITY
bool __libcpp_is_constant_evaluated() noexcept { return false; }
#endif // defined(_LIBCUDACXX_IS_CONSTANT_EVALUATED)

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___TYPE_TRAITS_IS_CONSTANT_EVALUATED_H
