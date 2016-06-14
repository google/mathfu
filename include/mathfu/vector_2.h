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
#ifndef MATHFU_VECTOR_2_H_
#define MATHFU_VECTOR_2_H_

#include "mathfu/utilities.h"
#include "mathfu/vector.h"

#include <math.h>

/// @file mathfu/vector_2.h MathFu Vector<T, 2> Specialization
/// @brief 2-dimensional specialization of mathfu::Vector for SIMD optimized
/// builds.
/// @see mathfu::Vector

#if !defined(MATHFU_COMPILE_WITHOUT_SIMD_SUPPORT) && defined(__ARM_NEON__)
#include <vectorial/simd2f.h>
#endif

namespace mathfu {

#if !defined(MATHFU_COMPILE_WITHOUT_SIMD_SUPPORT) && defined(__ARM_NEON__)
/// @cond MATHFU_INTERNAL
template <>
class Vector<float, 2> {
 public:
  typedef float Scalar;

  inline Vector() {}

  inline Vector(const Vector<float, 2>& v) { data_.simd = v.data_.simd; }

  explicit inline Vector(const Vector<int, 2>& v) {
    data_.float_array[0] = static_cast<float>(v[0]);
    data_.float_array[1] = static_cast<float>(v[1]);
  }

  explicit inline Vector(const simd2f& v) { data_.simd = v; }

  explicit inline Vector(const float& s) { data_.simd = simd2f_create(s, s); }

  inline Vector(const float& s1, const float& s2) {
    data_.simd = simd2f_create(s1, s2);
  }

  explicit inline Vector(const float* v) {
    data_.simd = simd2f_create(v[0], v[1]);
  }

  explicit inline Vector(const VectorPacked<float, 2>& vector) {
    data_.simd = simd2f_uload2(vector.data);
  }

  inline float& operator()(const int i) { return data_.float_array[i]; }

  inline const float& operator()(const int i) const {
    return data_.float_array[i];
  }

  inline float& operator[](const int i) { return data_.float_array[i]; }

  inline const float& operator[](const int i) const {
    return data_.float_array[i];
  }

  inline float& x() { return (*this)[0]; }
  inline float& y() { return (*this)[1]; }

  inline const float& x() const { return (*this)[0]; }
  inline const float& y() const { return (*this)[1]; }

  inline void Pack(VectorPacked<float, 2>* const vector) const {
    simd2f_ustore2(data_.simd, vector->data);
  }

  inline Vector<float, 2> operator-() const {
    return Vector<float, 2>(simd2f_sub(simd2f_zero(), data_.simd));
  }

  inline Vector<float, 2> operator*(const Vector<float, 2>& v) const {
    return Vector<float, 2>(simd2f_mul(data_.simd, v.data_.simd));
  }

  inline Vector<float, 2> operator/(const Vector<float, 2>& v) const {
    return Vector<float, 2>(simd2f_div(data_.simd, v.data_.simd));
  }

  inline Vector<float, 2> operator+(const Vector<float, 2>& v) const {
    return Vector<float, 2>(simd2f_add(data_.simd, v.data_.simd));
  }

  inline Vector<float, 2> operator-(const Vector<float, 2>& v) const {
    return Vector<float, 2>(simd2f_sub(data_.simd, v.data_.simd));
  }

  inline Vector<float, 2> operator*(const float& s) const {
    return Vector<float, 2>(simd2f_mul(data_.simd, simd2f_splat(s)));
  }

  inline Vector<float, 2> operator/(const float& s) const {
    return Vector<float, 2>(simd2f_div(data_.simd, simd2f_splat(s)));
  }

  inline Vector<float, 2> operator+(const float& s) const {
    return Vector<float, 2>(simd2f_add(data_.simd, simd2f_splat(s)));
  }

  inline Vector<float, 2> operator-(const float& s) const {
    return Vector<float, 2>(simd2f_sub(data_.simd, simd2f_splat(s)));
  }

  inline Vector<float, 2>& operator*=(const Vector<float, 2>& v) {
    data_.simd = simd2f_mul(data_.simd, v.data_.simd);
    return *this;
  }

  inline Vector<float, 2>& operator/=(const Vector<float, 2>& v) {
    data_.simd = simd2f_div(data_.simd, v.data_.simd);
    return *this;
  }

