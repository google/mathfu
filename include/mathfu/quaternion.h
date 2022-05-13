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
#define _USE_MATH_DEFINES  // For M_PI.
#endif                     // !defined(_USE_MATH_DEFINES)
#endif                     // _WIN32

#include "mathfu/matrix.h"
#include "mathfu/vector.h"

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
template <class T>
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
  /// @param qs1 First element of the Vector component.
  /// @param qs2 Second element of the Vector component.
  /// @param qs3 Third element of the Vector component.
  inline Quaternion(T s1, T qs1, T qs2, T qs3) {
    s_ = s1;
    v_ = Vector<T, 3>(qs1, qs2, qs3);
  }

  /// @brief Construct a quaternion from a scalar and 3-dimensional Vector.
  ///
  /// @param s1 Scalar component.
  /// @param v1 Vector component.
  inline Quaternion(T s1, const Vector<T, 3>& v1) {
    s_ = s1;
    v_ = v1;
  }

  /// @brief Return the scalar component of the quaternion.
  ///
  /// @return The scalar component
  inline const T& scalar() const { return s_; }

  /// @brief Set the scalar component of the quaternion.
  ///
  /// @param s Scalar component.
  inline void set_scalar(T s) { s_ = s; }

  /// @brief Return the vector component of the quaternion.
  ///
  /// @return The vector component
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
  inline Quaternion<T> Inverse() const { return Quaternion<T>(s_, -v_); }

  /// @brief Add this Quaternion to another Quaternion.
  ///
  /// @param q Quaternion to add.
  /// @return Quaternion containing the result.
  inline Quaternion<T> operator+(const Quaternion<T>& q) const {
    return Quaternion<T>(s_ + q.s_, v_ + q.v_);
  }

  /// @brief Add another Quaternion to this, and store the result.
  ///
  /// @param q Quaternion to add.
  /// @return Quaternion containing the result.
  inline Quaternion<T>& operator+=(const Quaternion<T>& q) {
    s_ += q.s_;
    v_ += q.v_;
    return *this;
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
  /// This conditions the Quaternion to be a rotation <= 180 degrees, then
  /// multiplies the angle of the rotation by a scalar factor.
  ///
  /// If the scalar factor is < 1, the resulting rotation will be on the shorter
  /// of the two paths to the identity orientation, which is often intuitive but
  /// can trip you up if you really did want to take the longer path.
  ///
  /// If the scalar factor is > 1, the resulting rotation will be on the longer
  /// of the two paths to the identity orientation, which can be unintuitive.
  /// For example, you are not guaranteed that (q * 2) * .5 and q * (2 * .5)
  /// are the same orientation, let alone the same quaternion.
  ///
  /// @param s1 Scalar to multiply with.
  /// @return Quaternion containing the result.
  inline Quaternion<T> operator*(T s1) const {
    T angle;
    Vector<T, 3> axis;
    ToAngleAxis(&angle, &axis);
    angle *= s1;
    // The axis coming from ToAngleAxis() is already normalized, but
    // ToAngleAxis may return slightly non-normal axes in unstable cases.
    // It should arguably handle that internally, allowing us to remove
    // the Normalized() here.
    return Quaternion<T>(cos(0.5f * angle),
                         axis.Normalized() * static_cast<T>(sin(0.5f * angle)));
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

  /// @brief Normalize this quaternion (in-place).
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
  inline Quaternion<T> Normalized() const {
    Quaternion<T> q(*this);
    q.Normalize();
    return q;
  }

  /// @brief Convert this Quaternion to a shortest-path angle and axis.
  ///
  /// The resulting angle-axis is guaranteed to have have angle <= 180 and
  /// represents the same orientation as *this, but it may not convert back to
  /// *this.
  ///
  /// For example, if *this represents "Rotate 350 degrees left", you will
  /// get the angle-axis "Rotate 10 degrees right".
  ///
  /// @param angle Receives the angle, in the range [0, pi].
  /// @param axis Receives the normalized axis.
  inline void ToAngleAxis(T* out_angle, Vector<T, 3>* out_axis) const {
    const Quaternion<T> q = (s_ > 0) ? *this : Quaternion<T>(-s_, -v_);
    q.ToAngleAxisFull(out_angle, out_axis);
  }

  /// @brief Convert this Quaternion to an angle and axis.
  ///
  /// The resulting angle-axis uses the full range of angles supported by
  /// quaternions, and will convert back to the original Quaternion.
  ///
  /// @param angle Receives the angle, in the range [0, 2pi).
  /// @param axis Receives the normalized axis.
  inline void ToAngleAxisFull(T* out_angle, Vector<T, 3>* out_axis) const {
    Vector<T, 3> axis = v_;
    const T axis_length = axis.Normalize();
    if (axis_length == 0) {
      // Normalize has left NaNs in axis.  This happens at angle = 0 and 360.
      // All axes are correct, so any will do.
      *out_axis = Vector<T, 3>(1, 0, 0);
    } else {
      *out_axis = axis;
    }
    *out_angle = 2 * atan2(axis_length, s_);
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
          0,
          m[2] < 0 ? static_cast<T>(0.5 * M_PI) : static_cast<T>(-0.5 * M_PI),
          -std::atan2(m[3], m[4]));
    } else {
      return Vector<T, 3>(std::atan2(m[5], m[8]),
                          std::atan2(-m[2], std::sqrt(cos2)),
                          std::atan2(m[1], m[0]));
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
                        2 * (sy + xz), 2 * (yz - sx), 1 - 2 * (x2 + y2));
  }

  /// @brief Convert to a 4x4 Matrix.
  ///
  /// @return 4x4 transform Matrix.
  inline Matrix<T, 4> ToMatrix4() const {
    const T x2 = v_[0] * v_[0], y2 = v_[1] * v_[1], z2 = v_[2] * v_[2];
    const T sx = s_ * v_[0], sy = s_ * v_[1], sz = s_ * v_[2];
    const T xz = v_[0] * v_[2], yz = v_[1] * v_[2], xy = v_[0] * v_[1];
    return Matrix<T, 4>(1 - 2 * (y2 + z2), 2 * (xy + sz), 2 * (xz - sy), 0.0f,
                        2 * (xy - sz), 1 - 2 * (x2 + z2), 2 * (sx + yz), 0.0f,
                        2 * (sy + xz), 2 * (yz - sx), 1 - 2 * (x2 + y2), 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);
  }

  /// @brief Create a Quaternion from an angle and axis.
  ///
  /// @param angle Angle in radians to rotate by.
  /// @param axis Axis in 3D space to rotate around.
  /// @return Quaternion containing the result.
  static Quaternion<T> FromAngleAxis(T angle, const Vector<T, 3>& axis) {
    const T halfAngle = static_cast<T>(0.5) * angle;
    Vector<T, 3> localAxis(axis);
    return Quaternion<T>(
        cos(halfAngle),
        localAxis.Normalized() * static_cast<T>(sin(halfAngle)));
  }

  /// @brief Create a quaternion from 3 euler angles.
  ///
  /// @param angles 3-dimensional Vector where each element contains an
  /// angle in radians to rotate by about the x, y and z axes.
  /// @return Quaternion containing the result.
  static Quaternion<T> FromEulerAngles(const Vector<T, 3>& angles) {
    const Vector<T, 3> halfAngles(static_cast<T>(0.5) * angles[0],
                                  static_cast<T>(0.5) * angles[1],
                                  static_cast<T>(0.5) * angles[2]);
    const T sinx = std::sin(halfAngles[0]);
    const T cosx = std::cos(halfAngles[0]);
    const T siny = std::sin(halfAngles[1]);
    const T cosy = std::cos(halfAngles[1]);
    const T sinz = std::sin(halfAngles[2]);
    const T cosz = std::cos(halfAngles[2]);
    return Quaternion<T>(cosx * cosy * cosz + sinx * siny * sinz,
                         sinx * cosy * cosz - cosx * siny * sinz,
                         cosx * siny * cosz + sinx * cosy * sinz,
                         cosx * cosy * sinz - sinx * siny * cosz);
  }

  /// @brief Create a quaternion from 3 euler angles.
  ///
  /// @param x_rotation angle in radians to rotate by about the x axis.
  /// @param y_rotation angle in radians to rotate by about the y axis.
  /// @param z_rotation angle in radians to rotate by about the z axis.
  /// @return Quaternion containing the result.
  static Quaternion<T> FromEulerAngles(T x_rotation,
                                       T y_rotation,
                                       T z_rotation) {
    return FromEulerAngles(Vector<T, 3>(x_rotation, y_rotation, z_rotation));
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
      return Quaternion<T>(static_cast<T>(0.25) * s, (m[5] - m[7]) * oneOverS,
                           (m[6] - m[2]) * oneOverS, (m[1] - m[3]) * oneOverS);
    } else if (m[0] > m[4] && m[0] > m[8]) {
      const T s = sqrt(m[0] - m[4] - m[8] + 1) * 2;
      const T oneOverS = 1 / s;
      return Quaternion<T>((m[5] - m[7]) * oneOverS, static_cast<T>(0.25) * s,
                           (m[3] + m[1]) * oneOverS, (m[6] + m[2]) * oneOverS);
    } else if (m[4] > m[8]) {
      const T s = sqrt(m[4] - m[0] - m[8] + 1) * 2;
      const T oneOverS = 1 / s;
      return Quaternion<T>((m[6] - m[2]) * oneOverS, (m[3] + m[1]) * oneOverS,
                           static_cast<T>(0.25) * s, (m[5] + m[7]) * oneOverS);
    } else {
      const T s = sqrt(m[8] - m[0] - m[4] + 1) * 2;
      const T oneOverS = 1 / s;
      return Quaternion<T>((m[1] - m[3]) * oneOverS, (m[6] + m[2]) * oneOverS,
                           (m[5] + m[7]) * oneOverS, static_cast<T>(0.25) * s);
    }
  }

  /// @brief Create a quaternion from the upper-left 3x3 roation Matrix of a 4x4
  /// Matrix.
  ///
  /// @param m 4x4 Matrix.
  /// @return Quaternion containing the result.
  static Quaternion<T> FromMatrix(const Matrix<T, 4>& m) {
    const T trace = m(0, 0) + m(1, 1) + m(2, 2);
    if (trace > 0) {
      const T s = sqrt(trace + 1) * 2;
      const T oneOverS = 1 / s;
      return Quaternion<T>(static_cast<T>(0.25) * s, (m[6] - m[9]) * oneOverS,
                           (m[8] - m[2]) * oneOverS, (m[1] - m[4]) * oneOverS);
    } else if (m[0] > m[5] && m[0] > m[10]) {
      const T s = sqrt(m[0] - m[5] - m[10] + 1) * 2;
      const T oneOverS = 1 / s;
      return Quaternion<T>((m[6] - m[9]) * oneOverS, static_cast<T>(0.25) * s,
                           (m[4] + m[1]) * oneOverS, (m[8] + m[2]) * oneOverS);
    } else if (m[5] > m[10]) {
      const T s = sqrt(m[5] - m[0] - m[10] + 1) * 2;
      const T oneOverS = 1 / s;
      return Quaternion<T>((m[8] - m[2]) * oneOverS, (m[4] + m[1]) * oneOverS,
                           static_cast<T>(0.25) * s, (m[6] + m[9]) * oneOverS);
    } else {
      const T s = sqrt(m[10] - m[0] - m[5] + 1) * 2;
      const T oneOverS = 1 / s;
      return Quaternion<T>((m[1] - m[4]) * oneOverS, (m[8] + m[2]) * oneOverS,
                           (m[6] + m[9]) * oneOverS, static_cast<T>(0.25) * s);
    }
  }

  /// @brief Calculate the dot product of two Quaternions.
  ///
  /// @param q1 First quaternion.
  /// @param q2 Second quaternion
  /// @return The scalar dot product of both Quaternions.
  static inline T DotProduct(const Quaternion<T>& q1, const Quaternion<T>& q2) {
    return q1.s_ * q2.s_ + Vector<T, 3>::DotProduct(q1.v_, q2.v_);
  }

  /// @brief Calculate the shortest-path spherical linear interpolation between
  /// two orientations.
  ///
  /// This method always gives you the "short way around" interpolation. If you
  /// need mathematical Slerp(), use ToAngleAxisFull() and FromAngleAxis().
  ///
  /// @param q1 Start Quaternion.
  /// @param q2 End Quaternion.
  /// @param s1 The scalar value determining how far from q1 and q2 the
  /// resulting quaternion should be.  A value of 0 corresponds to q1 and a
  /// value of 1 corresponds to q2.
  /// @result Quaternion containing the result.
  static inline Quaternion<T> Slerp(const Quaternion<T>& q1,
                                    const Quaternion<T>& q2, T s1) {
    if (q1.s_ * q2.s_ + Vector<T, 3>::DotProduct(q1.v_, q2.v_) > 0.9999f) {
      return Quaternion<T>(q1.s_ * (1 - s1) + q2.s_ * s1,
                           q1.v_ * (1 - s1) + q2.v_ * s1)
          .Normalized();
    }
    return q1 * ((q1.Inverse() * q2) * s1);
  }

  /// @brief Access an element of the quaternion.
  ///
  /// @param i Index of the element to access.
  /// @return A const reference to the accessed.
  inline T operator[](const int i) const { return i == 0 ? s_ : v_[i - 1]; }

  /// @brief Returns a vector that is perpendicular to the supplied vector.
  ///
  /// @param v1 An arbitrary vector
  /// @return A vector perpendicular to v1.  Normally this will just be
  /// the cross product of v1, v2.  If they are parallel or opposite though,
  /// the routine will attempt to pick a vector.
  static inline Vector<T, 3> PerpendicularVector(const Vector<T, 3>& v) {
    // We start out by taking the cross product of the vector and the x-axis to
    // find something parallel to the input vectors.  If that cross product
    // turns out to be length 0 (i. e. the vectors already lie along the x axis)
    // then we use the y-axis instead.
    Vector<T, 3> axis = Vector<T, 3>::CrossProduct(
        Vector<T, 3>(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)),
        v);
    // We use a fairly high epsilon here because we know that if this number
    // is too small, the axis we'll get from a cross product with the y axis
    // will be much better and more numerically stable.
    if (axis.LengthSquared() < static_cast<T>(0.05)) {
      axis = Vector<T, 3>::CrossProduct(
          Vector<T, 3>(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0)),
          v);
    }
    return axis;
  }

  /// @brief Returns the a Quaternion that rotates from start to end.
  ///
  /// @param v1 The starting vector
  /// @param v2 The vector to rotate to
  /// @param preferred_axis the axis to use, if v1 and v2 are parallel.
  /// @return A Quaternion describing the rotation from v1 to v2
  /// See the comment on RotateFromToWithAxis for an explanation of the math.
  static inline Quaternion<T> RotateFromToWithAxis(
      const Vector<T, 3>& v1, const Vector<T, 3>& v2,
      const Vector<T, 3>& preferred_axis) {
    Vector<T, 3> start = v1.Normalized();
    Vector<T, 3> end = v2.Normalized();

    T dot_product = Vector<T, 3>::DotProduct(start, end);
    // Any rotation < 0.1 degrees is treated as no rotation
    // in order to avoid division by zero errors.
    // So we early-out in cases where it's less than 0.1 degrees.
    // cos( 0.1 degrees) = 0.99999847691
    if (dot_product >= static_cast<T>(0.99999847691)) {
      return Quaternion<T>::identity;
    }
    // If the vectors point in opposite directions, return a 180 degree
    // rotation, on the axis that they asked for.
    if (dot_product <= static_cast<T>(-0.99999847691)) {
      return Quaternion<T>(static_cast<T>(0), preferred_axis);
    }
    // Degenerate cases have been handled, so if we're here, we have to
    // actually compute the angle we want:
    Vector<T, 3> cross_product = Vector<T, 3>::CrossProduct(start, end);

    return Quaternion<T>(static_cast<T>(1.0) + dot_product, cross_product)
        .Normalized();
  }

  /// @brief Returns the a Quaternion that rotates from start to end.
  ///
  /// @param v1 The starting vector
  /// @param v2 The vector to rotate to
  /// @return A Quaternion describing the rotation from v1 to v2.  In the case
  /// where the vectors are parallel, it returns the identity.  In the case
  /// where
  /// they point in opposite directions, it picks an arbitrary axis.  (Since
  /// there
  /// are technically infinite possible quaternions to represent a 180 degree
  /// rotation.)
  ///
  /// The final equation used here is fairly elegant, but its derivation is
  /// not obvious:  We want to find the quaternion that represents the angle
  /// between Start and End.
  ///
  /// The angle can be expressed as a quaternion with the values:
  ///  angle: ArcCos(dotproduct(start, end) / (|start|*|end|)
  ///  axis: crossproduct(start, end).normalized * sin(angle/2)
  ///
  /// or written as:
  ///  quaternion(cos(angle/2), axis * sin(angle/2))
  ///
  /// Using the trig identity:
  ///  sin(angle * 2) = 2 * sin(angle) * cos*angle)
  /// Via substitution, we can turn this into:
  ///  sin(angle/2) = 0.5 * sin(angle)/cos(angle/2)
  ///
  /// Using this substitution, we get:
  ///  quaternion( cos(angle/2),
  ///             0.5 * crossproduct(start, end).normalized
  ///                 * sin(angle) / cos(angle/2))
  ///
  /// If we scale the whole thing up by 2 * cos(angle/2) then we get:
  ///  quaternion(2 * cos(angle/2) * cos(angle/2),
  ///             crossproduct(start, end).normalized * sin(angle))
  ///
  /// (Note that the quaternion is no longer normalized after this scaling)
  ///
  /// Another trig identity:
  ///   cos(angle/2) = sqrt((1 + cos(angle) / 2)
  ///
  /// Substituting this in, we can simplify the quaternion scalar:
  ///
  ///  quaternion(1 + cos(angle),
  ///             crossproduct(start, end).normalized * sin(angle))
  ///
  /// Because cross(start, end) has a magnitude of |start|*|end|*sin(angle),
  ///  crossproduct(start,end).normalized
  /// is equivalent to
  ///  crossproduct(start,end) / |start| * |end| * sin(angle)
  /// So after that substitution:
  ///
  ///  quaternion(1 + cos(angle),
  ///             crossproduct(start, end) / (|start| * |end|))
  ///
  /// dotproduct(start, end) has the value of |start| * |end| * cos(angle),
  /// so by algebra,
  ///  cos(angle) = dotproduct(start, end) / (|start| * |end|)
  /// we can replace our quaternion scalar here also:
  ///
  ///  quaternion(1 + dotproduct(start, end) / (|start| * |end|),
  ///             crossproduct(start, end) / (|start| * |end|))
  ///
  /// If start and end are normalized, then |start| * |end| = 1, giving us a
  /// final quaternion of:
  ///
  /// quaternion(1 + dotproduct(start, end), crossproduct(start, end))
  static inline Quaternion<T> RotateFromTo(const Vector<T, 3>& v1,
                                           const Vector<T, 3>& v2) {
    Vector<T, 3> start = v1.Normalized();
    Vector<T, 3> end = v2.Normalized();

    T dot_product = Vector<T, 3>::DotProduct(start, end);
    // Any rotation < 0.1 degrees is treated as no rotation
    // in order to avoid division by zero errors.
    // So we early-out in cases where it's less than 0.1 degrees.
    // cos( 0.1 degrees) = 0.99999847691
    if (dot_product >= static_cast<T>(0.99999847691)) {
      return Quaternion<T>::identity;
    }
    // If the vectors point in opposite directions, return a 180 degree
    // rotation, on an arbitrary axis.
    if (dot_product <= static_cast<T>(-0.99999847691)) {
      return Quaternion<T>(0, PerpendicularVector(start));
    }
    // Degenerate cases have been handled, so if we're here, we have to
    // actually compute the angle we want:
    Vector<T, 3> cross_product = Vector<T, 3>::CrossProduct(start, end);

    return Quaternion<T>(static_cast<T>(1.0) + dot_product, cross_product)
        .Normalized();
  }

  /// @cond MATHFU_INTERNAL
  /// Create a 3-dimensional look-at matrix.
  static inline Matrix<T, 3, 3> LookAtHelper(const Vector<T, 3>& forward,
                                          const Vector<T, 3>& up,
                                          T handedness) {
    Matrix<T, 3, 3> m;
    m.GetColumn(2) = -handedness * forward;
    const Vector<T, 3>& right = Vector<T, 3>::CrossProduct(up, m.GetColumn(2));
    
    // avoid division by zero errors.
    T inverse = 1 / sqrt(std::max<T>(static_cast<T>(0.00001), Vector<T, 3>::DotProduct(right, right)));
    m.GetColumn(0) = right * inverse;
    m.GetColumn(1) = Vector<T, 3>::CrossProduct(m.GetColumn(2), m.GetColumn(0));
    return m;
  }
  /// @endcond

  /// @brief Returns a quaternion looking at forward vector with an up vector based on the default handedness.
  ///
  /// @param forward The forward vector (Vector to face).
  /// @param up The up vector.
  /// @param Forward and up do not have to be orthogonal.
  /// @param Forward and up cannot be parallel.
  /// @param Forward and up cannot be zero vectors.
  /// @param handedness 1.0f for RH, -1.0f for LH(default).
  /// @return A Quaternion looking at forward vector with an up vector.
  ///
  /// Uses Quaternion::LookAtHelper to get the Rotation Matrix, then convert it to Quat.
  /// Quaternion::LookAtHelper takes forward direction and Up vector as first and second param.
  /// The forward vector needs to be normalized.
  /// Up vector, how the camera is oriented. Typically (0, 1, 0).
  static inline Quaternion<T> LookAt(const Vector<T, 3>& forward,
                                     const Vector<T, 3>& up,
                                     T handedness = -1) {
    return FromMatrix(LookAtHelper(forward, up, handedness));
  }

  /// @brief Contains a quaternion doing the identity transform.
  static Quaternion<T> identity;

  MATHFU_DEFINE_CLASS_SIMD_AWARE_NEW_DELETE

 private:
  Vector<T, 3> v_;
  T s_;
};

template <typename T>
Quaternion<T> Quaternion<T>::identity = Quaternion<T>(1, 0, 0, 0);
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
template <class T>
inline Quaternion<T> operator*(T s, const Quaternion<T>& q) {
  return q * s;
}
/// @}

}  // namespace mathfu
#endif  // MATHFU_QUATERNION_H_
