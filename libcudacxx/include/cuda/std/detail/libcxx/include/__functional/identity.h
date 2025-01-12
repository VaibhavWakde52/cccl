// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___FUNCTIONAL_IDENTITY_H
#define _LIBCUDACXX___FUNCTIONAL_IDENTITY_H

#ifndef __cuda_std__
#include <__config>
#endif // __cuda_std__

#include "../__utility/forward.h"

#if defined(_CCCL_COMPILER_NVHPC) && defined(_CCCL_USE_IMPLICIT_SYSTEM_DEADER)
#pragma GCC system_header
#else // ^^^ _CCCL_COMPILER_NVHPC ^^^ / vvv !_CCCL_COMPILER_NVHPC vvv
_CCCL_IMPLICIT_SYSTEM_HEADER
#endif // !_CCCL_COMPILER_NVHPC

_LIBCUDACXX_BEGIN_NAMESPACE_STD

struct __identity {
  template <class _Tp>
  _LIBCUDACXX_NODISCARD_EXT _LIBCUDACXX_INLINE_VISIBILITY constexpr _Tp&& operator()(_Tp&& __t) const noexcept {
    return _CUDA_VSTD::forward<_Tp>(__t);
  }

  using is_transparent = void;
};

#if _LIBCUDACXX_STD_VER > 11

struct identity {
    template<class _Tp>
    _LIBCUDACXX_NODISCARD_EXT _LIBCUDACXX_INLINE_VISIBILITY constexpr _Tp&& operator()(_Tp&& __t) const noexcept
    {
        return _CUDA_VSTD::forward<_Tp>(__t);
    }

    using is_transparent = void;
};
#endif // _LIBCUDACXX_STD_VER > 11

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___FUNCTIONAL_IDENTITY_H
