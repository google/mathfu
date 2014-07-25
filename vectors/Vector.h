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
#ifndef MATHFU_VECTORS_VECTOR_H_
#define MATHFU_VECTORS_VECTOR_H_

#include <math.h>
#include <utilities/Utilities.h>

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable: 4127)  // conditional expression is constant
#endif

#define MATHFU_VEC_OPERATION(OP) \
  const int i = 0; OP; \
  if (d > 1) { \
    const int i = 1; OP; \
    if (d > 2) { \
      const int i = 2; OP; \
      if (d > 3) { \
        const int i = 3; OP; \
        if (d > 4) { \
          for (int i = 4; i < d; ++i) OP; \
        } \
      } \
    } \
  }

#define MATHFU_VEC_OPERATOR(OP) \
  { Vector<T, d> result; \
  MATHFU_VEC_OPERATION(result[i] = OP); \
  return result; }

#define MATHFU_VEC_SELF_OPERATOR(OP) \
  { MATHFU_VEC_OPERATION(OP); \
  return *this; }

namespace mathfu {

/// @class Vector
/// Stores a vector of d elements with type T and provides a set of utility
/// operations on each vector.
template<class T, int d>
class Vector {
 public:
  /// Typedef the vector type as Scalar to allow for access of the type by other
  /// classes.
  typedef T Scalar;

  /// Create a vector of unitialized values.
  inline Vector() {}

  /// Create a vector from another vector copying each element.
  /// @param v Vector that the data will be copied from.
  inline Vector(const Vector<T, d>& v) {
    MATHFU_VEC_OPERATION(data_[i] = v.data_[i]);
  }

  /// Create a vector from a single float. Each elements is set to be equal to
  /// the value given.
  /// @param s Scalar value that the vector will be initialized to.
  explicit inline Vector(const T& s) {
    MATHFU_VEC_OPERATION(data_[i] = s);
  }

  /// Create a vector form the first d elements of an array.
  /// @param a Array of values that the vector will be iniitlized to.
  explicit inline Vector(const T* a) {
    MATHFU_VEC_OPERATION(data_[i] = a[i]);
  }

  /// Create a vector from two values. This method only works when the vector is
  /// of size two.
  /// @param s1 Scalar value for the first element of the vector.
  /// @param s2 Scalar value for the second element of the vector.
  inline Vector(const T& s1, const T& s2) {
    STATIC_ASSERT(d == 2);
    data_[0] = s1;
    data_[1] = s2;
  }

  /// Create a vector from three values. This method only works when the vector
  /// is of size three.
  /// @param s1 Scalar value for the first element of the vector.
  /// @param s2 Scalar value for the second element of the vector.
  /// @param s3 Scalar value for the third element of the vector.
  inline Vector(const T& s1, const T& s2, const T& s3) {
    STATIC_ASSERT(d == 3);
    data_[0] = s1;
    data_[1] = s2;
    data_[2] = s3;
  }

  /// Create a vector from four values. This method only works when the vector
  /// is of size four.
  /// @param s1 Scalar value for the first element of the vector.
  /// @param s2 Scalar value for the second element of the vector.
  /// @param s3 Scalar value for the third element of the vector.
  /// @param s4 Scalar value for the forth element of the vector.
  inline Vector(const T& s1, const T& s2, const T& s3, const T& s4) {
    STATIC_ASSERT(d == 4);
    data_[0] = s1;
    data_[1] = s2;
    data_[2] = s3;
    data_[3] = s4;
  }

  inline T& operator()(const int i) {
    return data_[i - 1];
  }

  inline const T& operator()(const int i) const {
    return data_[i - 1];
  }

  /// Access an element of the vector.
  /// @param i The index to access.
  /// @return A reference to the accessed data that can be modified by the
  /// caller.
  inline T& operator[](const int i) {
    return data_[i];
  }

  /// Access an element of the vector.
  /// @param i The index to access.
  /// @return A const reference to the accessed data that cannot be modified
  /// by the caller.
  inline const T& operator[](const int i) const {
    return data_[i];
  }

