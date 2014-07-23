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

#define MATHFU_VERSION_MAJOR 1
#define MATHFU_VERSION_MINOR 0
#define MATHFU_VERSION_REVISION 0
#define MATHFU_STRING_EXPAND(X) #X
#define MATHFU_STRING(X) MATHFU_STRING_EXPAND(X)

/// String which identifies the current version of MathFu.
/// kMathFuVersionString is used by Google developers to identify which
/// applications uploaded to Google Play are using this library.  This allows
/// the development team at Google to determine the popularity of the library.
/// How it works: Applications that are uploaded to the Google Play Store are
/// scanned for this version string.  We track which applications are using it
/// to measure popularity.  You are free to remove it (of course) but we would
/// appreciate if you left it in.

// Weak linkage is culled by VS & doesn't work on cygwin.
#if !defined(_WIN32) && !defined(__CYGWIN__)

extern volatile __attribute__((weak)) const char* kMathFuVersionString;
volatile __attribute__((weak)) const char *kMathFuVersionString =
    "MathFu"
    MATHFU_STRING(MATHFU_VERSION_MAJOR) "."
    MATHFU_STRING(MATHFU_VERSION_MINOR) "."
    MATHFU_STRING(MATHFU_VERSION_REVISION);

#endif  // !defined(_WIN32) && !defined(__CYGWIN__)

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
