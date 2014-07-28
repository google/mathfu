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
#ifndef MATHFU_VECTORS_VECTOR_3D_H_
#define MATHFU_VECTORS_VECTOR_3D_H_

#include <math.h>
#include <utilities/Utilities.h>

#ifdef COMPILE_WITH_SIMD
#include <vectorial/simd4f.h>
#endif

#include <vectors/Vector.h>

// Add macros to account for both the case where the vector is stored as a simd
// intrinsic using 4 elements or as 3 values of type T.
// MATHFU_STORE/MATHFU_LOAD are additional operations used to load/store the
// non simd values from and to simd datatypes. If intrinsics are used these
// amount to essentially noops. MATHFU_INIT either creates a simd datatype if
// the intrinsic is used or sets the T values if not.
#ifdef COMPILE_WITH_PADDING
#define MATHFU_STORE(v, data) {data = v;}
#define MATHFU_LOAD(data) data
#define MATHFU_INIT(data, v1, v2, v3) { \
  data = simd4f_create(v1, v2, v3, 0); }
#else
#define MATHFU_STORE(v, data) {simd4f_ustore3(v, data);}
#define MATHFU_LOAD(data) simd4f_uload4(data)
#define MATHFU_INIT(data, v1, v2, v3) { \
  data[0] = v1; data[1] = v2; data[2] = v3; }
#endif

namespace mathfu {

#ifdef COMPILE_WITH_SIMD
template<>
class Vector<float, 3> {
 public:
  typedef float Scalar;

  inline Vector() {}

  inline Vector(const Vector<float, 3>& v) {
#ifdef COMPILE_WITH_PADDING
    data_ = v.data_;
#else
    data_[0] = v.data_[0]; data_[1] = v.data_[1]; data_[2] = v.data_[2];
#endif
  }

  inline Vector(const simd4f& v) {
    MATHFU_STORE(v, data_);
  }

  explicit inline Vector(const float& s) {
    MATHFU_INIT(data_, s, s, s);
  }

  inline Vector(const float& v1, const float& v2, const float& v3) {
    MATHFU_INIT(data_, v1, v2, v3);
  }

  explicit inline Vector(const float* v) {
    MATHFU_INIT(data_, v[0], v[1], v[2]);
  }

  inline float& operator()(const int i) {
    return *(CAST<float*>(&data_) + i - 1);
  }

  inline const float& operator()(const int i) const {
    return *(CAST<const float*>(&data_) + i - 1);
  }

  inline float& operator[](const int i) {
    return *(CAST<float*>(&data_) + i);
  }

  inline const float& operator[](const int i) const {
    return *(CAST<const float*>(&data_) + i);
  }

  inline T& x() { return (*this)[0]; }
  inline T& y() { return (*this)[1]; }
  inline T& z() { return (*this)[2]; }

  inline const T& x() const { return (*this)[0]; }
  inline const T& y() const { return (*this)[1]; }
  inline const T& z() const { return (*this)[2]; }

  inline Vector<float, 3> operator-() const {
    return Vector<float, 3>(simd4f_sub(simd4f_zero(), MATHFU_LOAD(data_)));
  }

  inline Vector<float, 3> operator*(const Vector<float, 3>& v) const {
    return Vector<float, 3>(simd4f_mul(MATHFU_LOAD(data_), MATHFU_LOAD(v.data_)));
  }

  inline Vector<float, 3> operator/(const Vector<float, 3>& v) const {
    return Vector<float, 3>(simd4f_div(MATHFU_LOAD(data_), MATHFU_LOAD(v.data_)));
  }

  inline Vector<float, 3> operator+(const Vector<float, 3>& v) const {
    return Vector<float, 3>(simd4f_add(MATHFU_LOAD(data_), MATHFU_LOAD(v.data_)));
  }

  inline Vector<float, 3> operator-(const Vector<float, 3>& v) const {
    return Vector<float, 3>(simd4f_sub(MATHFU_LOAD(data_), MATHFU_LOAD(v.data_)));
  }

