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
#ifndef MATHFU_INTERNAL_VECTOR_TYPES_H_
#define MATHFU_INTERNAL_VECTOR_TYPES_H_

#include "mathfu/internal/simd.h"

/// @file mathfu/vector.h Vector
/// @brief Data structures that can be used to describe vectors of different
/// types and size
/// @addtogroup mathfu_vector

namespace mathfu {
namespace internal {

// Suppress pedantic warnings.
// We need this to use anonymous unions and structs, which generate the
// the following warning in GCC and Clang,
//     error: ISO C++ prohibits anonymous structs [-Werror=pedantic]
// The only way to suppress this warning is to turn off all pedantic warnings.
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#endif  // defined(__GNUC__)

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpedantic"
#pragma clang diagnostic ignored "-Wignored-qualifiers"
#endif  // defined(__clang__)

// A generic base class for all vector types.
template <class T, int Dims>
struct VectorBase {
  T data_[Dims];
};

// VectorBase specialization for 2D vectors that provides x,y members.
template <class T>
struct VectorBase<T, 2> {
  union {
    T data_[2];
    struct {
      T x;
      T y;
    };
  };
};

// VectorBase specialization for 3D vectors that provides x,y,z members.
template <class T>
struct VectorBase<T, 3> {
  union {
    T data_[3];
    struct {
      T x;
      T y;
      T z;
    };
  };
};

// VectorBase specialization for 4D vectors that provides x,y,z,w members.
template <class T>
struct VectorBase<T, 4> {
  union {
    T data_[4];
    struct {
      T x;
      T y;
      T z;
      T w;
    };
  };
};

#ifdef MATHFU_COMPILE_WITH_SIMD

// We only provide SIMD-specific specializations for 2D, 3D, and 4D floating
// point vectors.  All other vectors are the same as non-SIMD vectors when
// compiling with SIMD enabled.
template <class T, int Dims>
struct SimdVectorBase : public VectorBase<T, Dims> {};

#ifdef MATHFU_SIMD2_ENABLED

// SimdVectorBase specialization for 2D vectors that provides x,y members.
template <>
struct SimdVectorBase<float, 2> {
  union {
    simd2f simd;
    simd2f simd2;
    float data_[2];
    struct {
      float x;
      float y;
    };
  };
};

#endif

#ifdef MATHFU_SIMD4_ENABLED

// SimdVectorBase specialization for 3D vectors that provides x,y,z members.
template <>
struct SimdVectorBase<float, 3> {
  union {
    simd4f simd;
    simd4f simd3;
    float data_[3];
    struct {
      float x;
      float y;
      float z;
    };
  };
};

// SimdVectorBase specialization for 4D vectors that provides x,y,z,w members.
template <>
struct SimdVectorBase<float, 4> {
  union {
    simd4f simd;
    simd4f simd4;
    float data_[4];
    struct {
      float x;
      float y;
      float z;
      float w;
    };
  };
};

#endif

#endif  // MATHFU_COMPILE_WITH_SIMD

}  // namespace internal
}  // namespace mathfu

// Restore disabled warnings.
#if defined(__clang__)
#pragma clang diagnostic pop
#endif  // defined(__clang__)

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif  // defined(__GNUC__)

#endif  // MATHFU_INTERNAL_VECTOR_TYPES_H_
