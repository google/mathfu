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
#define _USE_MATH_DEFINES // For M_PI.
#endif  // _WIN32

#include "mathfu/matrix.h"
#include "mathfu/vector_3.h"

#include <math.h>

namespace mathfu {

/// @class Quaternion
/// Stores a quaternion of type T and provides a set of utility
/// operations on each quaternion.
template<class T>
class Quaternion {
 public:
  /// Create a quaternion of unitialized values.
  inline Quaternion() {}

  /// Create a quaternion from another vector copying each element.
  /// @param q Quaternion that the data will be copied from.
  inline Quaternion(const Quaternion<T>& q) {
    s_ = q.s_;
    v_ = q.v_;
  }

  /// Create a quaternion from four values.
  /// @param s1 Scalar value for the scalar component of the quaternion.
  /// @param s2 Scalar value for the first element of the vector component.
  /// @param s3 Scalar value for the second element of the vector component.
  /// @param s4 Scalar value for the third element of the vector component.
  inline Quaternion(const T& s1, const T& s2, const T& s3, const T& s4) {
    s_ = s1;
    v_ = Vector<T, 3>(s2, s3, s4);
  }

  /// Create a quaternion from a scalar and vector of size 3.
  /// @param s1 Scalar value for the scalar component of the quaternion.
  /// @param v1 Vector value for the vector component of the quaternion.
  inline Quaternion(const T& s1, const Vector<T, 3>& v1) {
    s_ = s1;
    v_ = v1;
  }

  /// Find the inverse quaternion such that q*q.Inverse() is the identity.
  /// @return A new quaternion that stores the result.
  inline Quaternion<T> Inverse() const {
    return Quaternion<T>(s_, -v_);
  }

  /// Quaternion/Quaternion multiplication. Note that this is equivalent to
  /// FromMatrix(ToMatrix() * q.ToMatrix()).
  /// @param q The quaternion to multiply with.
  /// @return A new quaternion that stores the result.
  inline Quaternion<T> operator*(const Quaternion<T>& q) const {
    return Quaternion<T>(
        s_ * q.s_ - Vector<T, 3>::DotProduct(v_, q.v_),
        s_ * q.v_ + q.s_ * v_ + Vector<T, 3>::CrossProduct(v_, q.v_));
  }

  /// Quaternion/Scalar multiplcation. This will multiply the angle of the
  /// rotation by the scalar factor.
  /// @param s1 The scalar to multiply with
  /// @return A new quaternion that stores the result.
  inline Quaternion<T> operator*(const T& s1) const {
    T angle;
    Vector<T, 3> axis;
    ToAngleAxis(&angle, &axis);
    angle *= s1;
    return Quaternion<T>(cos(0.5f * angle),
                         axis.Normalized() * sin(0.5f * angle));
  }

  /// Quaternion/Vector multiplication. This will rotate the given vector by
  /// the rotation stored within the quaternion.
  /// @param v1 The vector to multiply with.
  /// @return A new quaternion that stores the result.
  inline Vector<T, 3> operator*(const Vector<T, 3>& v1) const {
    T ss = s_ + s_;
    return ss * Vector<T, 3>::CrossProduct(v_, v1) + (ss * s_ - 1) * v1 +
        2 * Vector<T, 3>::DotProduct(v_, v1) * v_;
  }

  /// Normalize the quaterion.
  /// @return The length of the quaternion.
  inline T Normalize() {
    T length = sqrt(s_ * s_ + v_ * v_);
    T scale = (1 / length);
    s_ *= scale;
    v_ *= scale;
    return length;
  }

  /// Convert to an Angle and axis. The angle is the size of the rotation in
  /// radians about the axis that this quaternion represents.
  /// @param angle An empty scalar to be filed with the angle.
  /// @param axis An empty vector to be filed with the normalized axis.
  inline void ToAngleAxis(T* angle, Vector<T, 3>* axis) const {
    *axis = s_ > 0 ? v_ : -v_;
    *angle = 2 * atan2(axis->Normalize(), s_ > 0 ? s_ : -s_);
  }

  /// Convert to 3 Euler Angles. The angles correspond to rotations (in radians)
  /// around the x, y, and z axes.
  /// @return A Vector of size 3 filed with the result.
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

  /// Convert to a matrix. The matrix is a rotation matrix that corresponds to
  /// the rotation in the quaternion.
  /// @return A 3x3 rotation Matrix with the result.
  inline Matrix<T, 3> ToMatrix() const {
    const T x2 = v_[0] * v_[0], y2 = v_[1] * v_[1], z2 = v_[2] * v_[2];
    const T sx = s_ * v_[0], sy = s_ * v_[1], sz = s_ * v_[2];
    const T xz = v_[0] * v_[2], yz = v_[1] * v_[2], xy = v_[0] * v_[1];
    return Matrix<T, 3>(1 - 2 * (y2 + z2), 2 * (xy + sz), 2 * (xz - sy),
                        2 * (xy - sz), 1 - 2 * (x2 + z2), 2 * (sx + yz),
                        2 * (sy + xz), 2 * (yz - sx), 1 - 2*(x2 + y2));
  }

  /// Create a quaternion from an angle and axis.
  /// @prarm angle The size of the angle in radians to rotate by.
  /// @param axis The axis in 3D space to rotate around
  /// @return A new quaternion that stores the result.
  static Quaternion<T> FromAngleAxis(const T& angle,
                                     const Vector<T, 3>& axis) {
    const T halfAngle = static_cast<T>(0.5) * angle;
    Vector<T, 3> localAxis(axis);
    return Quaternion<T>(cos(halfAngle),
                         localAxis.Normalized() * sin(halfAngle));
  }

  /// Create a quaternion from 3 euler angles. The angles correspond to
  /// rotations about the x, y, and z axes.
  /// @param angles The 3 angles in radians to rotate by.
  /// @return A new quaternion that stores the result.
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

  /// Create a quaternion from a rotation matrix.
  /// @param m The 3x3 rotation matrix that stores the rotation.
  /// @return A new quaternion that stores the result.
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

  /// Spherical linear interpolation between two quaternions.
  /// @param q1 The first quaternion.
  /// @param q2 The second quaternion.
  /// @param s1 The scalar value determining how far from q1 and q2 the
  /// resulting quaternion should be. A value of 0 corresponds to q1 and a
  /// value of 1 corresponds to q2.
  /// @result A new quaternion that stores the result.
  static inline Quaternion<T> Slerp(
    const Quaternion<T>& q1, const Quaternion<T>& q2, const T& s1) {
    if(q1.s_ * q2.s_ + Vector<T, 3>::DotProduct(q1.v_, q2.v_) > 0.999999f)
      return Quaternion<T>(
        q1.s_ * (1 - s1) + q2.s_ * s1, q1.v_ * (1 - s1) + q2.v_ * s1);
    return q1 * ((q1.Inverse() * q2) * s1);
  }

 private:
  T s_;
  Vector<T, 3> v_;
};

template<class T>
inline Quaternion<T> operator*(const T& s, const Quaternion<T>& q) {
  return q * s;
}

}  // namespace mathfu
#endif  // MATHFU_QUATERNION_H_
