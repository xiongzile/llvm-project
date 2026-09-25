//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <simd>

#include <simd>
#include <type_traits>

#include "type_algorithms.h"
#include "../utils.h"

namespace stdx = std::simd;

template <class T>
constexpr void test() {
  { // check size deduction
    using simd_t = stdx::mask<T>;
    static_assert(std::is_trivially_copyable_v<simd_t>);
  }

  { // check a few explicit sizes
    simd_utils::test_sizes([]<int N>(std::integral_constant<int, N>) {
      static_assert(std::is_trivially_copyable_v<stdx::mask<T, N>>);
    });
  }
}

static_assert([] {
  types::for_each(types::vectorizable_types{}, []<class T> { test<T>(); });

  return true;
}());
