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
#ifndef MATHFU_VECTOR_H_
#define MATHFU_VECTOR_H_

#include <math.h>
#include <cmath>

#include "mathfu/internal/vector_types.h"
#include "mathfu/internal/vector_functions.h"
#include "mathfu/utilities.h"


/// @file mathfu/vector.h Vector
/// @brief Vector class and functions.
/// @addtogroup mathfu_vector

namespace mathfu {

// Forward-declaration of the Vector type and its underlying data structure.
// See the actual VectorImpl class definition below for more details.
template <class Base>
class VectorImpl;

// When SIMD is enabled, Vector should use the SimdVectorBase data structure
// for implementing its data storage.  Otherwise, it can use the normal
// VectorBase data structure.
#ifdef MATHFU_COMPILE_WITH_SIMD
template <class T, int Dims>
using Vector = VectorImpl<internal::SimdVectorBase<T, Dims>>;
#else
template <class T, int Dims>
using Vector = VectorImpl<internal::VectorBase<T, Dims>>;
#endif

// Forward-declaration of the VectorPacked type and its underlying data
// structure.  See the actual VectorPackedImpl class definition below for more
// details.
template <class Base>
class VectorPackedImpl;

template <class T, int Dims>
using VectorPacked = VectorPackedImpl<internal::VectorBase<T, Dims>>;


/// @addtogroup mathfu_vector
/// @{

/// @class VectorPacked "mathfu/vector.h"
/// @brief Packed N-dimensional vector.
///
/// Some Vector classes are padded so that it's possible to use the data
/// structures with SIMD instructions.  This structure can be used in
/// conjunction with unpacked Vector classes to pack data
/// into flat arrays suitable for sending to a GPU (e.g vertex buffers).
///
/// <p>
/// For example, to pack (store) an unpacked to packed vector:<br>
/// <blockquote><code><pre>
/// VectorPacked<float, 3> packed;
/// Vector<float, 3> vector(3, 2, 1);
/// vector.Pack(&packed);
/// </pre></code></blockquote>
/// or<br>
/// <blockquote><code><pre>
/// Vector<float, 3> vector(3, 2, 1);
/// VectorPacked<float, 3> packed = vector;
/// </pre></code></blockquote>
/// </p>
///
/// <p>
/// To initialize a vector from a packed vector:<br>
/// <blockquote><code><pre>
/// VectorPacked<float, 3> packed = { 3, 2, 1 };
/// Vector<float, 3> vector(packed);
/// </pre></code></blockquote>
///
/// @tparam T type of VectorPacked elements.
/// @tparam d dimensions (number of elements) in the VectorPacked structure.
template <class Base>
class VectorPackedImpl : public Base {
 public:
  using T = typename std::decay<decltype(Base::data_[0])>::type;
  static constexpr int Dims = sizeof(Base::data_) / sizeof(Base::data_[0]);

  /// Create an uninitialized VectorPacked.
  VectorPackedImpl() {}

  /// Create a VectorPacked from a Vector.
  ///
  /// Both VectorPacked and Vector must have the same number of dimensions.
  /// @param vector Vector to create the VectorPacked from.
  explicit VectorPackedImpl(const Vector<T, Dims>& vector) {
    vector.Pack(this);
  }

  /// Copy a Vector to a VectorPacked.
  ///
  /// Both VectorPacked and Vector must have the same number of dimensions.
  /// @param vector Vector to copy to the VectorPacked.
  /// @returns A reference to this VectorPacked.
  VectorPackedImpl& operator=(const Vector<T, Dims>& vector) {
    vector.Pack(this);
    return *this;
  }
};
/// @}

/// @addtogroup mathfu_vector
/// @{
/// @class Vector "mathfu/vector.h"
/// @brief Vector of Dims elements with type T
///
/// Vector stores <b>Dims</b> elements of type <b>T</b> and provides a set
/// functions to perform operations on the set of elements.
///
/// @tparam T type of Vector elements.
/// @tparam Dims dimensions (number of elements) in the Vector structure.
template <class Base>
class VectorImpl : public Base {
 public:
  /// @brief Element type to enable reference by other classes.
  using T = typename internal::ValueType<VectorImpl>::type;
  using Scalar = T;
  static constexpr int Dims = sizeof(Base::data_) / sizeof(Base::data_[0]);

