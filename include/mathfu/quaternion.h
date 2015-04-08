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
#ifndef MATHFU_QUATERNION_H_
#define MATHFU_QUATERNION_H_

#ifdef _WIN32
#if !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES // For M_PI.
#endif  // !defined(_USE_MATH_DEFINES)
#endif  // _WIN32

#include "mathfu/matrix.h"
#include "mathfu/vector_3.h"

#include <math.h>

/// @file mathfu/quaternion.h
/// @brief Quaternion class and functions.
/// @addtogroup mathfu_quaternion
///
/// MathFu provides a Quaternion class that utilizes SIMD optimized
/// Matrix and Vector classes.

namespace mathfu {

/// @addtogroup mathfu_quaternion
/// @{
/// @class Quaternion
///
/// @brief Stores a Quaternion of type T and provides a set of utility
/// operations on each Quaternion.
/// @tparam T Type of each element in the Quaternion.
template<class T>
class Quaternion {
 public:
  /// @brief Construct an uninitialized Quaternion.
  inline Quaternion() {}

  /// @brief Construct a Quaternion from a copy.
  /// @param q Quaternion to copy.
  inline Quaternion(const Quaternion<T>& q) {
    s_ = q.s_;
    v_ = q.v_;
  }

  /// @brief Construct a Quaternion using scalar values to initialize each
  /// element.
  ///
  /// @param s1 Scalar component.
  /// @param s2 First element of the Vector component.
  /// @param s3 Second element of the Vector component.
  /// @param s4 Third element of the Vector component.
  inline Quaternion(const T& s1, const T& s2, const T& s3, const T& s4) {
    s_ = s1;
    v_ = Vector<T, 3>(s2, s3, s4);
  }

  /// @brief Construct a quaternion from a scalar and 3-dimensional Vector.
  ///
  /// @param s1 Scalar component.
  /// @param v1 Vector component.
  inline Quaternion(const T& s1, const Vector<T, 3>& v1) {
    s_ = s1;
    v_ = v1;
  }

  /// @brief Return the scalar component of the quaternion.
  ///
  /// @return The scalar component
  inline T& scalar() { return s_; }

  /// @brief Return the scalar component of the quaternion.
  ///
  /// @return The scalar component
  inline const T& scalar() const { return s_; }

  /// @brief Set the scalar component of the quaternion.
  ///
  /// @param s Scalar component.
  inline void set_scalar(const T& s) { s_ = s; }

  /// @brief Return the vector component of the quaternion.
  ///
  /// @return The scalar component
  inline Vector<T, 3>& vector() { return v_; }

  /// @brief Return the vector component of the quaternion.
  ///
  /// @return The scalar component
  inline const Vector<T, 3>& vector() const { return v_; }

  /// @brief Set the vector component of the quaternion.
  ///
  /// @param v Vector component.
  inline void set_vector(const Vector<T, 3>& v) { v_ = v; }

  /// @brief Calculate the inverse Quaternion.
  ///
  /// This calculates the inverse such that <code>(q * q).Inverse()</code>
  /// is the identity.
  ///
  /// @return Quaternion containing the result.
  inline Quaternion<T> Inverse() const {
    return Quaternion<T>(s_, -v_);
  }

  /// @brief Multiply this Quaternion with another Quaternion.
  ///
  /// @note This is equivalent to
  /// <code>FromMatrix(ToMatrix() * q.ToMatrix()).</code>
  /// @param q Quaternion to multiply with.
  /// @return Quaternion containing the result.
  inline Quaternion<T> operator*(const Quaternion<T>& q) const {
    return Quaternion<T>(
        s_ * q.s_ - Vector<T, 3>::DotProduct(v_, q.v_),
        s_ * q.v_ + q.s_ * v_ + Vector<T, 3>::CrossProduct(v_, q.v_));
  }

  /// @brief Multiply this Quaternion by a scalar.
  ///
  /// This multiplies the angle of the rotation by a scalar factor.
  /// @param s1 Scalar to multiply with.
  /// @return Quaternion containing the result.
  inline Quaternion<T> operator*(const T& s1) const {
    T angle;
    Vector<T, 3> axis;
    ToAngleAxis(&angle, &axis);
    angle *= s1;
    return Quaternion<T>(cos(0.5f * angle),
                         axis.Normalized() * sin(0.5f * angle));
  }

