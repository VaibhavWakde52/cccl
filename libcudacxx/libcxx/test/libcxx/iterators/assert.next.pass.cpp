//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

// REQUIRES: has-unix-headers
// UNSUPPORTED: c++03
// XFAIL: use_system_cxx_lib && target={{.+}}-apple-macosx{{10.9|10.10|10.11|10.12|10.13|10.14|10.15|11.0|12.0}}
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCUDACXX_ENABLE_ASSERTIONS=1
#define _LIBCUDACXX_ENABLE_ASSERTIONS 1

// <list>

// Call next(non-bidi iterator, -1)

#define _LIBCUDACXX_ENABLE_ASSERTIONS 1

#include <iterator>

#include "check_assertion.h"
#include "test_iterators.h"

int main(int, char**) {
    int a[] = {1, 2, 3};
    forward_iterator<int *> it(a+1);
    std::next(it, 1);  // should work fine
    std::next(it, 0);  // should work fine
    TEST_LIBCUDACXX_ASSERT_FAILURE(std::next(it, -1), "Attempt to next(it, n) with negative n on a non-bidirectional iterator");

    return 0;
}
