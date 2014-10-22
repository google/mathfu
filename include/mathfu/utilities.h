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
#include <assert.h>

#include <algorithm>
#include <memory>

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

// Unroll an loop up to 4 iterations, where iterator is the identifier
// used in each operation (e.g "i"), number_of_iterations is a constant which
// specifies the number of times to perform the operation and "operation" is
// the statement to execute for each iteration of the loop (e.g data[i] = v).
#define MATHFU_UNROLLED_LOOP(iterator, number_of_iterations, operation) \
  { \
    const int iterator = 0;  { operation ; } \
    if ((number_of_iterations) > 1) { \
      const int iterator = 1;  { operation ; } \
      if ((number_of_iterations) > 2) { \
        const int iterator = 2;  { operation ; } \
        if ((number_of_iterations) > 3) { \
          const int iterator = 3;  { operation ; } \
          if ((number_of_iterations) > 4) { \
            for (int iterator = 4; iterator < (number_of_iterations); \
                 ++iterator) { \
              operation ; \
            } \
          } \
        } \
      } \
    } \
  }

namespace mathfu {

/// Clamp x within [lower, upper]. Results are undefined if lower > upper.
template<class T>
T Clamp(const T& x, const T& lower, const T& upper) {
  return std::max<T>(lower, std::min<T>(x, upper));
}

// Interpolates between range_start and range_end, based on percent.
template<class T>
T Lerp(const T& range_start, const T& range_end, const T& percent) {
  const T one_minus_percent = static_cast<T>(1.0) - percent;
  return range_start * one_minus_percent + range_end * percent;
}

/// Generate a random value of type T greater than or equal to 0.0 and
/// less than 1.0.  This function uses rand() from math.h to generate the
/// random number.
template<class T> inline T Random() {
  return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}

template<> inline float Random() {
  return static_cast<float>(rand() >> 8) /
      (static_cast<float>((RAND_MAX >> 8) + 1));
}

template<> inline double Random() {
  return static_cast<double>(rand()) / (static_cast<double>(RAND_MAX + 1LL));
}

/// Generate a random value of type T in the range -range...+range
/// This function uses rand() from math.h to generate the random number.
template<class T> inline T RandomRange(T range) {
  return (Random<T>() * range * 2) - range;
}

/// Returns a random number between [range_start, range_end]
/// This function uses rand() from math.h to generate the random number.
template<class T> inline T RandomInRange(T range_start, T range_end) {
  return Lerp(range_start, range_end, Random<T>());
}

template<> inline int RandomInRange<int>(int range_start, int range_end) {
    return static_cast<int>(RandomInRange<float>(
                              static_cast<float>(range_start),
                              static_cast<float>(range_end)));
}

template<class T> T RoundUpToPowerOf2(T x) {
  return pow(static_cast<T>(2), ceil(log(x) / log(static_cast<T>(2))));
}

// If you use MathFU with SIMD (SSE in particular), you need to have all
// your allocations be 16-byte aligned (which isn't the case with the default
// allocators on most platforms except OS X).

// You can either use simd_allocator below, which solves the problem for
// any STL containers, but not for manual dynamic allocations.
// The new/delete override MATHFU_DEFINE_GLOBAL_SIMD_AWARE_NEW_DELETE will solve
// it for all allocations, at the cost of 16 bytes per allocation.

#define MATHFU_ALIGNMENT 16

inline void *AllocateAligned(size_t n) {
  // We need to allocate extra bytes to guarantee alignment,
  // and to store the pointer to the original buffer.
  uint8_t* buf = reinterpret_cast<uint8_t *>(malloc(n + MATHFU_ALIGNMENT));
  // Align to next higher multiple of MATHFU_ALIGNMENT.
  uint8_t* aligned_buf = reinterpret_cast<uint8_t *>(
                       (reinterpret_cast<size_t>(buf) + MATHFU_ALIGNMENT) &
                        ~(MATHFU_ALIGNMENT - 1));
  // Write out original buffer pointer before aligned buffer.
  // The assert will fail if the allocator granularity is less than the pointer
  // size, or if MATHFU_ALIGNMENT doesn't fit two pointers.
  assert(static_cast<size_t>(aligned_buf - buf) > sizeof(void *));
  *(reinterpret_cast<uint8_t **>(aligned_buf) - 1) = buf;
  return aligned_buf;
}

inline void FreeAligned(void *p) {
  free(*(reinterpret_cast<uint8_t **>(p) - 1));
}

// A SIMD-safe memory allocator, for use with std::vector.
// e.g.
// std::vector<vec4, mathfu::simd_allocator<vec4>> myvector
template <typename T> class simd_allocator : public std::allocator<T> {
 public:
  typedef size_t size_type;
  typedef T *pointer;
  typedef const T *const_pointer;

  simd_allocator() throw(): std::allocator<T>() {}
  simd_allocator(const simd_allocator &a) throw() : std::allocator<T>(a) {}
  template <class U> simd_allocator(const simd_allocator<U> &a) throw()
      : std::allocator<T>(a) {}
  ~simd_allocator() throw() {}

  template<typename _Tp1> struct rebind { typedef simd_allocator<_Tp1> other; };

  pointer allocate(size_type n) {
    return reinterpret_cast<pointer>(AllocateAligned(n * sizeof(T)));
  }

  void deallocate(pointer p, size_type) { FreeAligned(p); }
};

#if defined(_MSC_VER)
#if _MSC_VER <= 1700  // MSVC 2012
#if !defined(noexcept)
#define noexcept
#endif  // !defined(noexcept)
#endif  // _MSC_VER <= 1700
#endif  //  defined(_MSC_VER)

// To globally override new and delete, simply add a line saying just
// MATHFU_DEFINE_GLOBAL_SIMD_AWARE_NEW_DELETE to the end of your main .cpp file.
#define MATHFU_DEFINE_GLOBAL_SIMD_AWARE_NEW_DELETE \
  void *operator new(std::size_t n) { return mathfu::AllocateAligned(n); } \
  void *operator new[](std::size_t n) { return mathfu::AllocateAligned(n); } \
  void operator delete(void *p) noexcept { mathfu::FreeAligned(p); } \
  void operator delete[](void *p) noexcept { mathfu::FreeAligned(p); }

} // namespace mathfu

#endif  // MATHFU_UTILITIES_H_
