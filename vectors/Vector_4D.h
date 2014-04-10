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
#ifndef GOOMATH_VECTORS_VECTOR_4D_H_
#define GOOMATH_VECTORS_VECTOR_4D_H_

#include <math.h>
#include <utilities/Utilities.h>

#ifdef COMPILE_WITH_SIMD
#include <vectorial/simd4f.h>
#endif

#include <vectors/Vector.h>

namespace goomath {

#ifdef COMPILE_WITH_SIMD
template<>
class Vector<float, 4> {
 public:
  typedef float Scalar;

  inline Vector() {}

  inline Vector(const Vector<float, 4>& v) {
    data_ = v.data_;
  }

  inline Vector(const simd4f& v) {
    data_ = v;
  }

  explicit inline Vector(const float& s) {
    data_ = simd4f_splat(s);
  }

  inline Vector(
    const float& s1, const float& s2, const float& s3, const float& s4) {
    data_ = simd4f_create(s1, s2, s3, s4);
  }

  explicit inline Vector(const float* v) {
    data_ = simd4f_create(v[0], v[1], v[2], v[3]);
  }

  inline float& operator()(const int i) {
    return *(union_reinterpret_cast<float*>(&data_) + i - 1);
  }

  inline const float operator()(const int i) const {
    return i == 1 ? simd4f_get_x(data_) : i == 2 ? simd4f_get_y(data_):
      i == 3 ? simd4f_get_z(data_) : simd4f_get_w(data_);
  }

  inline float& operator[](const int i) {
    return *(union_reinterpret_cast<float*>(&data_) + i);
  }

  inline const float operator[](const int i) const {
    return i == 0 ? simd4f_get_x(data_) : i == 1 ? simd4f_get_y(data_):
      i == 2 ? simd4f_get_z(data_) : simd4f_get_w(data_);
  }

  inline Vector<float, 4> operator-() const {
    return Vector<float, 4>(simd4f_sub(simd4f_zero(), data_));
  }

  inline Vector<float, 4> operator*(const Vector<float, 4>& v) const {
    return Vector<float, 4>(simd4f_mul(data_, v.data_));
  }

  inline Vector<float, 4> operator+(const Vector<float, 4>& v) const {
    return Vector<float, 4>(simd4f_add(data_, v.data_));
  }

  inline Vector<float, 4> operator-(const Vector<float, 4>& v) const {
    return Vector<float, 4>(simd4f_sub(data_, v.data_));
  }

  inline Vector<float, 4> operator*(const float& s) const {
    return Vector<float, 4>(simd4f_mul(data_, simd4f_splat(s)));
  }

  inline Vector<float, 4> operator/(const float& s) const {
    return Vector<float, 4>(simd4f_mul(data_, simd4f_splat(1 / s)));
  }

  inline Vector<float, 4>& operator+=(const Vector<float, 4>& v) {
    data_ = simd4f_add(data_, v.data_); return *this;
  }

  inline Vector<float, 4>& operator-=(const Vector<float, 4>& v) {
    data_ = simd4f_sub(data_, v.data_); return *this;
  }

  inline Vector<float, 4>& operator*=(const float& s) {
    data_ = simd4f_mul(data_, simd4f_splat(s)); return *this;
  }

  inline Vector<float, 4>& operator/=(const float& s) {
    data_ = simd4f_mul(data_, simd4f_splat(1 / s)); return *this;
  }

  inline float LengthSquared() const {
    return simd4f_get_x(simd4f_dot4(data_, data_));
  }

  inline float Length() const {
    return simd4f_get_x(simd4f_length4(data_));
  }

  inline float Normalize() {
    const float length = Length();
    *this = simd4f_mul(data_, simd4f_splat(1 / length));
    return length;
  }

  inline Vector<float, 4> Normalized() const {
    return simd4f_normalize4(data_);
  }

  static inline float DotProduct(
    const Vector<float, 4>& v1, const Vector<float, 4>& v2) {
    return simd4f_get_x(simd4f_dot4(v1.data_, v2.data_));
  }

  static inline Vector<float, 4> HadamardProduct(
    const Vector<float, 4>& v1, const Vector<float, 4>& v2) {
    return Vector<float, 4>(simd4f_mul(v1.data_, v2.data_));
  }

  template<class T, int rows, int cols> friend class Matrix;

 private:
  simd4f data_;
};

#endif  // COMPILE_WITH_SIMD
}  // namespace goomath
#endif  // GOOMATH_VECTORS_VECTOR_2D_H_