  /// @brief Multiply a Vector by this Quaternion.
  ///
  /// This will rotate the specified vector by the rotation specified by this
  /// Quaternion.
  ///
  /// @param v1 Vector to multiply by this Quaternion.
  /// @return Rotated Vector.
  inline Vector<T, 3> operator*(const Vector<T, 3>& v1) const {
    T ss = s_ + s_;
    return ss * Vector<T, 3>::CrossProduct(v_, v1) + (ss * s_ - 1) * v1 +
        2 * Vector<T, 3>::DotProduct(v_, v1) * v_;
  }

  /// @brief Normalize this quaterion (in-place).
  ///
  /// @return Length of the quaternion.
  inline T Normalize() {
    T length = sqrt(s_ * s_ + Vector<T, 3>::DotProduct(v_, v_));
    T scale = (1 / length);
    s_ *= scale;
    v_ *= scale;
    return length;
  }

  /// @brief Calculate the normalized version of this quaternion.
  ///
  /// @return The normalized quaternion.
  inline Quaternion<T> Normalized() {
    Quaternion<T> q(*this);
    q.Normalize();
    return q;
  }

  /// @brief Convert this Quaternion to an Angle and axis.
  ///
  /// The returned  angle is the size of the rotation in radians about the
  /// axis represented by this Quaternion.
  ///
  /// @param angle Receives the angle.
  /// @param axis Receives the normalized axis.
  inline void ToAngleAxis(T* angle, Vector<T, 3>* axis) const {
    *axis = s_ > 0 ? v_ : -v_;
    *angle = 2 * atan2(axis->Normalize(), s_ > 0 ? s_ : -s_);
  }

  /// @brief Convert this Quaternion to 3 Euler Angles.
  ///
  /// @return 3-dimensional Vector where each element is a angle of rotation
  /// (in radians) around the x, y, and z axes.
  inline Vector<T, 3> ToEulerAngles() const {
    Matrix<T, 3> m(ToMatrix());
    T cos2 = m[0] * m[0] + m[1] * m[1];
    if (cos2 < 1e-6f) {
      return Vector<T, 3>(
          0, m[2] < 0 ? static_cast<T>(0.5 * M_PI) :
              static_cast<T>(-0.5 * M_PI), -atan2(m[3], m[4]));
    } else {
      return Vector<T, 3>(
        atan2(m[5], m[8]), atan2(-m[2], sqrt(cos2)), atan2(m[1], m[0]));
    }
  }

  /// @brief Convert to a 3x3 Matrix.
  ///
  /// @return 3x3 rotation Matrix.
  inline Matrix<T, 3> ToMatrix() const {
    const T x2 = v_[0] * v_[0], y2 = v_[1] * v_[1], z2 = v_[2] * v_[2];
    const T sx = s_ * v_[0], sy = s_ * v_[1], sz = s_ * v_[2];
    const T xz = v_[0] * v_[2], yz = v_[1] * v_[2], xy = v_[0] * v_[1];
    return Matrix<T, 3>(1 - 2 * (y2 + z2), 2 * (xy + sz), 2 * (xz - sy),
                        2 * (xy - sz), 1 - 2 * (x2 + z2), 2 * (sx + yz),
                        2 * (sy + xz), 2 * (yz - sx), 1 - 2*(x2 + y2));
  }

  /// @brief Create a Quaternion from an angle and axis.
  ///
  /// @param angle Angle in radians to rotate by.
  /// @param axis Axis in 3D space to rotate around.
  /// @return Quaternion containing the result.
  static Quaternion<T> FromAngleAxis(const T& angle,
                                     const Vector<T, 3>& axis) {
    const T halfAngle = static_cast<T>(0.5) * angle;
    Vector<T, 3> localAxis(axis);
    return Quaternion<T>(cos(halfAngle),
                         localAxis.Normalized() * sin(halfAngle));
  }

  /// @brief Create a quaternion from 3 euler angles.
  ///
  /// @param angles 3-dimensional Vector where each element contains an
  /// angle in radius to rotate by about the x, y and z axes.
  /// @return Quaternion containing the result.
  static Quaternion<T> FromEulerAngles(const Vector<T, 3>& angles) {
    const Vector<T, 3> halfAngles(static_cast<T>(0.5) * angles[0],
                                  static_cast<T>(0.5) * angles[1],
                                  static_cast<T>(0.5) * angles[2]);
    const T sinx = sin(halfAngles[0]), cosx = cos(halfAngles[0]);
    const T siny = sin(halfAngles[1]), cosy = cos(halfAngles[1]);
    const T sinz = sin(halfAngles[2]), cosz = cos(halfAngles[2]);
    return Quaternion<T>(
      cosx * cosy * cosz + sinx * siny * sinz,
      sinx * cosy * cosz - cosx * siny * sinz,
      cosx * siny * cosz + sinx * cosy * sinz,
      cosx * cosy * sinz - sinx * siny * cosz);
  }