  inline Vector<float, 3> operator*(const float& s) const {
    return Vector<float, 3>(simd4f_mul(MATHFU_LOAD(data_), simd4f_splat(s)));
  }

  inline Vector<float, 3> operator/(const float& s) const {
    return Vector<float, 3>(simd4f_mul(MATHFU_LOAD(data_), simd4f_splat(1 / s)));
  }

  inline Vector<float, 3> operator+(const float& s) const {
    return Vector<float, 3>(simd4f_add(MATHFU_LOAD(data_), simd4f_splat(s)));
  }

  inline Vector<float, 3> operator-(const float& s) const {
    return Vector<float, 3>(simd4f_sub(MATHFU_LOAD(data_), simd4f_splat(1 / s)));
  }

  inline Vector<float, 3>& operator*=(const Vector<float, 3>& v) {
    *this = simd4f_mul(MATHFU_LOAD(data_), MATHFU_LOAD(v.data_)); return *this;
  }

  inline Vector<float, 3>& operator/=(const Vector<float, 3>& v) {
    *this = simd4f_div(MATHFU_LOAD(data_), MATHFU_LOAD(v.data_)); return *this;
  }

  inline Vector<float, 3>& operator+=(const Vector<float, 3>& v) {
    *this = simd4f_add(MATHFU_LOAD(data_), MATHFU_LOAD(v.data_)); return *this;
  }

  inline Vector<float, 3>& operator-=(const Vector<float, 3>& v) {
    *this = simd4f_sub(MATHFU_LOAD(data_), MATHFU_LOAD(v.data_)); return *this;
  }

  inline Vector<float, 3>& operator*=(const float& s) {
    *this = simd4f_mul(MATHFU_LOAD(data_), simd4f_splat(s)); return *this;
  }

  inline Vector<float, 3>& operator/=(const float& s) {
    *this = simd4f_mul(MATHFU_LOAD(data_), simd4f_splat(1 / s)); return *this;
  }

  inline Vector<float, 3>& operator+=(const float& s) {
    *this = simd4f_add(MATHFU_LOAD(data_), simd4f_splat(s)); return *this;
  }

  inline Vector<float, 3>& operator-=(const float& s) {
    *this = simd4f_sub(MATHFU_LOAD(data_), simd4f_splat(s)); return *this;
  }

  inline float LengthSquared() const {
    return simd4f_dot3(MATHFU_LOAD(data_), MATHFU_LOAD(data_));
  }

  inline float Length() const {
    return simd4f_get_x(simd4f_length3(MATHFU_LOAD(data_)));
  }

  inline float Normalize() {
    const float length = Length();
    *this = simd4f_mul(MATHFU_LOAD(data_), simd4f_splat(1 / length));
    return length;
  }

  inline Vector<float, 3> Normalized() const {
    return simd4f_normalize3(MATHFU_LOAD(data_));
  }

  static inline float DotProduct(
    const Vector<float, 3>& v1, const Vector<float, 3>& v2) {
    return simd4f_dot3(MATHFU_LOAD(v1.data_), MATHFU_LOAD(v2.data_));
  }

  static inline Vector<float, 3> CrossProduct(
    const Vector<float, 3>& v1, const Vector<float, 3>& v2) {
    return Vector<float, 3>(
      simd4f_cross3(MATHFU_LOAD(v1.data_), MATHFU_LOAD(v2.data_)));
  }

  static inline Vector<float, 3> HadamardProduct(
    const Vector<float, 3>& v1, const Vector<float, 3>& v2) {
    return Vector<float, 3>(
      simd4f_mul(MATHFU_LOAD(v1.data_), MATHFU_LOAD(v2.data_)));
  }

  template<class T, int rows, int cols> friend class Matrix;

 private:
#ifdef COMPILE_WITH_PADDING
  simd4f data_;
#else
  float data_[3];
#endif
};
#endif  // COMPILE_WITH_SIMD
}  // namespace mathfu
#endif  // MATHFU_VECTORS_VECTOR_3D
