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
#ifndef MATHFU_VECTOR_3_H_
#define MATHFU_VECTOR_3_H_

#include "mathfu/utilities.h"
#include "mathfu/vector.h"

#include <math.h>

#ifdef MATHFU_COMPILE_WITH_SIMD
#include "vectorial/simd4f.h"
#endif

/// @file mathfu/vector_3.h MathFu Vector<T, 3> Specialization
/// @brief 3-dimensional specialization of mathfu::Vector for SIMD optimized
/// builds.
/// @see mathfu::Vector

/// @cond MATHFU_INTERNAL
/// Add macros to account for both the case where the vector is stored as a
/// simd intrinsic using 4 elements or as 3 values of type T.
/// MATHFU_VECTOR3_STORE3/MATHFU_VECTOR3_LOAD3 are additional operations used
/// to load/store the non simd values from and to simd datatypes. If intrinsics
/// are used these amount to essentially noops. MATHFU_VECTOR3_INIT3 either
/// creates a simd datatype if the intrinsic is used or sets the T values if
/// not.
#ifdef MATHFU_COMPILE_WITH_PADDING
#define MATHFU_VECTOR3_STORE3(simd_to_store, data) \
  { data.simd = simd_to_store; }
#define MATHFU_VECTOR3_LOAD3(data) data.simd
#define MATHFU_VECTOR3_INIT3(data, v1, v2, v3) \
  { data.simd = simd4f_create(v1, v2, v3, 0); }
#else
#define MATHFU_VECTOR3_STORE3(simd_to_store, data) \
  { simd4f_ustore3(simd_to_store, data.float_array); }
#define MATHFU_VECTOR3_LOAD3(data) simd4f_uload3(data.float_array)
#define MATHFU_VECTOR3_INIT3(data, v1, v2, v3) \
  {                                            \
    data.float_array[0] = v1;                  \
    data.float_array[1] = v2;                  \
    data.float_array[2] = v3;                  \
  }
#endif  // MATHFU_COMPILE_WITH_PADDING
/// @endcond

namespace mathfu {

#ifdef MATHFU_COMPILE_WITH_SIMD
/// @cond MATHFU_INTERNAL
// This class should remain plain old data.
template <>
class Vector<float, 3> {
 public:
  typedef float Scalar;

  inline Vector() {}

  inline Vector(const Vector<float, 3>& v) {
#ifdef MATHFU_COMPILE_WITH_PADDING
    data_.simd = v.data_.simd;
#else
    MATHFU_VECTOR3_INIT3(data_, v[0], v[1], v[2]);
#endif  // MATHFU_COMPILE_WITH_PADDING
  }

  explicit inline Vector(const Vector<int, 3>& v) {
    MATHFU_VECTOR3_INIT3(data_, static_cast<float>(v[0]),
                         static_cast<float>(v[1]), static_cast<float>(v[2]));
  }

  inline Vector(const simd4f& v) { MATHFU_VECTOR3_STORE3(v, data_); }

  explicit inline Vector(const float& s) {
    MATHFU_VECTOR3_INIT3(data_, s, s, s);
  }

  inline Vector(const float& v1, const float& v2, const float& v3) {
    MATHFU_VECTOR3_INIT3(data_, v1, v2, v3);
  }

  inline Vector(const Vector<float, 2>& v12, const float& v3) {
    MATHFU_VECTOR3_INIT3(data_, v12.x(), v12.y(), v3);
  }

  explicit inline Vector(const float* v) {
    MATHFU_VECTOR3_INIT3(data_, v[0], v[1], v[2]);
  }

