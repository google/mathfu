/*
* Copyright 2014 Google Inc. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#ifndef MATHFU_INTERNAL_SIMD_H_
#define MATHFU_INTERNAL_SIMD_H_

#include <stdint.h>
#include "mathfu/utilities.h"

#ifdef MATHFU_COMPILE_WITH_SIMD
#if !defined(MATHFU_COMPILE_WITHOUT_SIMD_SUPPORT) && defined(__ARM_NEON__)
#define MATHFU_SIMD2_ENABLED
#include "vectorial/simd2f.h"
#endif
#define MATHFU_SIMD4_ENABLED
#include "vectorial/simd4f.h"
#endif  // MATHFU_COMPILE_WITH_SIMD

// This file contains types and functions that enable SIMD functionality in
// mathfu using the vectorial library.

namespace mathfu {
namespace internal {

// To simplify usage of the vectorial library, we're going to provide more
// generic implementations of some functions.  For example, we can do this:
//     auto c = simd_add(a, b);
// with either simd2f or simd4f types and things will just work.
//
// But, first, we provide some dummy unimplemented templated versions of these
// functions so that we don't have to wrap everything inside
// MATHFU_COMPILE_WITH_SIMD checks everywhere.  We leave these unimplented to
// ensure we're never actually using them, but provide them to silence the
// compiler.
template <typename T> T simd_zero(T);
template <typename T> T simd_add(T, T);
template <typename T> T simd_sub(T, T);
template <typename T> T simd_mul(T, T);
template <typename T> T simd_div(T, T);
template <typename T> T simd_min(T, T);
template <typename T> T simd_max(T, T);
template <typename T> T simd_splat(T, float);

// Now we implement overloads of those functions with both simd2f (if enabled)
// and simd4f support.
#ifdef MATHFU_COMPILE_WITH_SIMD
#ifdef MATHFU_SIMD2_ENABLED
inline simd2f simd_zero(simd2f tmp) { return simd2f_zero(); }
inline simd2f simd_add(simd2f lhs, simd2f rhs) { return simd2f_add(lhs, rhs); }
inline simd2f simd_sub(simd2f lhs, simd2f rhs) { return simd2f_sub(lhs, rhs); }
inline simd2f simd_mul(simd2f lhs, simd2f rhs) { return simd2f_mul(lhs, rhs); }
inline simd2f simd_div(simd2f lhs, simd2f rhs) { return simd2f_div(lhs, rhs); }
inline simd2f simd_min(simd2f lhs, simd2f rhs) { return simd2f_min(lhs, rhs); }
inline simd2f simd_max(simd2f lhs, simd2f rhs) { return simd2f_max(lhs, rhs); }
inline simd2f simd_splat(simd2f tmp, float v) { return simd2f_splat(v); }
#endif  // MATHFU_SIMD2_ENABLED
inline simd4f simd_zero(simd4f tmp) { return simd4f_zero(); }
inline simd4f simd_add(simd4f lhs, simd4f rhs) { return simd4f_add(lhs, rhs); }
inline simd4f simd_sub(simd4f lhs, simd4f rhs) { return simd4f_sub(lhs, rhs); }
inline simd4f simd_mul(simd4f lhs, simd4f rhs) { return simd4f_mul(lhs, rhs); }
inline simd4f simd_div(simd4f lhs, simd4f rhs) { return simd4f_div(lhs, rhs); }
inline simd4f simd_min(simd4f lhs, simd4f rhs) { return simd4f_min(lhs, rhs); }
inline simd4f simd_max(simd4f lhs, simd4f rhs) { return simd4f_max(lhs, rhs); }
inline simd4f simd_splat(simd4f tmp, float v) { return simd4f_splat(v); }
#endif  // MATHFU_COMPILE_WITH_SIMD

// Type traits to help specialize functions depending on the storage type of
// the underlying vector.  Uses SFINAE to help select the correct function
// for a given vector.  Typical usage would be:
//
//     template <class Vec, MATHFU_ENABLE_IF(scalar)>
//     Vec DoSomething(Vec a, Vec b) {
//       //  ... operate on data_ array ...
//     }
//     template <class Vec, MATHFU_ENABLE_IF(simd)>
//     Vec DoSomething(Vec a, Vec b) {
//       //  ... operate on simd data ...
//     }
template <typename U>
struct SimdType {
  template <typename T>
  static uint8_t test(decltype(&T::simd2));

  template <typename T>
  static uint16_t test(decltype(&T::simd3));

  template <typename T>
  static uint32_t test(decltype(&T::simd4));

  template <typename T>
  static uint64_t test(...);

  static constexpr bool simd2 = (sizeof(test<U>(0)) == sizeof(uint8_t));
  static constexpr bool simd3 = (sizeof(test<U>(0)) == sizeof(uint16_t));
  static constexpr bool simd4 = (sizeof(test<U>(0)) == sizeof(uint32_t));
  static constexpr bool scalar = (sizeof(test<U>(0)) == sizeof(uint64_t));
  static constexpr bool simd = simd2 || simd3 || simd4;
};

// A helper macro to make it easier to use the SimdType trait for enabled
// different implementations for functions.
#define MATHFU_ENABLE_IF(X) \
  typename std::enable_if<internal::SimdType<Vec>::X, int>::type = 0

}  // namespace internal
}  // namespace mathfu

#endif  // MATHFU_INTERNAL_SIMD_H_
