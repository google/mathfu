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
#ifndef MATHFU_VECTOR_4_SIMD_H_
#define MATHFU_VECTOR_4_SIMD_H_

#include "mathfu/internal/vector_4.h"
#include "mathfu/utilities.h"

#include <math.h>

#ifdef MATHFU_COMPILE_WITH_SIMD
#include "vectorial/simd4f.h"
#endif

/// @file mathfu/internal/vector_4_simd.h MathFu Vector<T, 4> Specialization
/// @brief 4-dimensional specialization of mathfu::Vector for SIMD optimized
/// builds.
/// @see mathfu::Vector

namespace mathfu {

#ifdef MATHFU_COMPILE_WITH_SIMD

template <>
class Vector<float, 4> {
 public:
  typedef float Scalar;

  inline Vector() {}

  inline Vector(const Vector<float, 4>& v) { simd4 = v.simd4; }

  explicit inline Vector(const Vector<int, 4>& v) {
    data_[0] = static_cast<float>(v[0]);
    data_[1] = static_cast<float>(v[1]);
    data_[2] = static_cast<float>(v[2]);
    data_[3] = static_cast<float>(v[3]);
  }

  explicit inline Vector(const simd4f& v) { simd4 = v; }

  explicit inline Vector(const float& s) { simd4 = simd4f_splat(s); }

  inline Vector(const float& s1, const float& s2, const float& s3,
                const float& s4) {
    simd4 = simd4f_create(s1, s2, s3, s4);
  }

  explicit inline Vector(const float* v) { simd4 = simd4f_uload4(v); }

  inline Vector(const Vector<float, 3>& vector3, const float& value) {
#ifdef MATHFU_COMPILE_WITH_PADDING
    simd4 = vector3.simd3;
    (*this)[3] = value;
#else
    simd4 = simd4f_create(vector3[0], vector3[1], vector3[2], value);
#endif  // MATHFU_COMPILE_WITH_PADDING
  }

  inline Vector(const Vector<float, 2>& vector12,
                const Vector<float, 2>& vector34) {
    simd4 = simd4f_create(vector12[0], vector12[1], vector34[0], vector34[1]);
  }

  explicit inline Vector(const VectorPacked<float, 4>& vector) {
    simd4 = simd4f_uload4(vector.data_);
  }

  inline float& operator()(const int i) { return data_[i]; }

  inline const float& operator()(const int i) const { return data_[i]; }

  inline float& operator[](const int i) { return data_[i]; }

  inline const float& operator[](const int i) const { return data_[i]; }

  /// GLSL style multi-component accessors.
  inline Vector<float, 3> xyz() { return Vector<float, 3>(x, y, z); }
  inline const Vector<float, 3> xyz() const {
    return Vector<float, 3>(x, y, z);
  }

  inline Vector<float, 2> xy() { return Vector<float, 2>(x, y); }
  inline const Vector<float, 2> xy() const { return Vector<float, 2>(x, y); }

  inline Vector<float, 2> zw() { return Vector<float, 2>(z, w); }
  inline const Vector<float, 2> zw() const { return Vector<float, 2>(z, w); }

  inline void Pack(VectorPacked<float, 4>* const vector) const {
    simd4f_ustore4(simd4, vector->data_);
  }

  inline Vector<float, 4> operator-() const {
    return Vector<float, 4>(simd4f_sub(simd4f_zero(), simd4));
  }

  inline Vector<float, 4> operator*(const Vector<float, 4>& v) const {
    return Vector<float, 4>(simd4f_mul(simd4, v.simd4));
  }

  inline Vector<float, 4> operator/(const Vector<float, 4>& v) const {
    return Vector<float, 4>(simd4f_div(simd4, v.simd4));
  }

  inline Vector<float, 4> operator+(const Vector<float, 4>& v) const {
    return Vector<float, 4>(simd4f_add(simd4, v.simd4));
  }

  inline Vector<float, 4> operator-(const Vector<float, 4>& v) const {
    return Vector<float, 4>(simd4f_sub(simd4, v.simd4));
  }

  inline Vector<float, 4> operator*(const float& s) const {
    return Vector<float, 4>(simd4f_mul(simd4, simd4f_splat(s)));
  }

  inline Vector<float, 4> operator/(const float& s) const {
    return Vector<float, 4>(simd4f_div(simd4, simd4f_splat(s)));
  }

  inline Vector<float, 4> operator+(const float& s) const {
    return Vector<float, 4>(simd4f_add(simd4, simd4f_splat(s)));
  }

  inline Vector<float, 4> operator-(const float& s) const {
    return Vector<float, 4>(simd4f_sub(simd4, simd4f_splat(s)));
  }

  inline Vector<float, 4>& operator*=(const Vector<float, 4>& v) {
    simd4 = simd4f_mul(simd4, v.simd4);
    return *this;
  }

  inline Vector<float, 4>& operator/=(const Vector<float, 4>& v) {
    simd4 = simd4f_div(simd4, v.simd4);
    return *this;
  }