  /// Vector negation.
  /// @return A new vector that stores the negation result.
  inline Vector<T, d> operator-() const {
    MATHFU_VEC_OPERATOR(-data_[i]);
  }

  /// Vector mulitplication. Note that in line with GLSL this does componentwise
  /// multiplication.
  /// @param v A vector to mulitply this vector with.
  /// @return A new vector that stores the result.
  inline Vector<T, d> operator*(const Vector<T, d>& v) const {
    return HadamardProduct(*this, v);
  }

  /// Vector division. Note that in line with GLSL this does componentwise
  /// division.
  /// @param v A vector to divide this vector by.
  /// @return A new vector that stores the result.
  inline Vector<T, d> operator/(const Vector<T, d>& v) const {
    MATHFU_VEC_OPERATOR(data_[i] / v[i]);
  }

  /// Vector addition.
  /// @param v A vector to add this vector with.
  /// @return A new vector that stores the result.
  inline Vector<T, d> operator+(const Vector<T, d>& v) const {
    MATHFU_VEC_OPERATOR(data_[i] + v[i]);
  }

  /// Vector subtraction.
  /// @param v A vector to subtract from this vector.
  /// @return A new vector that stores the result.
  inline Vector<T, d> operator-(const Vector<T, d>& v) const {
    MATHFU_VEC_OPERATOR(data_[i] - v[i]);
  }

  /// Vector/Scalar multiplication.
  /// @param s A scalar to multiply this vector with.
  /// @return A new vector that stores the result.
  inline Vector<T, d> operator*(const T& s) const {
    MATHFU_VEC_OPERATOR(data_[i] * s);
  }

  /// Vector/Scalar division. Note that this is defined as multiplication by the
  /// inverse of the scalar.
  /// @param s A scalar to divide this vector with.
  /// @return A new vector that stores the result.
  inline Vector<T, d> operator/(const T& s) const {
    return (*this) * (1 / s);
  }

  /// Vector/Scalar addition. Note that this is defined as addition between
  /// the origional vector and the scalar multiplied by a vector of ones.
  /// @param s A scalar to add to this vector.
  /// @return A new vector that stores the result.
  inline Vector<T, d> operator+(const T& s) const {
    MATHFU_VEC_OPERATOR(data_[i] + s);
  }

  /// Vector/Scalar subtraction. Note that this is defined as subtraction
  /// between the origional vector and the scalar multiplied by a vector of
  /// ones.
  /// @param s A scalar to subtract from this vector.
  /// @return A new vector that stores the result.
  inline Vector<T, d> operator-(const T& s) const {
    MATHFU_VEC_OPERATOR(data_[i] - s);
  }

  /// In place vector multiplication. Note that in line with GLSL this does
  /// componentwise multiplication.
  /// @param v A vector to multiply this vector with.
  /// @return A reference to this class.
  inline Vector<T, d>& operator*=(const Vector<T, d>& v) {
    MATHFU_VEC_SELF_OPERATOR(data_[i] *= v[i]);
  }

  /// In place vector division. Note that in line with GLSL this does
  /// componentwise division.
  /// @param v A vector to divide this vector by.
  /// @return A reference to this class.
  inline Vector<T, d>& operator/=(const Vector<T, d>& v) {
    MATHFU_VEC_SELF_OPERATOR(data_[i] *= v[i]);
  }

  /// In place vector addition.
  /// @param v A vector to add this vector with.
  /// @return A reference to this class.
  inline Vector<T, d>& operator+=(const Vector<T, d>& v) {
    MATHFU_VEC_SELF_OPERATOR(data_[i] += v[i]);
  }

  /// In place vector subtraction.
  /// @param v A vector to subtract this vector by.
  /// @return A reference to this class.
  inline Vector<T, d>& operator-=(const Vector<T, d>& v) {
    MATHFU_VEC_SELF_OPERATOR(data_[i] -= v[i]);
  }

