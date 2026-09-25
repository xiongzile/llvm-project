//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <simd>

// REQUIRES: std-at-least-c++26

#include <array>
#include <cassert>
#include <numeric>
#include <simd>
#include <type_traits>

#include "type_algorithms.h"
#include "../../utils.h"

namespace stdx = std::simd;

constexpr bool test() {
  types::for_each(types::vectorizable_types{}, []<class T> {
    simd_utils::test_sizes([]<int N>(std::integral_constant<int, N>) {
      std::array<T, N> arr;
      std::iota(std::begin(arr), std::end(arr), 55);
      stdx::vec<T, N> vec(arr);
      const stdx::vec<T, N> vec2(T(6)); // make sure operator- is const
      std::same_as<stdx::vec<T, N>&> auto&& ret = vec /= vec2;
      assert(&ret == &vec);
      for (int i = 0; i != N; ++i)
        assert(ret[i] == T(i + 55) / 6);
    });
  });

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;
}