  /// @brief Create an uninitialized Vector.
  inline VectorImpl() {}

  /// @brief Create a vector from another vector copying each element.
  ///
  /// @param v Vector that the data will be copied from.
  inline VectorImpl(const Vector<T, Dims>& v) {
    MATHFU_UNROLLED_LOOP(i, Dims, this->data_[i] = v.data_[i]);
  }

  /// @brief Create a vector from another vector of a different type.
  ///
  /// This copies each element of a Vector which makes it possible to between
  /// vectors of different types, for example
  /// <code>float/double/int</code> vectors.
  /// @param v Vector that the data will be copied from.
  /// @tparam U type of Vector elements to copy.
  template <typename U>
  explicit inline VectorImpl(const Vector<U, Dims>& v) {
    MATHFU_UNROLLED_LOOP(i, Dims, this->data_[i] = static_cast<T>(v[i]));
  }

  /// @brief Create a vector from a single float.
  ///
  /// Each elements is set to be equal to the value given.
  /// @param s Scalar value that the vector will be initialized to.
  explicit inline VectorImpl(T s) {
    MATHFU_UNROLLED_LOOP(i, Dims, this->data_[i] = s);
  }

  /// @brief Create a vector form the first Dims elements of an array.
  ///
  /// @param a Array of values that the vector will be iniitlized to.
  explicit inline VectorImpl(const T* a) {
    MATHFU_UNROLLED_LOOP(i, Dims, this->data_[i] = a[i]);
  }

  /// @brief Create a vector from two values.
  ///
  /// @note This method only works when the vector is of size two.
  ///
  /// @param s1 Scalar value for the first element of the vector.
  /// @param s2 Scalar value for the second element of the vector.
  inline VectorImpl(T s1, T s2) {
    MATHFU_STATIC_ASSERT(Dims == 2);
    this->data_[0] = s1;
    this->data_[1] = s2;
  }

  /// @brief Create a vector from three values.
  ///
  /// @note This method only works when the vector is of size three.
  ///
  /// @param s1 Scalar value for the first element of the vector.
  /// @param s2 Scalar value for the second element of the vector.
  /// @param s3 Scalar value for the third element of the vector.
  inline VectorImpl(T s1, T s2, T s3) {
    MATHFU_STATIC_ASSERT(Dims == 3);
    this->data_[0] = s1;
    this->data_[1] = s2;
    this->data_[2] = s3;
  }

  /// @brief Create a vector from a 2 component vector and a third value.
  ///
  /// @note This method only works when the vector is of size three.
  ///
  /// @param v12 Vector containing the first 2 values.
  /// @param s3 Scalar value for the third element of the vector.
  inline VectorImpl(const Vector<T, 2>& v12, T s3) {
    MATHFU_STATIC_ASSERT(Dims == 3);
    this->data_[0] = v12[0];
    this->data_[1] = v12[1];
    this->data_[2] = s3;
  }

  /// @brief Create a vector from four values.
  ///
  /// @note This method only works when the vector is of size four.
  ///
  /// @param s1 Scalar value for the first element of the vector.
  /// @param s2 Scalar value for the second element of the vector.
  /// @param s3 Scalar value for the third element of the vector.
  /// @param s4 Scalar value for the forth element of the vector.
  inline VectorImpl(T s1, T s2, T s3, T s4) {
    MATHFU_STATIC_ASSERT(Dims == 4);
    this->data_[0] = s1;
    this->data_[1] = s2;
    this->data_[2] = s3;
    this->data_[3] = s4;
  }

  /// @brief Create a 4-dimensional vector from a Vector<T, 3>.
  ///
  /// The last element is initialized to the specified value.
  /// @note This method only works with 4 element vectors.
  ///
  /// @param vector3 Vector used to initialize the first 3 elements.
  /// @param value Value used to set the last element of the vector.
  inline VectorImpl(const Vector<T, 3>& vector123, T s4) {
    MATHFU_STATIC_ASSERT(Dims == 4);
    this->data_[0] = vector123[0];
    this->data_[1] = vector123[1];
    this->data_[2] = vector123[2];
    this->data_[3] = s4;
  }

