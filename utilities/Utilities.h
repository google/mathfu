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
#ifndef GMATH_UTILITIES_UTILITIES_H_
#define GMATH_UTILITIES_UTILITIES_H_

#if !defined(COMPILE_WITHOUT_SIMD_SUPPORT) && \
  (defined(__SSE__) || defined(__ARM_NEON__))
#define COMPILE_WITH_SIMD
#endif

#ifdef COMPILE_WITH_SIMD
#define CAST union_reinterpret_cast
#else
#define CAST reinterpret_cast
#endif

template<bool> struct static_assert_util;
template<> struct static_assert_util<true> {};
#define STATIC_ASSERT(x) static_assert_util<(x)>()

// This is done to avoid aliasing issues.
// http://cellperformance.beyond3d.com/articles/2006/06/understanding-strict-aliasing.html
template<class T1, class T2>
T1 union_reinterpret_cast(T2 data) {
  typedef union {
    T2 data2;
    T1 data1;
  } conversion_union;
  conversion_union local_union = {data};
  return local_union.data1;
}

#endif  // GMATH_UTILITIES_UTILITIES_H_
