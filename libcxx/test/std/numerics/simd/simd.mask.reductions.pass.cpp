//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <simd>

// REQUIRES: std-at-least-c++26

#include <cassert>
#include <concepts>
#include <simd>

#include "type_algorithms.h"
#include "utils.h"

namespace stdx = std::simd;

constexpr bool test() {
  types::for_each(types::vectorizable_types{}, []<class T> {
    {   // test with 4 elements (and non-trivial patterns)
      { // simple check
        auto mask = simd_utils::make_mask<4>({true, true, false, false});
        assert(stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 2);
        assert(stdx::reduce_min_index(mask) == 0);
        assert(stdx::reduce_max_index(mask) == 1);
      }
      { // all are true
        auto mask = simd_utils::make_mask<4>({true, true, true, true});
        assert(stdx::any_of(mask));
        assert(stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 4);
        assert(stdx::reduce_min_index(mask) == 0);
        assert(stdx::reduce_max_index(mask) == 3);
      }
      { // none are true
        auto mask = simd_utils::make_mask<4>({false, false, false, false});
        assert(!stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 0);
        // no reduce_{min,max}_index, since the precondition isn't met.
      }
      { // interleaved true and false
        auto mask = simd_utils::make_mask<4>({false, true, false, true});
        assert(stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 2);
        assert(stdx::reduce_min_index(mask) == 1);
        assert(stdx::reduce_max_index(mask) == 3);
      }
      { // single element is true at the start
        auto mask = simd_utils::make_mask<4>({true, false, false, false});
        assert(stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 1);
        assert(stdx::reduce_min_index(mask) == 0);
        assert(stdx::reduce_max_index(mask) == 0);
      }
      { // single element is true in the middle
        auto mask = simd_utils::make_mask<4>({false, true, false, false});
        assert(stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 1);
        assert(stdx::reduce_min_index(mask) == 1);
        assert(stdx::reduce_max_index(mask) == 1);
      }
      { // single element is true at the end
        auto mask = simd_utils::make_mask<4>({false, false, false, true});
        assert(stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 1);
        assert(stdx::reduce_min_index(mask) == 3);
        assert(stdx::reduce_max_index(mask) == 3);
      }
    }
    {   // test with eight elements
      { // simple check
        auto mask = simd_utils::make_mask<8>({true, true, false, false, true, true, false, false});
        assert(stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 4);
        assert(stdx::reduce_min_index(mask) == 0);
        assert(stdx::reduce_max_index(mask) == 5);
      }
      { // all are true
        auto mask = simd_utils::make_mask<8>({true, true, true, true, true, true, true, true});
        assert(stdx::any_of(mask));
        assert(stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 8);
        assert(stdx::reduce_min_index(mask) == 0);
        assert(stdx::reduce_max_index(mask) == 7);
      }
      { // none are true
        auto mask = simd_utils::make_mask<8>({false, false, false, false, false, false, false, false});
        assert(!stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 0);
        // no reduce_{min,max}_index, since the precondition isn't met.
      }
    }
    {   // test with sixteen elements
      { // simple check
        auto mask = simd_utils::make_mask<16>(
            {true, true, false, false, true, true, false, false, true, true, false, false, true, true, false, false});
        assert(stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 8);
        assert(stdx::reduce_min_index(mask) == 0);
        assert(stdx::reduce_max_index(mask) == 13);
      }
      { // all are true
        auto mask = simd_utils::make_mask<16>(
            {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true});
        assert(stdx::any_of(mask));
        assert(stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 16);
        assert(stdx::reduce_min_index(mask) == 0);
        assert(stdx::reduce_max_index(mask) == 15);
      }
      { // none are true
        auto mask = simd_utils::make_mask<16>(
            {false,
             false,
             false,
             false,
             false,
             false,
             false,
             false,
             false,
             false,
             false,
             false,
             false,
             false,
             false,
             false});
        assert(!stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 0);
        // no reduce_{min,max}_index, since the precondition isn't met.
      }
    }
    {   // test with thirtytwo elements
      { // simple check
        auto mask = simd_utils::make_mask<32>(
            {true, true, false, false, true, true, false, false, true, true, false, false, true, true, false, false,
             true, true, false, false, true, true, false, false, true, true, false, false, true, true, false, false});
        assert(stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 16);
        assert(stdx::reduce_min_index(mask) == 0);
        assert(stdx::reduce_max_index(mask) == 29);
      }
      { // all are true
        auto mask = simd_utils::make_mask<32>(
            {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
             true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true});
        assert(stdx::any_of(mask));
        assert(stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 32);
        assert(stdx::reduce_min_index(mask) == 0);
        assert(stdx::reduce_max_index(mask) == 31);
      }
      { // none are true
        auto mask = simd_utils::make_mask<32>(
            {false, false, false, false, false, false, false, false, false, false, false,
             false, false, false, false, false, false, false, false, false, false, false,
             false, false, false, false, false, false, false, false, false, false});
        assert(!stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 0);
        // no reduce_{min,max}_index, since the precondition isn't met.
      }
    }
    {   // test with sixtyfour elements
      { // simple check
        auto mask = simd_utils::make_mask<64>(
            {true, true, false, false, true, true, false, false, true, true, false, false, true, true, false, false,
             true, true, false, false, true, true, false, false, true, true, false, false, true, true, false, false,
             true, true, false, false, true, true, false, false, true, true, false, false, true, true, false, false,
             true, true, false, false, true, true, false, false, true, true, false, false, true, true, false, false});
        assert(stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 32);
        assert(stdx::reduce_min_index(mask) == 0);
        assert(stdx::reduce_max_index(mask) == 61);
      }
      { // all are true
        auto mask = simd_utils::make_mask<64>(
            {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
             true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
             true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
             true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true});
        assert(stdx::any_of(mask));
        assert(stdx::all_of(mask));
        assert(!stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 64);
        assert(stdx::reduce_min_index(mask) == 0);
        assert(stdx::reduce_max_index(mask) == 63);
      }
      { // none are true
        auto mask = simd_utils::make_mask<64>(
            {false, false, false, false, false, false, false, false, false, false, false, false, false,
             false, false, false, false, false, false, false, false, false, false, false, false, false,
             false, false, false, false, false, false, false, false, false, false, false, false, false,
             false, false, false, false, false, false, false, false, false, false, false, false, false,
             false, false, false, false, false, false, false, false, false, false, false, false});
        assert(!stdx::any_of(mask));
        assert(!stdx::all_of(mask));
        assert(stdx::none_of(mask));
        assert(stdx::reduce_count(mask) == 0);
        // no reduce_{min,max}_index, since the precondition isn't met.
      }
    }
  });

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;
}