  /// @brief Create a vector from two 2 component vectors.
  ///
  /// @note This method only works when the vector is of size four.
  ///
  /// @param v12 Vector containing the first 2 values.
  /// @param v34 Vector containing the last 2 values.
  inline VectorImpl(const Vector<T, 2>& v12, const Vector<T, 2>& v34) {
    MATHFU_STATIC_ASSERT(Dims == 4);
    this->data_[0] = v12[0];
    this->data_[1] = v12[1];
    this->data_[2] = v34[0];
    this->data_[3] = v34[1];
  }

  /// @brief Create a vector from packed vector (VectorPacked).
  ///
  /// @param vector Packed vector used to initialize an unpacked.
  explicit inline VectorImpl(const VectorPacked<T, Dims>& vector) {
    MATHFU_UNROLLED_LOOP(i, Dims, this->data_[i] = vector.data_[i]);
  }

  #ifdef MATHFU_SIMD2_ENABLED
  VectorImpl(simd2f simd) { this->simd = simd; }
  #endif

  #ifdef MATHFU_SIMD4_ENABLED
  VectorImpl(simd4f simd) { this->simd = simd; }
  #endif

  /// @brief Access an element of the vector.
  ///
  /// @param i Index of the element to access.
  /// @return A reference to the accessed data that can be modified by the
  /// caller.
  inline T& operator()(const int i) { return this->data_[i]; }

  /// @brief Access an element of the vector.
  ///
  /// @param i Index of the element to access.
  /// @return A reference to the accessed data.
  inline const T& operator()(const int i) const { return this->data_[i]; }

  /// @brief Access an element of the vector.
  ///
  /// @param i Index of the element to access.
  /// @return A reference to the accessed data that can be modified by the
  /// caller.
  inline T& operator[](const int i) { return this->data_[i]; }

  /// @brief Access an element of the vector.
  ///
  /// @param i Index of the element to access.
  /// @return A const reference to the accessed.
  inline const T& operator[](const int i) const { return this->data_[i]; }

  /// @brief GLSL style 3 element accessor.
  ///
  /// This only works with vectors that contain more than 3 elements.
  /// @returns A 3-dimensional Vector containing the first 3 elements of
  // this Vector.
  inline Vector<T, 3> xyz() {
    MATHFU_STATIC_ASSERT(Dims > 3);
    return Vector<T, 3>(this->data_[0], this->data_[1], this->data_[2]);
  }

  /// @brief GLSL style 3 element accessor.
  ///
  /// This only works with vectors that contain more than 3 elements.
  /// @returns A 3-dimensional Vector containing the first 3 elements of
  // this Vector.
  inline const Vector<T, 3> xyz() const {
    MATHFU_STATIC_ASSERT(Dims > 3);
    return Vector<T, 3>(this->data_[0], this->data_[1], this->data_[2]);
  }

  /// @brief GLSL style 2 element accessor.
  ///
  /// This only works with vectors that contain more than 2 elements.
  /// @returns A 2-dimensional Vector with the first 2 elements of this Vector.
  inline Vector<T, 2> xy() {
    MATHFU_STATIC_ASSERT(Dims > 2);
    return Vector<T, 2>(this->data_[0], this->data_[1]);
  }

  /// @brief GLSL style 2 element accessor.
  ///
  /// This only works with vectors that contain more than 2 elements.
  /// @returns A 2-dimensional Vector with the first 2 elements of this Vector.
  inline const Vector<T, 2> xy() const {
    MATHFU_STATIC_ASSERT(Dims > 2);
    return Vector<T, 2>(this->data_[0], this->data_[1]);
  }

  /// @brief GLSL style 2 element accessor.
  ///
  /// This only works with vectors that contain 4 elements.
  /// @returns A 2-dimensional Vector with the last 2 elements of this Vector.
  inline Vector<T, 2> zw() {
    MATHFU_STATIC_ASSERT(Dims == 4);
    return Vector<T, 2>(this->data_[2], this->data_[3]);
  }

  /// @brief GLSL style 2 element accessor.
  ///
  /// This only works with vectors that contain 4 elements.
  /// @returns A 2-dimensional Vector with the last 2 elements of this Vector.
  inline const Vector<T, 2> zw() const {
    MATHFU_STATIC_ASSERT(Dims == 4);
    return Vector<T, 2>(this->data_[2], this->data_[3]);
  }

