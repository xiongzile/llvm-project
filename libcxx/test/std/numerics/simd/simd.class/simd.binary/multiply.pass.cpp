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
#include <simd>
#include <type_traits>

#include "type_algorithms.h"
#include "../../utils.h"

namespace stdx = std::simd;

constexpr bool test() {
  types::for_each(types::vectorizable_types{}, []<class T> {
    simd_utils::test_sizes([]<int N>(std::integral_constant<int, N>) {
      std::array<T, N> arr;
      for (size_t i = 0; i != N; ++i)
        arr[i] = i % 8;
      const stdx::vec<T, N> vec(arr); // make sure operator* is const
      std::same_as<stdx::vec<T, N>> auto&& ret = vec * vec;
      for (int i = 0; i != N; ++i) {
        auto mod = i % 8;
        assert(ret[i] == T(mod * mod));
      }
    });
  });

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;
}
