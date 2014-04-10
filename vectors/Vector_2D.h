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
#ifndef GOOMATH_VECTORS_VECTOR_2D_H_
#define GOOMATH_VECTORS_VECTOR_2D_H_

#include <math.h>
#include <utilities/Utilities.h>

#if !defined(COMPILE_WITHOUT_SIMD_SUPPORT) && defined(__ARM_NEON__)
#include <vectorial/simd2f.h>
#endif

#include <vectors/Vector.h>

namespace goomath {

#if !defined(COMPILE_WITHOUT_SIMD_SUPPORT) && defined(__ARM_NEON__)
template<>
class Vector<float, 2> {
 public:
  typedef float Scalar;

  inline Vector() {}

  inline Vector(const Vector<float, 2>& v) {
    data_ = v.data_;
  }

  inline Vector(const simd2f& v) {
    data_ = v;
  }

  explicit inline Vector(const float& s) {
    data_ = simd2f_create(s, s);
  }

  inline Vector(const float& s1, const float& s2) {
    data_ = simd2f_create(s1, s2);
  }

  explicit inline Vector(const float* v) {
    data_ = simd2f_create(v[0], v[1]);
  }

  inline float& operator()(const int i) {
    return *(union_reinterpret_cast<float*>(&data_) + i - 1);
  }

  inline const float operator()(const int i) const {
    return i == 1 ? simd2f_get_x(data_):simd2f_get_y(data_);
  }

  inline float& operator[](const int i) {
    return *(union_reinterpret_cast<float*>(&data_) + i);
  }

  inline const float operator[](const int i) const {
    return i == 0 ? simd2f_get_x(data_):simd2f_get_y(data_);
  }

  inline Vector<float, 2> operator-() const {
    return Vector<float, 2>(simd2f_sub(simd2f_zero(), data_));
  }

  inline Vector<float, 2> operator*(const Vector<float, 2>& v) const {
    return Vector<float, 2>(simd2f_mul(data_, v.data_));
  }

  inline Vector<float, 2> operator/(const Vector<float, 2>& v) const {
    return Vector<float, 2>(simd2f_div(data_, v.data_));
  }

  inline Vector<float, 2> operator+(const Vector<float, 2>& v) const {
    return Vector<float, 2>(simd2f_add(data_, v.data_));
  }

  inline Vector<float, 2> operator-(const Vector<float, 2>& v) const {
    return Vector<float, 2>(simd2f_sub(data_, v.data_));
  }

  inline Vector<float, 2> operator*(const float& s) const {
    return Vector<float, 2>(simd2f_mul(data_, simd2f_splat(s)));
  }

  inline Vector<float, 2> operator/(const float& s) const {
    return Vector<float, 2>(simd2f_mul(data_, simd2f_splat(1 / s)));
  }

  inline Vector<float, 2> operator+(const float& s) const {
    return Vector<float, 2>(simd2f_add(data_, simd2f_splat(s)));
  }

  inline Vector<float, 2> operator-(const float& s) const {
    return Vector<float, 2>(simd2f_sub(data_, simd2f_splat(s)));
  }

  inline Vector<float, 2>& operator*=(const Vector<float, 2>& v) {
    data_ = simd2f_mul(data_, v.data_); return *this;
  }

  inline Vector<float, 2>& operator/=(const Vector<float, 2>& v) {
    data_ = simd2f_div(data_, v.data_); return *this;
  }

  inline Vector<float, 2>& operator+=(const Vector<float, 2>& v) {
    data_ = simd2f_add(data_, v.data_); return *this;
  }

  inline Vector<float, 2>& operator-=(const Vector<float, 2>& v) {
    data_ = simd2f_sub(data_, v.data_); return *this;
  }

  inline Vector<float, 2>& operator*=(const float& s) {
    data_ = simd2f_mul(data_, simd2f_splat(s)); return *this;
  }

  inline Vector<float, 2>& operator/=(const float& s) {
    data_ = simd2f_mul(data_, simd2f_splat(1 / s)); return *this;
  }

  inline Vector<float, 2>& operator+=(const float& s) {
    data_ = simd2f_add(data_, simd2f_splat(s)); return *this;
  }

  inline Vector<float, 2>& operator-=(const float& s) {
    data_ = simd2f_sub(data_, simd2f_splat(s)); return *this;
  }

  inline float LengthSquared() const {
    return simd2f_get_x(simd2f_dot2(data_, data_));
  }

  inline float Length() const {
    return simd2f_get_x(simd2f_length2(data_));
  }

  inline float Normalize() {
    const float length = Length();
    data_ = simd2f_mul(data_, simd2f_splat(1 / length));
    return length;
  }

  inline Vector<float, 2> Normalized() const {
    return simd2f_normalize2(data_);
  }

  static inline float DotProduct(
    const Vector<float, 2>& v1, const Vector<float, 2>& v2) {
    return simd2f_get_x(simd2f_dot2(v1.data_, v2.data_));
  }

  static inline Vector<float, 2> HadamardProduct(
    const Vector<float, 2>& v1, const Vector<float, 2>& v2) {
    return Vector<float, 2>(simd2f_mul(v1.data_, v2.data_));
  }

 private:
  simd2f data_;
};

#endif  //  !defined(COMPILE_WITHOUT_SIMD_SUPPORT) && defined(__ARM_NEON__)
}  // namespace goomath
#endif  // GOOMATH_VECTORS_VECTOR_2D_H_