  /// @brief Pack a Vector to a packed "Dims" element vector structure.
  ///
  /// @param vector Packed "Dims" element vector to write to.
  inline void Pack(VectorPacked<T, Dims>* const vector) const {
    MATHFU_UNROLLED_LOOP(i, Dims, vector->data_[i] = this->data_[i]);
  }

  /// @brief Calculate the squared length of this vector.
  ///
  /// @return The length of this vector squared.
  inline T LengthSquared() const {
    return internal::LengthSquaredHelper(*this);
  }

  /// @brief Calculate the length of this vector.
  ///
  /// @return The length of this vector.
  inline T Length() const { return internal::LengthHelper(*this); }

  /// @brief Normalize this vector in-place.
  ///
  /// @return The length of this vector.
  inline T Normalize() { return internal::NormalizeHelper(*this); }

  /// @brief Calculate the normalized version of this vector.
  ///
  /// @return The normalized vector.
  inline Vector<T, Dims> Normalized() const {
    return internal::NormalizedHelper(*this);
  }

  /// @brief Load from any type that is some formulation of a length Dims array
  /// of
  ///        type T.
  ///
  /// Essentially this is just a type cast and a load, but it happens safely
  /// so that we avoid aliasing bugs.
  ///
  /// @return `compatible` cast to `Vector<T,Dims>` and dereferenced.
  template <typename CompatibleT>
  static inline Vector<T, Dims> FromType(const CompatibleT& compatible) {
    return internal::FromTypeHelper<Vector<T, Dims>, CompatibleT>(compatible);
  }

  /// @brief Load into any type that is some formulation of a length Dims array
  /// of
  ///        type T.
  ///
  /// Essentially this is just a type cast and a load, but it happens safely
  /// so that we avoid aliasing bugs.
  ///
  /// @return `v` cast to `CompatibleT` and dereferenced.
  template <typename CompatibleT>
  static inline CompatibleT ToType(const Vector<T, Dims>& v) {
    return internal::ToTypeHelper<Vector<T, Dims>, CompatibleT>(v);
  }

  /// @brief Calculate the dot product of two vectors.
  ///
  /// @param v1 First vector.
  /// @param v2 Second vector.
  /// @return The dot product of v1 and v2.
  static inline T DotProduct(const Vector<T, Dims>& v1,
                             const Vector<T, Dims>& v2) {
    return internal::DotProductHelper(v1, v2);
  }

  /// @brief Calculate the hadamard or componentwise product of two vectors.
  ///
  /// @param v1 First vector.
  /// @param v2 Second vector.
  /// @return The hadamard product of v1 and v2.
  static inline Vector<T, Dims> HadamardProduct(const Vector<T, Dims>& v1,
                                                const Vector<T, Dims>& v2) {
    return internal::HadamardProductHelper(v1, v2);
  }

  /// @brief Calculate the cross product of two vectors.
  ///
  /// Note that this function is only defined for 3-dimensional Vectors.
  /// @param v1 First vector.
  /// @param v2 Second vector.
  /// @return The cross product of v1 and v2.
  static inline Vector<T, 3> CrossProduct(const Vector<T, 3>& v1,
                                          const Vector<T, 3>& v2) {
    return internal::CrossProductHelper(v1, v2);
  }

  /// @brief Linearly interpolate two vectors.
  ///
  /// @param v1 First vector.
  /// @param v2 Second vector.
  /// @param percent Percentage from v1 to v2 in range 0.0...1.0.
  /// @return The hadamard product of v1 and v2.
  static inline Vector<T, Dims> Lerp(const Vector<T, Dims>& v1,
                                     const Vector<T, Dims>& v2,
                                     const T percent) {
    return internal::LerpHelper(v1, v2, percent);
  }

  /// @brief Generates a random vector.
  ///
  /// The range of each component is bounded by min and max.
  /// @param min Minimum value of the vector.
  /// @param max Maximum value of the vector.
  static inline Vector<T, Dims> RandomInRange(const Vector<T, Dims>& min,
                                              const Vector<T, Dims>& max) {
    return internal::RandomInRangeHelper(min, max);
  }