  /// In place vector/scalar multiplication.
  /// @param s A scalar to mulitply this vector with.
  /// @return A reference to this class.
  inline Vector<T, d>& operator*=(const T& s) {
    MATHFU_VEC_SELF_OPERATOR(data_[i] *= s);
  }

  /// In place vector/scalar division. Note that this is defined as
  /// multiplication by the inverse of the scalar.
  /// @param s A scalar to divide this vector by.
  /// @return A reference to this class.
  inline Vector<T, d>& operator/=(const T& s) {
    return (*this) *= (1 / s);
  }

  /// In place vector/scalar addition. Note that this is defined as addition
  /// between the origional vector and the scalar multiplied by a vector of
  /// ones.
  /// @param s A scalar to add this vector to.
  /// @return A reference to this class.
  inline Vector<T, d>& operator+=(const T& s) {
    MATHFU_VEC_SELF_OPERATOR(data_[i] += s);
  }

  /// In place vector/scalar subtraction. Note that this is defined as
  /// subtraction between the origional vector and the scalar multiplied by a
  /// vector of ones.
  /// @param s A scalar to subtract from this vector.
  /// @return A reference to this class.
  inline Vector<T, d>& operator-=(const T& s) {
    MATHFU_VEC_SELF_OPERATOR(data_[i] -= s);
  }

  /// Find length squared.
  /// @return The length of this vector squared.
  inline T LengthSquared() const {
    return DotProduct(*this,*this);
  }

  /// Find length.
  /// @return The length of this vector.
  inline T Length() const {
    return sqrt(LengthSquared());
  }

  /// Normalize this vector.
  /// @return The length of this vector.
  inline T Normalize() {
    const T length = Length();
    *this = *this*(1 / length);
    return length;
  }

  /// Find a Normalized version of this vector.
  /// @return A copy of this vector normalized.
  inline Vector<T, d> Normalized() {
    return *this*(1 / Length());
  }

  /// Caclculate the dot product of two vectors.
  /// @param v1 First vector.
  /// @param v2 Second vector.
  /// @return The dot product of v1 and v2.
  static inline T DotProduct(
    const Vector<T, d>& v1, const Vector<T, d>& v2) {
    if(d == 2) return v1[0] * v2[0] + v1[1] * v2[1];
    if(d == 3) return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
    if(d == 4)
      return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3];
    if(d > 4) {
      T result = 0; MATHFU_VEC_OPERATION(result += v1[i] * v2[i]); return result;
    }
  }

  /// Calculate the hadamard or componentwise product of two vectors.
  /// @param v1 First vector.
  /// @param v2 Second vector.
  /// @return The hadamard product of v1 and v2.
  static inline Vector<T, d> HadamardProduct(
      const Vector<T, d>& v1, const Vector<T, d>& v2) {
    MATHFU_VEC_OPERATOR(v1[i] * v2[i]);
  }

  /// Calculate the cross product of two vectors. Note that this function is
  /// only defined for Vectors with length 3.
  /// @param v1 First vector.
  /// @param v2 Second vector.
  /// @return The cross product of v1 and v2.
  static inline Vector<T, 3> CrossProduct(
    const Vector<T, 3>& v1,const Vector<T, 3>& v2) {
    return Vector<T, 3>(
      v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2],
      v1[0] * v2[1] - v1[1] * v2[0]);
  }

 private:
  T data_[d];
};

template<class T, int d>
inline Vector<T, d> operator*(const T& s, const Vector<T, d>& v) {
  return v * s;
}

template<class T, int d>
inline Vector<T, d> operator/(const T& s, const Vector<T, d>& v) {
  return v / s;
}

template<class T, int d>
inline Vector<T, d> operator+(const T& s, const Vector<T, d>& v) {
  return v + s;
}

template<class T, int d>
inline Vector<T, d> operator-(const T& s, const Vector<T, d>& v) {
  return v - s;
}

}  // namespace mathfu

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // MATHFU_VECTORS_VECTOR_H_
