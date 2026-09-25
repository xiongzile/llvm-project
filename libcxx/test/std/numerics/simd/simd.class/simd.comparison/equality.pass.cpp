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
#include <concepts>
#include <numeric>
#include <simd>

#include "type_algorithms.h"
#include "../../utils.h"

namespace stdx = std::simd;

template <class T, std::size_t N>
constexpr void test(stdx::vec<T, N> lhs, stdx::vec<T, N> rhs, std::array<bool, N> expected) {
  { // Test operator==
    std::same_as<typename stdx::vec<T, N>::mask_type> auto result = lhs == rhs;
    for (size_t i = 0; i != N; ++i) {
      assert(result[i] == expected[i]);
    }
  }
  { // Test operator!=
    std::same_as<typename stdx::vec<T, N>::mask_type> auto result = lhs != rhs;
    for (size_t i = 0; i != N; ++i) {
      assert(result[i] == !expected[i]);
    }
  }
}

template <class T>
constexpr void test() {
  test<T, 4>(std::array<T, 4>{1, 2, 3, 4}, std::array<T, 4>{1, 2, 3, 4}, {true, true, true, true});
  test<T, 4>(std::array<T, 4>{4, 3, 2, 1}, std::array<T, 4>{1, 2, 3, 4}, {false, false, false, false});
  test<T, 4>(std::array<T, 4>{1, 2, 3, 4}, std::array<T, 4>{1, 2, 4, 3}, {true, true, false, false});
  test<T, 4>(std::array<T, 4>{1, 1, 3, 4}, std::array<T, 4>{1, 2, 4, 1}, {true, false, false, false});
}

constexpr bool test() {
  types::for_each(types::vectorizable_types{}, []<class T> { test<T>(); });
  types::for_each(types::vectorizable_float_types{}, []<class T> {
    constexpr auto nan = std::numeric_limits<T>::quiet_NaN();
    stdx::vec<T, 4> a   = std::array<T, 4>{nan, nan, nan, nan};
    stdx::vec<T, 4> b   = a;
    assert(stdx::none_of(a == b));
    assert(stdx::all_of(a != b));
  });

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;
}