  /// @brief Compare each component and returns max values.
  ///
  /// @param v1 First vector.
  /// @param v2 Second vector.
  /// @return Max value of v1 and v2.
  static inline Vector<T, Dims> Max(const Vector<T, Dims>& v1,
                                    const Vector<T, Dims>& v2) {
    return internal::MaxHelper(v1, v2);
  }

  /// @brief Compare each component and returns min values.
  ///
  /// @param v1 First vector.
  /// @param v2 Second vector.
  /// @return Min value of v1 and v2.
  static inline Vector<T, Dims> Min(const Vector<T, Dims>& v1,
                                    const Vector<T, Dims>& v2) {
    return internal::MinHelper(v1, v2);
  }

  /// @brief Returns the distance between 2 vectors.
  ///
  /// @param v1 First vector.
  /// @param v2 Second vector.
  /// @return Distance between vectors v1 and v2.
  static inline T Distance(const Vector<T, Dims>& v1,
                           const Vector<T, Dims>& v2) {
    return (v1 - v2).Length();
  }

  /// @brief Returns the squared distance between 2 vectors.
  ///
  /// @param v1 First vector.
  /// @param v2 Second vector.
  /// @return Squared distance between vectors v1 and v2.
  static inline T DistanceSquared(const Vector<T, Dims>& v1,
                                  const Vector<T, Dims>& v2) {
    return (v1 - v2).LengthSquared();
  }

  /// @brief Returns the angle between 2 vectors in radians.
  ///
  /// @param v1 First vector.
  /// @param v2 Second vector.
  /// @return Angle between vectors v1 and v2.
  static inline T Angle(const Vector<T, Dims>& v1, const Vector<T, Dims>& v2) {
    return internal::AngleHelper(v1, v2);
  }