  /// @brief Create a quaternion from a rotation Matrix.
  ///
  /// @param m 3x3 rotation Matrix.
  /// @return Quaternion containing the result.
  static Quaternion<T> FromMatrix(const Matrix<T, 3>& m) {
    const T trace = m(0, 0) + m(1, 1) + m(2, 2);
    if (trace > 0) {
      const T s = sqrt(trace + 1) * 2;
      const T oneOverS = 1 / s;
      return Quaternion<T>(
          static_cast<T>(0.25) * s, (m[5] - m[7]) * oneOverS,
          (m[6] - m[2]) * oneOverS, (m[1] - m[3]) * oneOverS);
    } else if (m[0] > m[4] && m[0] > m[8]) {
      const T s = sqrt(m[0] - m[4] - m[8] + 1) * 2;
      const T oneOverS = 1 / s;
      return Quaternion<T>(
          (m[5] - m[7]) * oneOverS, static_cast<T>(0.25) * s,
          (m[3] + m[1]) * oneOverS, (m[6] + m[2]) * oneOverS);
    } else if (m[4] > m[8]) {
      const T s = sqrt(m[4] - m[0] - m[8] + 1) * 2;
      const T oneOverS = 1 / s;
      return Quaternion<T>(
          (m[6] - m[2]) * oneOverS, (m[3] + m[1]) * oneOverS,
          static_cast<T>(0.25) * s, (m[5] + m[7]) * oneOverS);
    } else {
      const T s = sqrt(m[8] - m[0] - m[4] + 1) * 2;
      const T oneOverS = 1 / s;
      return Quaternion<T>(
          (m[1] - m[3]) * oneOverS, (m[6] + m[2]) * oneOverS,
          (m[5] + m[7]) * oneOverS, static_cast<T>(0.25) * s);
    }
  }

  /// @brief Calculate the spherical linear interpolation between two
  /// Quaternions.
  ///
  /// @param q1 Start Quaternion.
  /// @param q2 End Quaternion.
  /// @param s1 The scalar value determining how far from q1 and q2 the
  /// resulting quaternion should be.  A value of 0 corresponds to q1 and a
  /// value of 1 corresponds to q2.
  /// @result Quaternion containing the result.
  static inline Quaternion<T> Slerp(
    const Quaternion<T>& q1, const Quaternion<T>& q2, const T& s1) {
    if(q1.s_ * q2.s_ + Vector<T, 3>::DotProduct(q1.v_, q2.v_) > 0.999999f)
      return Quaternion<T>(
        q1.s_ * (1 - s1) + q2.s_ * s1, q1.v_ * (1 - s1) + q2.v_ * s1);
    return q1 * ((q1.Inverse() * q2) * s1);
  }

  /// @brief Access an element of the quaternion.
  ///
  /// @param i Index of the element to access.
  /// @return A reference to the accessed data that can be modified by the
  /// caller.
  inline T& operator[](const int i) {
    if (i == 0) return s_;
    return v_[i - 1];
  }

  /// @brief Access an element of the quaternion.
  ///
  /// @param i Index of the element to access.
  /// @return A const reference to the accessed.
  inline const T& operator[](const int i) const {
    return const_cast<Quaternion<T>*>(this)->operator[](i);
  }


 private:
  T s_;
  Vector<T, 3> v_;
};
/// @}

/// @addtogroup mathfu_quaternion
/// @{

/// @brief Multiply a Quaternion by a scalar.
///
/// This multiplies the angle of the rotation of the specified Quaternion
/// by a scalar factor.
/// @param s Scalar to multiply with.
/// @param q Quaternion to scale.
/// @return Quaternion containing the result.
///
/// @related Quaternion
template<class T>
inline Quaternion<T> operator*(const T& s, const Quaternion<T>& q) {
  return q * s;
}
/// @}

}  // namespace mathfu
#endif  // MATHFU_QUATERNION_H_

