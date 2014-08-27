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
#ifndef MATHFU_UTILITIES_H_
#define MATHFU_UTILITIES_H_

#include <math.h>
#include <stdlib.h>

#include <algorithm>

// Enable SIMD compile based upon the target architecture and compiler options.
#if !defined(MATHFU_COMPILE_WITHOUT_SIMD_SUPPORT)
#  if defined(__SSE__)
#    define MATHFU_COMPILE_WITH_SIMD
#  elif defined(__ARM_NEON__)
#    define MATHFU_COMPILE_WITH_SIMD
#  elif defined(_M_IX86_FP)  // MSVC
#    if _M_IX86_FP >= 1 // SSE enabled
#      define MATHFU_COMPILE_WITH_SIMD
#    endif  // _M_IX86_FP >= 1
#  endif
#endif  // !defined(MATHFU_COMPILE_WITHOUT_SIMD_SUPPORT)

#ifdef MATHFU_COMPILE_WITH_SIMD
#  define MATHFU_CAST union_reinterpret_cast
#else
#  define MATHFU_CAST reinterpret_cast
#endif

// Enable padding of some data structures to be more efficient with SIMD
// operations.
#ifdef MATHFU_COMPILE_WITH_SIMD
#  define MATHFU_COMPILE_WITH_PADDING

// If MATHFU_COMPILE_FORCE_PADDING is defined, enable / disable padding of
// data structures depending upon the value of the symbol.
#  if defined(MATHFU_COMPILE_FORCE_PADDING)
#    if MATHFU_COMPILE_FORCE_PADDING == 1
#      if !defined(MATHFU_COMPILE_WITH_PADDING)
#        define MATHFU_COMPILE_WITH_PADDING
#      endif // !defined(MATHFU_COMPILE_WITH_PADDING)
#    else
#      if defined(MATHFU_COMPILE_WITH_PADDING)
#        undef MATHFU_COMPILE_WITH_PADDING
#      endif  // MATHFU_COMPILE_WITH_PADDING
#    endif  // MATHFU_COMPILE_FORCE_PADDING == 1
#  endif  // MATHFU_COMPILE_FORCE_PADDING
#endif  // MATHFU_COMPILE_WITH_SIMD

#define MATHFU_VERSION_MAJOR 1
#define MATHFU_VERSION_MINOR 0
#define MATHFU_VERSION_REVISION 0
#define MATHFU_STRING_EXPAND(X) #X
#define MATHFU_STRING(X) MATHFU_STRING_EXPAND(X)

// Generate string which contains build options for the library.
#if defined(MATHFU_COMPILE_WITH_SIMD)
#define MATHFU_BUILD_OPTIONS_SIMD "[simd]"
#else
#define MATHFU_BUILD_OPTIONS_SIMD "[no simd]"
#endif  // defined(MATHFU_COMPILE_WITH_SIMD)
#if defined(MATHFU_COMPILE_WITH_PADDING)
#define MATHFU_BUILD_OPTIONS_PADDING "[padding]"
#else
#define MATHFU_BUILD_OPTIONS_PADDING "[no padding]"
#endif  // defined(MATHFU_COMPILE_WITH_PADDING)
#define MATHFU_BUILD_OPTIONS_STRING \
  (MATHFU_BUILD_OPTIONS_SIMD " " MATHFU_BUILD_OPTIONS_PADDING)

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
#define MATHFU_STATIC_ASSERT(x) static_assert_util<(x)>()

// This is done to avoid aliasing issues.
// http://cellperformance.beyond3d.com/articles/2006/06/understanding-strict-aliasing.html
template<class T1, class T2>
union ConversionUnion {
  T1 data1;
  T2 data2;
};

template<class T1, class T2>
static T1& union_reinterpret_cast(T2& data) {
  return reinterpret_cast<ConversionUnion<T1, T2> *>(&data)->data1;
}

template<class T1, class T2>
static const T1& union_reinterpret_cast(const T2& data) {
  return reinterpret_cast<const ConversionUnion<T1, T2> *>(&data)->data1;
}

namespace mathfu {

/// Clamp x within [lower, upper]. Results are undefined if lower > upper.
template<class T>
T Clamp(const T& x, const T& lower, const T& upper) {
  return std::max<T>(lower, std::min<T>(x, upper));
}

/// Generate a random value of type T in the range 0.0...1.0.
/// This function uses rand() from math.h to generate the random number.
template<class T> inline T Random() {
  return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}

/// Generate a random value of type T in the range -range...+range
/// This function uses rand() from math.h to generate the random number.
template<class T> inline T RandomRange(T range) {
  return (Random<T>() * range * 2) - range;
}

} // namespace mathfu

#endif  // MATHFU_UTILITIES_H_