  explicit inline Vector(const VectorPacked<float, 3>& vector) {
    MATHFU_VECTOR3_INIT3(data_, vector.data[0], vector.data[1], vector.data[2]);
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
  inline float& z() { return (*this)[2]; }

  inline const float& x() const { return (*this)[0]; }
  inline const float& y() const { return (*this)[1]; }
  inline const float& z() const { return (*this)[2]; }

  /// GLSL style multi-component accessors.
  inline Vector<float, 2> xy() { return Vector<float, 2>(x(), y()); }
  inline const Vector<float, 2> xy() const {
    return Vector<float, 2>(x(), y());
  }

  inline void Pack(VectorPacked<float, 3>* const vector) const {
#ifdef MATHFU_COMPILE_WITH_PADDING
    simd4f_ustore3(data_.simd, vector->data);
#else
    vector->data[0] = data_.float_array[0];
    vector->data[1] = data_.float_array[1];
    vector->data[2] = data_.float_array[2];
#endif  // MATHFU_COMPILE_WITH_PADDING
  }

  inline Vector<float, 3> operator-() const {
    return Vector<float, 3>(
        simd4f_sub(simd4f_zero(), MATHFU_VECTOR3_LOAD3(data_)));
  }

  inline Vector<float, 3> operator*(const Vector<float, 3>& v) const {
    return Vector<float, 3>(
        simd4f_mul(MATHFU_VECTOR3_LOAD3(data_), MATHFU_VECTOR3_LOAD3(v.data_)));
  }

  inline Vector<float, 3> operator/(const Vector<float, 3>& v) const {
    return Vector<float, 3>(
        simd4f_div(MATHFU_VECTOR3_LOAD3(data_), MATHFU_VECTOR3_LOAD3(v.data_)));
  }

  inline Vector<float, 3> operator+(const Vector<float, 3>& v) const {
    return Vector<float, 3>(
        simd4f_add(MATHFU_VECTOR3_LOAD3(data_), MATHFU_VECTOR3_LOAD3(v.data_)));
  }

  inline Vector<float, 3> operator-(const Vector<float, 3>& v) const {
    return Vector<float, 3>(
        simd4f_sub(MATHFU_VECTOR3_LOAD3(data_), MATHFU_VECTOR3_LOAD3(v.data_)));
  }

  inline Vector<float, 3> operator*(const float& s) const {
    return Vector<float, 3>(
        simd4f_mul(MATHFU_VECTOR3_LOAD3(data_), simd4f_splat(s)));
  }

  inline Vector<float, 3> operator/(const float& s) const {
    return Vector<float, 3>(
        simd4f_div(MATHFU_VECTOR3_LOAD3(data_), simd4f_splat(s)));
  }

  inline Vector<float, 3> operator+(const float& s) const {
    return Vector<float, 3>(
        simd4f_add(MATHFU_VECTOR3_LOAD3(data_), simd4f_splat(s)));
  }

  inline Vector<float, 3> operator-(const float& s) const {
    return Vector<float, 3>(
        simd4f_sub(MATHFU_VECTOR3_LOAD3(data_), simd4f_splat(s)));
  }

  inline Vector<float, 3>& operator*=(const Vector<float, 3>& v) {
    *this =
        simd4f_mul(MATHFU_VECTOR3_LOAD3(data_), MATHFU_VECTOR3_LOAD3(v.data_));
    return *this;
  }

  inline Vector<float, 3>& operator/=(const Vector<float, 3>& v) {
    *this =
        simd4f_div(MATHFU_VECTOR3_LOAD3(data_), MATHFU_VECTOR3_LOAD3(v.data_));
    return *this;
  }

  inline Vector<float, 3>& operator+=(const Vector<float, 3>& v) {
    *this =
        simd4f_add(MATHFU_VECTOR3_LOAD3(data_), MATHFU_VECTOR3_LOAD3(v.data_));
    return *this;
  }

  inline Vector<float, 3>& operator-=(const Vector<float, 3>& v) {
    *this =
        simd4f_sub(MATHFU_VECTOR3_LOAD3(data_), MATHFU_VECTOR3_LOAD3(v.data_));
    return *this;
  }

  inline Vector<float, 3>& operator*=(const float& s) {
    *this = simd4f_mul(MATHFU_VECTOR3_LOAD3(data_), simd4f_splat(s));
    return *this;
  }

  inline Vector<float, 3>& operator/=(const float& s) {
    *this = simd4f_div(MATHFU_VECTOR3_LOAD3(data_), simd4f_splat(s));
    return *this;
  }

  inline Vector<float, 3>& operator+=(const float& s) {
    *this = simd4f_add(MATHFU_VECTOR3_LOAD3(data_), simd4f_splat(s));
    return *this;
  }

  inline Vector<float, 3>& operator-=(const float& s) {
    *this = simd4f_sub(MATHFU_VECTOR3_LOAD3(data_), simd4f_splat(s));
    return *this;
  }

  inline bool operator==(const Vector<float, 3>& v) const {
    for (int i = 0; i < 3; ++i) {
      if ((*this)[i] != v[i]) return false;
    }
    return true;
  }

  inline bool operator!=(const Vector<float, 3>& v) const {
    return !operator==(v);
  }

  inline float LengthSquared() const {
    return simd4f_dot3_scalar(MATHFU_VECTOR3_LOAD3(data_),
                              MATHFU_VECTOR3_LOAD3(data_));
  }

  inline float Length() const {
    return simd4f_get_x(simd4f_length3(MATHFU_VECTOR3_LOAD3(data_)));
  }

  inline float Normalize() {
    const float length = Length();
    *this = simd4f_mul(MATHFU_VECTOR3_LOAD3(data_), simd4f_splat(1 / length));
    return length;
  }

  inline Vector<float, 3> Normalized() const {
    return Vector<float, 3>(simd4f_normalize3(MATHFU_VECTOR3_LOAD3(data_)));
  }

  static inline float DotProduct(const Vector<float, 3>& v1,
                                 const Vector<float, 3>& v2) {
    return simd4f_dot3_scalar(MATHFU_VECTOR3_LOAD3(v1.data_),
                              MATHFU_VECTOR3_LOAD3(v2.data_));
  }

  static inline Vector<float, 3> CrossProduct(const Vector<float, 3>& v1,
                                              const Vector<float, 3>& v2) {
    return Vector<float, 3>(simd4f_cross3(MATHFU_VECTOR3_LOAD3(v1.data_),
                                          MATHFU_VECTOR3_LOAD3(v2.data_)));
  }

  static inline Vector<float, 3> HadamardProduct(const Vector<float, 3>& v1,
                                                 const Vector<float, 3>& v2) {
    return Vector<float, 3>(simd4f_mul(MATHFU_VECTOR3_LOAD3(v1.data_),
                                       MATHFU_VECTOR3_LOAD3(v2.data_)));
  }

  static inline Vector<float, 3> Lerp(const Vector<float, 3>& v1,
                                      const Vector<float, 3>& v2,
                                      float percent) {
    const Vector<float, 3> percentv(percent);
    const Vector<float, 3> one(1.0f);
    const Vector<float, 3> one_minus_percent = one - percentv;
    return Vector<float, 3>(
        simd4f_add(simd4f_mul(MATHFU_VECTOR3_LOAD3(one_minus_percent.data_),
                              MATHFU_VECTOR3_LOAD3(v1.data_)),
                   simd4f_mul(MATHFU_VECTOR3_LOAD3(percentv.data_),
                              MATHFU_VECTOR3_LOAD3(v2.data_))));
  }

  /// Generates a random vector, where the range for each component is
  /// bounded by min and max.
  static inline Vector<float, 3> RandomInRange(const Vector<float, 3>& min,
                                               const Vector<float, 3>& max) {
    return Vector<float, 3>(mathfu::RandomInRange<float>(min[0], max[0]),
                            mathfu::RandomInRange<float>(min[1], max[1]),
                            mathfu::RandomInRange<float>(min[2], max[2]));
  }

  static inline Vector<float, 3> Max(const Vector<float, 3>& v1,
                                     const Vector<float, 3>& v2) {
#ifdef MATHFU_COMPILE_WITH_PADDING
    return Vector<float, 3>(simd4f_max(MATHFU_VECTOR3_LOAD3(v1.data_),
                                       MATHFU_VECTOR3_LOAD3(v2.data_)));
#else
    return Vector<float, 3>(std::max(v1[0], v2[0]), std::max(v1[1], v2[1]),
                            std::max(v1[2], v2[2]));
#endif  // MATHFU_COMPILE_WITH_PADDING
  }

  static inline Vector<float, 3> Min(const Vector<float, 3>& v1,
                                     const Vector<float, 3>& v2) {
#ifdef MATHFU_COMPILE_WITH_PADDING
    return Vector<float, 3>(simd4f_min(MATHFU_VECTOR3_LOAD3(v1.data_),
                                       MATHFU_VECTOR3_LOAD3(v2.data_)));
#else
    return Vector<float, 3>(std::min(v1[0], v2[0]), std::min(v1[1], v2[1]),
                            std::min(v1[2], v2[2]));
#endif  // MATHFU_COMPILE_WITH_PADDING
  }

  template <class T, int rows, int cols>
  friend class Matrix;
  template <class T, int d>
  friend class Vector;

  MATHFU_DEFINE_CLASS_SIMD_AWARE_NEW_DELETE

 private:
  union {
#ifdef MATHFU_COMPILE_WITH_PADDING
    simd4f simd;
    float float_array[4];
#else
    float float_array[3];
#endif  // MATHFU_COMPILE_WITH_PADDING
  } data_;
};
/// @endcond
#endif  // MATHFU_COMPILE_WITH_SIMD

}  // namespace mathfu

#endif  // MATHFU_VECTOR_3_H_