  inline Vector<float, 4>& operator+=(const Vector<float, 4>& v) {
    simd4 = simd4f_add(simd4, v.simd4);
    return *this;
  }

  inline Vector<float, 4>& operator-=(const Vector<float, 4>& v) {
    simd4 = simd4f_sub(simd4, v.simd4);
    return *this;
  }

  inline Vector<float, 4>& operator*=(const float& s) {
    simd4 = simd4f_mul(simd4, simd4f_splat(s));
    return *this;
  }

  inline Vector<float, 4>& operator/=(const float& s) {
    simd4 = simd4f_div(simd4, simd4f_splat(s));
    return *this;
  }

  inline Vector<float, 4>& operator+=(const float& s) {
    simd4 = simd4f_add(simd4, simd4f_splat(s));
    return *this;
  }

  inline Vector<float, 4>& operator-=(const float& s) {
    simd4 = simd4f_sub(simd4, simd4f_splat(s));
    return *this;
  }

  inline bool operator==(const Vector<float, 4>& v) const {
    for (int i = 0; i < 4; ++i) {
      if ((*this)[i] != v[i]) return false;
    }
    return true;
  }

  inline bool operator!=(const Vector<float, 4>& v) const {
    return !operator==(v);
  }

  inline float LengthSquared() const {
    return simd4f_get_x(simd4f_dot4(simd4, simd4));
  }

  inline float Length() const { return simd4f_get_x(simd4f_length4(simd4)); }

  inline float Normalize() {
    const float length = Length();
    simd4 = simd4f_mul(simd4, simd4f_splat(1 / length));
    return length;
  }

  inline Vector<float, 4> Normalized() const {
    return Vector<float, 4>(simd4f_normalize4(simd4));
  }

  template <typename CompatibleT>
  static inline Vector<float, 4> FromType(const CompatibleT& compatible) {
    return FromTypeHelper<float, 4, CompatibleT>(compatible);
  }

  template <typename CompatibleT>
  static inline CompatibleT ToType(const Vector<float, 4>& v) {
    return ToTypeHelper<float, 4, CompatibleT>(v);
  }

  static inline float DotProduct(const Vector<float, 4>& v1,
                                 const Vector<float, 4>& v2) {
    return simd4f_get_x(simd4f_dot4(v1.simd4, v2.simd4));
  }

  static inline Vector<float, 4> HadamardProduct(const Vector<float, 4>& v1,
                                                 const Vector<float, 4>& v2) {
    return Vector<float, 4>(simd4f_mul(v1.simd4, v2.simd4));
  }

  static inline Vector<float, 4> Lerp(const Vector<float, 4>& v1,
                                      const Vector<float, 4>& v2,
                                      float percent) {
    const Vector<float, 4> percentv(percent);
    const Vector<float, 4> one(1.0f);
    const Vector<float, 4> one_minus_percent = one - percentv;
    return Vector<float, 4>(
        simd4f_add(simd4f_mul(one_minus_percent.simd4, v1.simd4),
                   simd4f_mul(percentv.simd4, v2.simd4)));
  }

  /// Generates a random vector, where the range for each component is
  /// bounded by min and max.
  static inline Vector<float, 4> RandomInRange(const Vector<float, 4>& min,
                                               const Vector<float, 4>& max) {
    return Vector<float, 4>(mathfu::RandomInRange<float>(min[0], max[0]),
                            mathfu::RandomInRange<float>(min[1], max[1]),
                            mathfu::RandomInRange<float>(min[2], max[2]),
                            mathfu::RandomInRange<float>(min[3], max[3]));
  }

  static inline Vector<float, 4> Max(const Vector<float, 4>& v1,
                                     const Vector<float, 4>& v2) {
    return Vector<float, 4>(simd4f_max(v1.simd4, v2.simd4));
  }

  static inline Vector<float, 4> Min(const Vector<float, 4>& v1,
                                     const Vector<float, 4>& v2) {
    return Vector<float, 4>(simd4f_min(v1.simd4, v2.simd4));
  }

  static inline float Distance(const Vector<float, 4>& v1,
                               const Vector<float, 4>& v2) {
    return (v1 - v2).Length();
  }

  static inline float DistanceSquared(const Vector<float, 4>& v1,
                                      const Vector<float, 4>& v2) {
    return (v1 - v2).LengthSquared();
  }

  static inline float Angle(const Vector<float, 4>& v1,
                            const Vector<float, 4>& v2) {
    return AngleHelper(v1, v2);
  }

  template <class T, int rows, int cols>
  friend class Matrix;

  MATHFU_DEFINE_CLASS_SIMD_AWARE_NEW_DELETE

#include "mathfu/internal/disable_warnings_begin.h"
  union {
    simd4f simd4;
    float data_[4];
    struct {
      float x;
      float y;
      float z;
      float w;
    };
  };
#include "mathfu/internal/disable_warnings_end.h"
};
/// @endcond
#endif  // MATHFU_COMPILE_WITH_SIMD

}  // namespace mathfu

#endif  // MATHFU_VECTOR_4_SIMD_H_