  inline Vector<float, 2>& operator+=(const Vector<float, 2>& v) {
    data_.simd = simd2f_add(data_.simd, v.data_.simd);
    return *this;
  }

  inline Vector<float, 2>& operator-=(const Vector<float, 2>& v) {
    data_.simd = simd2f_sub(data_.simd, v.data_.simd);
    return *this;
  }

  inline Vector<float, 2>& operator*=(const float& s) {
    data_.simd = simd2f_mul(data_.simd, simd2f_splat(s));
    return *this;
  }

  inline Vector<float, 2>& operator/=(const float& s) {
    data_.simd = simd2f_div(data_.simd, simd2f_splat(s));
    return *this;
  }

  inline Vector<float, 2>& operator+=(const float& s) {
    data_.simd = simd2f_add(data_.simd, simd2f_splat(s));
    return *this;
  }

  inline Vector<float, 2>& operator-=(const float& s) {
    data_.simd = simd2f_sub(data_.simd, simd2f_splat(s));
    return *this;
  }

  inline bool operator==(const Vector<float, 2>& v) const {
    for (int i = 0; i < 2; ++i) {
      if ((*this)[i] != v[i]) return false;
    }
    return true;
  }

  inline bool operator!=(const Vector<float, 2>& v) const {
    return !operator==(v);
  }

  inline float LengthSquared() const {
    return simd2f_get_x(simd2f_dot2(data_.simd, data_.simd));
  }

  inline float Length() const {
    return simd2f_get_x(simd2f_length2(data_.simd));
  }

  inline float Normalize() {
    const float length = Length();
    data_.simd = simd2f_mul(data_.simd, simd2f_splat(1 / length));
    return length;
  }

  inline Vector<float, 2> Normalized() const {
    return Vector<float, 2>(simd2f_normalize2(data_.simd));
  }

  static inline float DotProduct(const Vector<float, 2>& v1,
                                 const Vector<float, 2>& v2) {
    return simd2f_get_x(simd2f_dot2(v1.data_.simd, v2.data_.simd));
  }

  static inline Vector<float, 2> HadamardProduct(const Vector<float, 2>& v1,
                                                 const Vector<float, 2>& v2) {
    return Vector<float, 2>(simd2f_mul(v1.data_.simd, v2.data_.simd));
  }

  static inline Vector<float, 2> Lerp(const Vector<float, 2>& v1,
                                      const Vector<float, 2>& v2,
                                      float percent) {
    const Vector<float, 2> percentv(percent);
    const Vector<float, 2> one_minus_percent(
        simd2f_sub(simd2f_splat(1.0f), percentv.data_.simd));
    return Vector<float, 2>(
        simd2f_add(simd2f_mul(one_minus_percent.data_.simd, v1.data_.simd),
                   simd2f_mul(percentv.data_.simd, v2.data_.simd)));
  }

  /// Generates a random vector, where the range for each component is
  /// bounded by min and max.
  static inline Vector<float, 2> RandomInRange(const Vector<float, 2>& min,
                                               const Vector<float, 2>& max) {
    return Vector<float, 2>(mathfu::RandomInRange<float>(min[0], max[0]),
                            mathfu::RandomInRange<float>(min[1], max[1]));
  }

  static inline Vector<float, 2> Max(const Vector<float, 2>& v1,
                                     const Vector<float, 2>& v2) {
    return Vector<float, 2>(std::max(v1[0], v2[0]), std::max(v1[1], v2[1]));
  }

  static inline Vector<float, 2> Min(const Vector<float, 2>& v1,
                                     const Vector<float, 2>& v2) {
    return Vector<float, 2>(std::min(v1[0], v2[0]), std::min(v1[1], v2[1]));
  }

  MATHFU_DEFINE_CLASS_SIMD_AWARE_NEW_DELETE

 private:
  union {
    simd2f simd;
    float float_array[2];
  } data_;
};
/// @endcond
#endif  // !defined(MATHFU_COMPILE_WITHOUT_SIMD_SUPPORT) &&
        // defined(__ARM_NEON__)

}  // namespace mathfu

#endif  // MATHFU_VECTOR_2_H_