  MATHFU_DEFINE_CLASS_SIMD_AWARE_NEW_DELETE
};
/// @}

/// @addtogroup mathfu_vector
/// @{

/// @brief Compare 2 Vectors of the same size for equality.
///
/// @note: The likelyhood of two float values being the same is very small.
/// Instead consider comparing the difference between two float vectors using
/// LengthSquared() with an epsilon value.
/// For example, v1.LengthSquared(v2) < epsilon.
///
/// @return true if the 2 vectors contains the same value, false otherwise.
template <class T, int Dims>
inline bool operator==(const Vector<T, Dims>& lhs, const Vector<T, Dims>& rhs) {
  return internal::EqualHelper(lhs, rhs);
}

/// @brief Compare 2 Vectors of the same size for inequality.
///
/// @return true if the elements of two vectors differ, false otherwise.
template <class T, int Dims>
inline bool operator!=(const Vector<T, Dims>& lhs, const Vector<T, Dims>& rhs) {
  return internal::NotEqualHelper(lhs, rhs);
}

/// @brief Negate all elements of the Vector.
///
/// @return A new Vector containing the result.
template <class T, int Dims>
inline Vector<T, Dims> operator-(const Vector<T, Dims>& v) {
  return internal::NegateHelper(v);
}

/// @brief Multiply a Vector by a scalar.
///
/// Multiplies each component of the specified Vector with a scalar.
///
/// @param s scalar to multiply.
/// @param v Vector to multiply.
/// @return Vector containing the result.
/// @related Vector
template <class T, int Dims>
inline Vector<T, Dims> operator*(T s, const Vector<T, Dims>& v) {
  return internal::MultiplyHelperSV(s, v);
}

/// @brief Divide a Vector by a scalar.
///
/// Divides each component of the specified Vector by a scalar.
///
/// @param v Vector to be divided.
/// @param s scalar to divide the vector by.
/// @return Vector containing the result.
/// @related Vector
template <class T, int Dims>
inline Vector<T, Dims> operator/(const Vector<T, Dims>& v, T s) {
  return internal::DivideHelperVS(v, s);
}

/// @brief Divide a scalar by a Vector.
///
/// Divides a scalar by each component of the specified Vector.
///
/// @param s scalar numerator.
/// @param v Vector denominator.
/// @return Vector containing the result.
/// @related Vector
template <class T, int Dims>
inline Vector<T, Dims> operator/(const T& s, const Vector<T, Dims>& v) {
  return internal::DivideHelperSV(s, v);
}

/// @brief Add a scalar to each element of a Vector.
///
/// @param s scalar to add to each element of a Vector.
/// @param v Vector to add the scalar to.
/// @return Vector containing the result.
/// @related Vector
template <class T, int Dims>
inline Vector<T, Dims> operator+(T s, const Vector<T, Dims>& v) {
  return internal::AddHelperSV(s, v);
}

/// @brief Subtract a scalar from each element of a Vector.
///
/// @param s scalar to subtract from each element of a Vector.
/// @param v Vector to subtract the scalar from.
/// @return Vector containing the result.
/// @related Vector
template <class T, int Dims>
inline Vector<T, Dims> operator-(T s, const Vector<T, Dims>& v) {
  return internal::SubtractHelperSV(s, v);
}

/// @brief Multiply a vector by another Vector.
///
/// In line with GLSL, this performs component-wise multiplication.
/// @param lhs First vector to use as a starting point.
/// @param rhs Second vector to multiply by.
/// @return A new Vector containing the result.
template <class T, int Dims>
inline Vector<T, Dims> operator*(const Vector<T, Dims>& lhs,
                                 const Vector<T, Dims>& rhs) {
  return internal::MultiplyHelperVV(lhs, rhs);
}

/// @brief Divide a vector by another Vector.
///
/// In line with GLSL, this performs component-wise division.
/// @param lhs First vector to use as a starting point.
/// @param rhs Second vector to divide by.
/// @return A new Vector containing the result.
template <class T, int Dims>
inline Vector<T, Dims> operator/(const Vector<T, Dims>& lhs,
                                 const Vector<T, Dims>& rhs) {
  return internal::DivideHelperVV(lhs, rhs);
}

/// @brief Add a vector with another Vector.
///
/// @param lhs First vector to use as a starting point.
/// @param rhs Second vector to add by.
/// @return A new vector containing the result.
template <class T, int Dims>
inline Vector<T, Dims> operator+(const Vector<T, Dims>& lhs,
                                 const Vector<T, Dims>& rhs) {
  return internal::AddHelperVV(lhs, rhs);
}

/// @brief subtract a vector with another Vector.
///
/// @param lhs First vector to use as a starting point.
/// @param rhs Second vector to subtract by.
/// @return A new vector containing the result.
template <class T, int Dims>
inline Vector<T, Dims> operator-(const Vector<T, Dims>& lhs,
                                 const Vector<T, Dims>& rhs) {
  return internal::SubtractHelperVV(lhs, rhs);
}

/// @brief Multiply a vector with a scalar.
///
/// @param v Vector for the operation.
/// @param s A scalar to multiply the vector with.
/// @return A new vector containing the result.
template <class T, int Dims>
inline Vector<T, Dims> operator*(const Vector<T, Dims>& v, T s) {
  return internal::MultiplyHelperVS(v, s);
}

/// @brief Add a scalar to all elements of a vector.
///
/// @param v Vector for the operation.
/// @param s A scalar to add to the vector.
/// @return A new vector containing the result.
template <class T, int Dims>
inline Vector<T, Dims> operator+(const Vector<T, Dims>& v, T s) {
  return internal::AddHelperVS(v, s);
}

/// @brief Subtract a scalar from all elements of a vector.
///
/// @param v Vector for the operation.
/// @param s A scalar to subtract from a vector.
/// @return A new vector that stores the result.
template <class T, int Dims>
inline Vector<T, Dims> operator-(const Vector<T, Dims>& v, T s) {
  return internal::SubtractHelperVS(v, s);
}

/// @brief Multiply (in-place) a vector with another Vector.
///
/// In line with GLSL, this performs component-wise multiplication.
/// @param lhs First vector to use as a starting point.
/// @param rhs Second vector to multiply by.
/// @return A reference to the input <b>v</b> vector.
template <class T, int Dims>
inline Vector<T, Dims>& operator*=(Vector<T, Dims>& lhs,
                                   const Vector<T, Dims>& rhs) {
  return internal::MultiplyAssignHelperVV(lhs, rhs);
}

/// @brief Divide (in-place) a vector by another Vector.
///
/// In line with GLSL, this performs component-wise division.
/// @param lhs First vector to use as a starting point.
/// @param rhs Second vector to divide by.
/// @return A reference to the input <b>v</b> vector.
template <class T, int Dims>
inline Vector<T, Dims>& operator/=(Vector<T, Dims>& lhs,
                                   const Vector<T, Dims>& rhs) {
  return internal::DivideAssignHelperVV(lhs, rhs);
}

/// @brief Add (in-place) a vector with another Vector.
///
/// @param lhs First vector to use as a starting point.
/// @param rhs Second vector to add.
/// @return A reference to the input <b>v</b> vector.
template <class T, int Dims>
inline Vector<T, Dims>& operator+=(Vector<T, Dims>& lhs,
                                   const Vector<T, Dims>& rhs) {
  return internal::AddAssignHelperVV(lhs, rhs);
}

/// @brief Subtract (in-place) another Vector from a vector.
///
/// @param lhs First vector to use as a starting point.
/// @param rhs Second vector to subtract by.
/// @return A reference to the input <b>v</b> vector.
template <class T, int Dims>
inline Vector<T, Dims>& operator-=(Vector<T, Dims>& lhs,
                                   const Vector<T, Dims>& rhs) {
  return internal::SubtractAssignHelperVV(lhs, rhs);
}

/// @brief Multiply (in-place) each element of a vector with a scalar.
///
/// @param v Vector for the operation.
/// @param s A scalar to multiply the vector with.
/// @return A reference to the input <b>v</b> vector.
template <class T, int Dims>
inline Vector<T, Dims>& operator*=(Vector<T, Dims>& v, T s) {
  return internal::MultiplyAssignHelperVS(v, s);
}

/// @brief Divide (in-place) each element of a vector by a scalar.
///
/// @param v Vector for the operation.
/// @param s A scalar to divide the vector by.
/// @return A reference to the input <b>v</b> vector.
template <class T, int Dims>
inline Vector<T, Dims>& operator/=(Vector<T, Dims>& v, T s) {
  return internal::DivideAssignHelperVS(v, s);
}

/// @brief Add (in-place) a scalar to each element of a vector.
///
/// @param v Vector for the operation.
/// @param s A scalar to add the vector to.
/// @return A reference to the input <b>v</b> vector.
template <class T, int Dims>
inline Vector<T, Dims>& operator+=(Vector<T, Dims>& v, T s) {
  return internal::AddAssignHelperVS(v, s);
}

/// @brief Subtract (in-place) a scalar from each element of a vector.
///
/// @param v Vector for the operation.
/// @param s A scalar to subtract from the vector.
/// @return A reference to the input <b>v</b> vector.
template <class T, int Dims>
inline Vector<T, Dims>& operator-=(Vector<T, Dims>& v, T s) {
  return internal::SubtractAssignHelperVS(v, s);
}

/// @brief Check if val is within [range_start..range_end), denoting a
/// rectangular area.
///
/// @param val 2D vector to be tested.
/// @param range_start Starting point of the range (inclusive).
/// @param range_end Ending point of the range (non-inclusive).
/// @return Bool indicating success.
///
/// @tparam T Type of vector components to test.
template <class T>
bool InRange2D(const Vector<T, 2>& val,
               const Vector<T, 2>& range_start,
               const Vector<T, 2>& range_end) {
  return InRange(val[0], range_start[0], range_end[0]) &&
         InRange(val[1], range_start[1], range_end[1]);
}

/// @addtogroup mathfu_utilities
/// @{

/// @brief Specialized version of RoundUpToPowerOf2 for vector.
template <typename T, int Dims>
inline Vector<T, Dims> RoundUpToPowerOf2(const Vector<T, Dims>& v) {
  return internal::RoundUpToPowerOf2(v);
}

/// @brief Specialized version of Clamp for vector.
template <typename T, int Dims>
inline Vector<T, Dims> Clamp(const Vector<T, Dims>& x,
                             const Vector<T, Dims>& lower,
                             const Vector<T, Dims>& upper) {
  return Vector<T, Dims>::Max(lower, Vector<T, Dims>::Min(x, upper));
}
/// @}

}  // namespace mathfu

#endif  // MATHFU_VECTOR_H_
