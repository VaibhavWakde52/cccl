//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___CONCEPTS_REGULAR_H
#define _LIBCUDACXX___CONCEPTS_REGULAR_H

#ifndef __cuda_std__
#include <__config>
#endif //__cuda_std__

#include "../__concepts/__concept_macros.h"
#include "../__concepts/equality_comparable.h"
#include "../__concepts/semiregular.h"

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

#if _LIBCUDACXX_STD_VER > 17

// [concept.object]

template<class _Tp>
concept regular = semiregular<_Tp> && equality_comparable<_Tp>;

#elif _LIBCUDACXX_STD_VER > 11

// [concept.object]

template<class _Tp>
_LIBCUDACXX_CONCEPT_FRAGMENT(
  __regular_,
  requires()(
    requires(semiregular<_Tp>),
    requires(equality_comparable<_Tp>)
  ));

template<class _Tp>
_LIBCUDACXX_CONCEPT regular = _LIBCUDACXX_FRAGMENT(__regular_, _Tp);

#endif // _LIBCUDACXX_STD_VER > 11

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___CONCEPTS_REGULAR_H
