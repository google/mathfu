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
#include "mathfu/quaternion.h"
#include "mathfu/constants.h"
#include "mathfu/io.h"

#include <math.h>

#include "gtest/gtest.h"

#include "precision.h"

namespace {

using ::testing::AssertionResult;

class QuaternionTests : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

// This will automatically generate tests for each template parameter.
#define TEST_ALL_F(MY_TEST)                   \
  TEST_F(QuaternionTests, MY_TEST) {          \
    MY_TEST##_Test<float>(FLOAT_PRECISION);   \
    MY_TEST##_Test<double>(DOUBLE_PRECISION); \
  }

#define EXPECT_NEAR_VEC3(v1, v2, abs_error)             \
  EXPECT_TRUE(IsNearVector((v1), (v2), (abs_error)))    \
      << v1 << "\n" << v2 << "\n"

#define EXPECT_EQ_QUAT(q1, q2)               \
  {                                          \
    EXPECT_EQ((q1).scalar(), (q2).scalar()); \
    EXPECT_EQ((q1).vector(), (q2).vector()); \
  }

#define EXPECT_NEAR_QUAT(q1, q2, abs_error)             \
  EXPECT_TRUE(IsNearQuat((q1), (q2), (abs_error)))      \
      << q1 << "\n" << q2 << "\n"

#define EXPECT_NEAR_ORIENTATION(q1, q2, abs_error)        \
  EXPECT_TRUE(IsNearOrientation((q1), (q2), (abs_error))) \
      << q1 << "\n" << q2 << "\n"

AssertionResult IsNearDouble(double val1, double val2, double abs_error) {
  const double diff = std::fabs(val1 - val2);
  if (diff <= abs_error) {
    return ::testing::AssertionSuccess();
  }
  return ::testing::AssertionFailure()
      << "The difference between " << val1 << " and " << val2
      << " is " << diff << ", which exceeds " << abs_error << ".";
}

template<class T, int d>
AssertionResult IsNearVector(
     mathfu::Vector<T, d> v1, mathfu::Vector<T, d> v2, double abs_error) {
  for (int i = 0; i < d; ++i) {
    AssertionResult result = IsNearDouble(v1[i], v2[i], abs_error);
    if (!result) {
      return result << " at v[" << i << "]";
    }
  }
  return ::testing::AssertionSuccess();
}

template<class T>
AssertionResult IsNearQuat(
     mathfu::Quaternion<T> q1, mathfu::Quaternion<T> q2, double abs_error) {
  {
    AssertionResult result = IsNearDouble(q1.scalar(), q2.scalar(), abs_error);
    if (!result) {
      return result << " at .scalar() " << q1 << " " << q2;
    }
  }
  {
    AssertionResult result =
        IsNearVector(q1.vector(), q2.vector(), abs_error);
    if (!result) {
      return result << " at .vector() " << q1 << " " << q2;
    }
  }
  return ::testing::AssertionSuccess();
}

// Unlike IsNearQuat, this test considers q and -q to be equivalent.
// This is appropriate when treating quats as orientations (rather than
// rotations).
template<class T>
AssertionResult IsNearOrientation(
     mathfu::Quaternion<T> q1, mathfu::Quaternion<T> q2, double abs_error) {
  // Put them both into the same hemisphere.
  if (mathfu::Quaternion<T>::DotProduct(q1, q2) < 0) {
    q2 = mathfu::Quaternion<T>(-q2.scalar(), -q2.vector());
  }
  return IsNearQuat(q1, q2, abs_error);
}

// Test our test helpers.
template<class T>
void TestHelpers_Test(const T& precision) {
  using Quaternion = mathfu::Quaternion<T>;
  const double epsilon = 1e-5;

  EXPECT_NEAR_QUAT(Quaternion(1, 0, 0, 1e-6f), Quaternion::identity, epsilon);

  // Test that opposing quats are !IsNearQuat and IsNearOrientation.
  const Quaternion q2 = Quaternion(3, 4, 5, 6).Normalized();
  const Quaternion q2_negated(-q2.scalar(), -q2.vector());
  EXPECT_FALSE(IsNearQuat(q2, q2_negated, epsilon));
  EXPECT_TRUE(IsNearOrientation(q2, q2_negated, epsilon));
  EXPECT_NEAR_ORIENTATION(q2, q2_negated, epsilon);
}
TEST_ALL_F(TestHelpers)

// Test accessing elements of the quaternion using the const array accessor.
template <class T>
void ConstAccessor_Test(const T& precision) {
  (void)precision;
  const mathfu::Quaternion<T> quaternion(
      static_cast<T>(0.50), static_cast<T>(0.76), static_cast<T>(0.38),
      static_cast<T>(0.19));
  EXPECT_EQ(static_cast<T>(0.50), quaternion[0]);
  EXPECT_EQ(static_cast<T>(0.76), quaternion[1]);
  EXPECT_EQ(static_cast<T>(0.38), quaternion[2]);
  EXPECT_EQ(static_cast<T>(0.19), quaternion[3]);
}
TEST_ALL_F(ConstAccessor)

// Test accessing the scalar component of the quaternion using the scalar
// accessor.
template <class T>
void ScalarAccessor_Test(const T& precision) {
  (void)precision;
  mathfu::Quaternion<T> quaternion(static_cast<T>(0.50), static_cast<T>(0.76),
                                   static_cast<T>(0.38), static_cast<T>(0.19));
  EXPECT_EQ(static_cast<T>(0.50), quaternion.scalar());
}
TEST_ALL_F(ScalarAccessor)

// Test accessing the scalar component of the quaternion using the const scalar
// accessor.
template <class T>
void ConstScalarAccessor_Test(const T& precision) {
  (void)precision;
  const mathfu::Quaternion<T> quaternion(
      static_cast<T>(0.50), static_cast<T>(0.76), static_cast<T>(0.38),
      static_cast<T>(0.19));
  EXPECT_EQ(static_cast<T>(0.50), quaternion.scalar());
}
TEST_ALL_F(ConstScalarAccessor)

// Test mutating the scalar component of the quaternion using the scalar
// mutator.
template <class T>
void ScalarMutator_Test(const T& precision) {
  (void)precision;
  mathfu::Quaternion<T> quaternion;
  quaternion.set_scalar(static_cast<T>(0.38));
  EXPECT_EQ(static_cast<T>(0.38), quaternion[0]);
}
TEST_ALL_F(ScalarMutator)

// Test accessing elements of the quaternion using the vector accessor.
template <class T>
void VectorAccessor_Test(const T& precision) {
  (void)precision;
  mathfu::Quaternion<T> quaternion(static_cast<T>(0.50), static_cast<T>(0.76),
                                   static_cast<T>(0.38), static_cast<T>(0.19));
  EXPECT_EQ(static_cast<T>(0.76), quaternion.vector()[0]);
  EXPECT_EQ(static_cast<T>(0.38), quaternion.vector()[1]);
  EXPECT_EQ(static_cast<T>(0.19), quaternion.vector()[2]);
}
TEST_ALL_F(VectorAccessor)

// Test accessing elements of the quaternion using the const vector accessor.
template <class T>
void ConstVectorAccessor_Test(const T& precision) {
  (void)precision;
  const mathfu::Quaternion<T> quaternion(
      static_cast<T>(0.50), static_cast<T>(0.76), static_cast<T>(0.38),
      static_cast<T>(0.19));
  EXPECT_EQ(static_cast<T>(0.76), quaternion.vector()[0]);
  EXPECT_EQ(static_cast<T>(0.38), quaternion.vector()[1]);
  EXPECT_EQ(static_cast<T>(0.19), quaternion.vector()[2]);
}
TEST_ALL_F(ConstVectorAccessor)

// Test mutating the vector component of the quaternion using the vector
// mutator.
template <class T>
void VectorMutator_Test(const T& precision) {
  (void)precision;
  mathfu::Quaternion<T> quaternion;
  quaternion.set_vector(mathfu::Vector<T, 3>(
      static_cast<T>(0.38), static_cast<T>(0.76), static_cast<T>(0.50)));
  EXPECT_EQ(static_cast<T>(0.38), quaternion.vector()[0]);
  EXPECT_EQ(static_cast<T>(0.76), quaternion.vector()[1]);
  EXPECT_EQ(static_cast<T>(0.50), quaternion.vector()[2]);
}
TEST_ALL_F(VectorMutator)

// This will test converting a Quaternion to and from Angle/Axis,
// Euler Angles, and Matrices
template <class T>
void Conversion_Test(const T& precision) {
  mathfu::Vector<T, 3> angles(static_cast<T>(1.5), static_cast<T>(2.3),
                              static_cast<T>(0.6));
  // This will create a Quaternion from Euler Angles, convert back to
  // Euler Angles, and verify that they match
  mathfu::Quaternion<T> qea(mathfu::Quaternion<T>::FromEulerAngles(angles));
  mathfu::Vector<T, 3> convertedAngles(qea.ToEulerAngles());
  EXPECT_NEAR(angles[0], M_PI + convertedAngles[0], precision);
  EXPECT_NEAR(angles[1], M_PI - convertedAngles[1], precision);
  EXPECT_NEAR(angles[2], M_PI + convertedAngles[2], precision);
  // This will create a Quaternion from Axis Angle, convert back to
  // Axis Angle, and verify that they match.
  mathfu::Vector<T, 3> axis(static_cast<T>(4.3), static_cast<T>(7.6),
                            static_cast<T>(1.2));
  axis.Normalize();
  T angle = static_cast<T>(1.2);
  mathfu::Quaternion<T> qaa(mathfu::Quaternion<T>::FromAngleAxis(angle, axis));
  mathfu::Vector<T, 3> convertedAxis;
  T convertedAngle;
  qaa.ToAngleAxis(&convertedAngle, &convertedAxis);
  EXPECT_NEAR(angle, convertedAngle, precision);
  EXPECT_NEAR(axis[0], convertedAxis[0], precision);
  EXPECT_NEAR(axis[1], convertedAxis[1], precision);
  EXPECT_NEAR(axis[2], convertedAxis[2], precision);
  // This will create a Quaternion from a 3x3 Matrix, convert back to a Matrix,
  // and verify that they match.
  mathfu::Matrix<T, 3> rx(1, 0, 0, 0, cos(angles[0]), sin(angles[0]), 0,
                          -sin(angles[0]), cos(angles[0]));
  mathfu::Matrix<T, 3> ry(cos(angles[1]), 0, -sin(angles[1]), 0, 1, 0,
                          sin(angles[1]), 0, cos(angles[1]));
  mathfu::Matrix<T, 3> rz(cos(angles[2]), sin(angles[2]), 0, -sin(angles[2]),
                          cos(angles[2]), 0, 0, 0, 1);
  mathfu::Matrix<T, 3> m(rz * ry * rx);
  mathfu::Quaternion<T> qm(mathfu::Quaternion<T>::FromMatrix(m));
  mathfu::Matrix<T, 3> convertedM(qm.ToMatrix());
  for (int i = 0; i < 9; ++i) EXPECT_NEAR(m[i], convertedM[i], precision);
  // This will create a Quaternion from a 4x4 Matrix, convert back to a Matrix,
  // and verify that they match.
  // Recycling the 3x3 matrix from before.
  mathfu::Matrix<T, 4> m4 = mathfu::Matrix<T, 4>::FromRotationMatrix(m);
  mathfu::Quaternion<T> qm4(mathfu::Quaternion<T>::FromMatrix(m4));
  mathfu::Matrix<T, 4> convertedM4(qm4.ToMatrix4());
  for (int i = 0; i < 15; ++i) EXPECT_NEAR(m4[i], convertedM4[i], precision);
}
TEST_ALL_F(Conversion)

// This will test inverting a quaternion and verify that their combination
// corresponds to a rotation of 0.
template <class T>
void Inverse_Test(const T& precision) {
  mathfu::Quaternion<T> q(static_cast<T>(1.4), static_cast<T>(6.3),
                          static_cast<T>(8.5), static_cast<T>(5.9));
  mathfu::Vector<T, 3> v((q.Inverse() * q).ToEulerAngles());
  EXPECT_NEAR(0, v[0], precision);
  EXPECT_NEAR(0, v[1], precision);
  EXPECT_NEAR(0, v[2], precision);
}
TEST_ALL_F(Inverse)

// This will test the multiplication of quaternions.
template <class T>
void Mult_Test(const T& precision) {
  mathfu::Vector<T, 3> axis(static_cast<T>(4.3), static_cast<T>(7.6),
                            static_cast<T>(1.2));
  axis.Normalize();
  T angle1 = static_cast<T>(1.2), angle2 = static_cast<T>(0.7),
    angle3 = angle2 + precision * 10;
  mathfu::Quaternion<T> qaa1(
      mathfu::Quaternion<T>::FromAngleAxis(angle1, axis));
  mathfu::Quaternion<T> qaa2(
      mathfu::Quaternion<T>::FromAngleAxis(angle2, axis));
  mathfu::Quaternion<T> qaa3(
      mathfu::Quaternion<T>::FromAngleAxis(angle3, axis));
  mathfu::Vector<T, 3> convertedAxis;
  T convertedAngle;
  // This will verify that multiplying two quaternions corresponds to the sum
  // of the rotations.
  (qaa1 * qaa2).ToAngleAxis(&convertedAngle, &convertedAxis);
  EXPECT_NEAR(angle1 + angle2, convertedAngle, precision);
  // This will verify that multiplying a quaternion with a scalar corresponds
  // to scaling the rotation.
  (qaa1 * 2).ToAngleAxis(&convertedAngle, &convertedAxis);
  EXPECT_NEAR(angle1 * 2, convertedAngle, precision);
  mathfu::Vector<T, 3> v(3.5f, 6.4f, 7.0f);
  mathfu::Vector<T, 4> v4(3.5f, 6.4f, 7.0f, 0.0f);
  // This will verify that multiplying by a vector corresponds to applying
  // the rotation to that vector.
  mathfu::Vector<T, 3> quatRotatedV(qaa1 * v);
  mathfu::Vector<T, 3> matRotatedV(qaa1.ToMatrix() * v);
  mathfu::Vector<T, 4> mat4RotatedV(qaa1.ToMatrix4() * v4);
  EXPECT_NEAR(quatRotatedV[0], matRotatedV[0], 10 * precision);
  EXPECT_NEAR(quatRotatedV[1], matRotatedV[1], 10 * precision);
  EXPECT_NEAR(quatRotatedV[2], matRotatedV[2], 10 * precision);

  EXPECT_NEAR(quatRotatedV[0], mat4RotatedV[0], 10 * precision);
  EXPECT_NEAR(quatRotatedV[1], mat4RotatedV[1], 10 * precision);
  EXPECT_NEAR(quatRotatedV[2], mat4RotatedV[2], 10 * precision);
  // This will verify that interpolating two quaternions corresponds to
  // interpolating the angle.
  mathfu::Quaternion<T> slerp1(mathfu::Quaternion<T>::Slerp(qaa1, qaa2, 0.5));
  slerp1.ToAngleAxis(&convertedAngle, &convertedAxis);
  EXPECT_NEAR(.5 * (angle1 + angle2), convertedAngle, precision);
  mathfu::Quaternion<T> slerp2(mathfu::Quaternion<T>::Slerp(qaa2, qaa3, 0.5));
  slerp2.ToAngleAxis(&convertedAngle, &convertedAxis);
  EXPECT_NEAR(.5 * (angle2 + angle3), convertedAngle, precision);
  mathfu::Quaternion<T> slerp3(mathfu::Quaternion<T>::Slerp(qaa2, qaa2, 0.5));
  slerp3.ToAngleAxis(&convertedAngle, &convertedAxis);
  EXPECT_NEAR(angle2, convertedAngle, precision);
}
TEST_ALL_F(Mult)

// This tests that quat * float changes the direction of the quat to keep it
// in the "small" hemisphere, before doing the multiplication.  This makes
// scalar factors < 1 act intuitively, at the cost of sometimes making
// multiplication non-associative for scale factors > 1.
template <class T>
void MultQuatFloatFlipsQuat_Test(const T& precision) {
  using Quaternion = mathfu::Quaternion<T>;
  using Vector3 = mathfu::Vector<T, 3>;
  const Vector3 up(0, 1, 0);
  const double epsilon = 1e-5;

  // Test the flipping behavior directly.
  const Quaternion bigQuat = Quaternion::FromAngleAxis(
      static_cast<T>(mathfu::kPi * 1.5), up);
  EXPECT_NEAR_QUAT(Quaternion(-bigQuat.scalar(), -bigQuat.vector()),
                   bigQuat * 1,
                   epsilon);

  // Test the claim made in the documentation:
  // "For example, you are not guaranteed that (q * 2) * .5 and q * (2 * .5)
  //  are the same orientation, let alone the same quaternion."
  const Quaternion base = Quaternion::FromAngleAxis(
      static_cast<T>(mathfu::kPi * .75), up);
  const Quaternion q1 = (base * 2) * .5f;
  const Quaternion q2 = base * (2 * .5f);
  EXPECT_NEAR_QUAT(
      q1,
      Quaternion::FromAngleAxis(static_cast<T>(mathfu::kPi * -.25), up),
      epsilon);
  EXPECT_NEAR_QUAT(q2, base, epsilon);
  EXPECT_FALSE(IsNearOrientation(q1, q2, epsilon));
  EXPECT_FALSE(IsNearQuat(q1, q2, epsilon));
}
TEST_ALL_F(MultQuatFloatFlipsQuat)

// This will test the dot product of quaternions.
template <class T>
void Dot_Test(const T& precision) {
  mathfu::Vector<T, 3> axis(static_cast<T>(4.3), static_cast<T>(7.6),
                            static_cast<T>(1.2));
  axis.Normalize();
  T angle1 = static_cast<T>(1.2), angle2 = static_cast<T>(angle1 + M_PI / 2.0),
    angle3 = static_cast<T>(angle1 + M_PI), angle4 = static_cast<T>(0.7);
  mathfu::Quaternion<T> qaa1(
      mathfu::Quaternion<T>::FromAngleAxis(angle1, axis));
  mathfu::Quaternion<T> qaa2(
      mathfu::Quaternion<T>::FromAngleAxis(angle2, axis));
  mathfu::Quaternion<T> qaa3(
      mathfu::Quaternion<T>::FromAngleAxis(angle3, axis));
  mathfu::Quaternion<T> qaa4(
      mathfu::Quaternion<T>::FromAngleAxis(angle4, axis));

  // This will verify that Dotting two quaternions works correctly.
  EXPECT_NEAR(mathfu::Quaternion<T>::DotProduct(qaa1, qaa1), 1.0, precision);
  EXPECT_NEAR(mathfu::Quaternion<T>::DotProduct(qaa1, qaa2), sqrt(2.0) / 2.0,
              precision);
  EXPECT_NEAR(mathfu::Quaternion<T>::DotProduct(qaa1, qaa3), 0.0, precision);
  // 2 x acos(dot) should be the angle between two quaternions:
  EXPECT_NEAR(acos(mathfu::Quaternion<T>::DotProduct(qaa1, qaa4)) * 2.0,
              angle1 - angle4, precision);
}
TEST_ALL_F(Dot)

// This will test normalization of quaternions.
template <class T>
void Normalize_Test(const T& precision) {
  mathfu::Quaternion<T> quat_1(static_cast<T>(12), static_cast<T>(0),
                               static_cast<T>(0), static_cast<T>(0));
  const mathfu::Quaternion<T> const_quat_1 = quat_1;
  const mathfu::Quaternion<T> normalized_quat_1 = const_quat_1.Normalized();
  quat_1.Normalize();
  mathfu::Quaternion<T> reference_quat_1(static_cast<T>(1), static_cast<T>(0),
                                         static_cast<T>(0), static_cast<T>(0));
  EXPECT_NEAR(reference_quat_1[0], quat_1[0], precision);
  EXPECT_NEAR(reference_quat_1[1], quat_1[1], precision);
  EXPECT_NEAR(reference_quat_1[2], quat_1[2], precision);
  EXPECT_NEAR(reference_quat_1[3], quat_1[3], precision);
  EXPECT_NEAR(reference_quat_1[0], normalized_quat_1[0], precision);
  EXPECT_NEAR(reference_quat_1[1], normalized_quat_1[1], precision);
  EXPECT_NEAR(reference_quat_1[2], normalized_quat_1[2], precision);
  EXPECT_NEAR(reference_quat_1[3], normalized_quat_1[3], precision);

  mathfu::Quaternion<T> quat_2(static_cast<T>(123), static_cast<T>(123),
                               static_cast<T>(123), static_cast<T>(123));
  mathfu::Quaternion<T> normalized_quat_2 = quat_2.Normalized();
  quat_2.Normalize();
  mathfu::Quaternion<T> reference_quat_2(
      static_cast<T>(sqrt(.25)), static_cast<T>(sqrt(.25)),
      static_cast<T>(sqrt(.25)), static_cast<T>(sqrt(.25)));
  EXPECT_NEAR(reference_quat_2[0], quat_2[0], precision);
  EXPECT_NEAR(reference_quat_2[1], quat_2[1], precision);
  EXPECT_NEAR(reference_quat_2[2], quat_2[2], precision);
  EXPECT_NEAR(reference_quat_2[3], quat_2[3], precision);
  EXPECT_NEAR(reference_quat_2[0], normalized_quat_2[0], precision);
  EXPECT_NEAR(reference_quat_2[1], normalized_quat_2[1], precision);
  EXPECT_NEAR(reference_quat_2[2], normalized_quat_2[2], precision);
  EXPECT_NEAR(reference_quat_2[3], normalized_quat_2[3], precision);
}
TEST_ALL_F(Normalize)

// This tests that ToAngleAxis returns angle <= 180 degrees, even if the
// Quaternion had a larger angle.
template <class T>
void ToAngleAxisReturnsSmallQuat_Test(const T& precision) {
  using Quaternion = mathfu::Quaternion<T>;
  using Vector3 = mathfu::Vector<T, 3>;
  const float epsilon = 1e-5f;

  // Test the specific example called out in the documentation:
  // "For example, if *this represents "Rotate 350 degrees left", you will
  //  get the angle-axis "Rotate 10 degrees right"."
  const Vector3 kUp(0, 1, 0);
  const float k350Degrees = 350 * mathfu::kDegreesToRadians;
  const Quaternion k350Left = Quaternion::FromAngleAxis(k350Degrees, kUp);

  const Vector3 kDown(0, -1, 0);
  const float k10Degrees = 10 * mathfu::kDegreesToRadians;
  const Quaternion k10Right = Quaternion::FromAngleAxis(k10Degrees, kDown);

  {
    T angle;
    Vector3 axis;
    k350Left.ToAngleAxis(&angle, &axis);
    EXPECT_NEAR(k10Degrees, angle, epsilon);
    EXPECT_NEAR_VEC3(kDown, axis, epsilon);
    EXPECT_NEAR_QUAT(k10Right, Quaternion::FromAngleAxis(angle, axis), epsilon);
  }
  {
    T angle;
    Vector3 axis;
    k350Left.ToAngleAxisFull(&angle, &axis);
    EXPECT_NEAR(k350Degrees, angle, epsilon);
    EXPECT_NEAR_VEC3(kUp, axis, epsilon);
    EXPECT_NEAR_QUAT(k350Left, Quaternion::FromAngleAxis(angle, axis), epsilon);
  }
}
TEST_ALL_F(ToAngleAxisReturnsSmallQuat)

// This will test normalization of quaternions.
template <class T>
void RotateFromTo_Test(const T& precision) {
  mathfu::Vector<T, 3> x_axis = mathfu::Vector<T, 3>(
      static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
  mathfu::Vector<T, 3> y_axis = mathfu::Vector<T, 3>(
      static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
  mathfu::Vector<T, 3> z_axis = mathfu::Vector<T, 3>(
      static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));

  mathfu::Quaternion<T> x_to_y =
      mathfu::Quaternion<T>::RotateFromTo(x_axis, y_axis);
  mathfu::Quaternion<T> y_to_z =
      mathfu::Quaternion<T>::RotateFromTo(y_axis, z_axis);
  mathfu::Quaternion<T> z_to_x =
      mathfu::Quaternion<T>::RotateFromTo(z_axis, x_axis);

  // Check some axis rotations:
  // By definition, rotateFromTo(v1, v2) * v2 should always equal v2.
  // if v1 and v2 are 90 degrees apart (as they are in the case of axes)
  // then applying the same rotation twice should invert the vector.
  mathfu::Vector<T, 3> x_to_y_result = x_to_y * x_axis;
  mathfu::Vector<T, 3> x_to_y_twice_result = x_to_y * x_to_y * x_axis;
  EXPECT_NEAR_VEC3(x_to_y_result, y_axis, precision);
  EXPECT_NEAR_VEC3(x_to_y_twice_result, -x_axis, precision);

  mathfu::Vector<T, 3> y_to_z_result = y_to_z * y_axis;
  mathfu::Vector<T, 3> y_to_z_twice_result = y_to_z * y_to_z * y_axis;
  EXPECT_NEAR_VEC3(y_to_z_result, z_axis, precision);
  EXPECT_NEAR_VEC3(y_to_z_twice_result, -y_axis, precision);

  mathfu::Vector<T, 3> z_to_x_result = z_to_x * z_axis;
  mathfu::Vector<T, 3> z_to_x_twice_result = z_to_x * z_to_x * z_axis;
  EXPECT_NEAR_VEC3(z_to_x_result, x_axis, precision);
  EXPECT_NEAR_VEC3(z_to_x_twice_result, -z_axis, precision);

  // Try some weirder vectors:
  mathfu::Vector<T, 3> arbitrary_1 = mathfu::Vector<T, 3>(
      static_cast<T>(2), static_cast<T>(-5), static_cast<T>(9));
  mathfu::Vector<T, 3> arbitrary_2 = mathfu::Vector<T, 3>(
      static_cast<T>(-1), static_cast<T>(3), static_cast<T>(16));

  mathfu::Quaternion<T> arbitrary_to_arbitrary =
      mathfu::Quaternion<T>::RotateFromTo(arbitrary_1, arbitrary_2);

  mathfu::Vector<T, 3> arbitrary_1_to_2 = arbitrary_to_arbitrary * arbitrary_1;
  arbitrary_1_to_2.Normalize();
  mathfu::Vector<T, 3> arbitrary_2_normalized = arbitrary_2.Normalized();

  EXPECT_NEAR_VEC3(arbitrary_1_to_2, arbitrary_2_normalized, precision);

  // Using RotateFromTo on one vector should give us the identity quaternion:
  mathfu::Quaternion<T> identity =
      mathfu::Quaternion<T>::RotateFromTo(arbitrary_1, arbitrary_1);

  mathfu::Vector<T, 3> arbitrary_2_identity = identity * arbitrary_2;
  EXPECT_NEAR_VEC3(arbitrary_2_identity, arbitrary_2, precision);

  // Using RotateFromTo on an inverted vector should give a 180 degree rotation:
  mathfu::Quaternion<T> reverse =
      mathfu::Quaternion<T>::RotateFromTo(arbitrary_1, -arbitrary_1);

  // Relaxing the precision slightly, because there are a lot of chained
  // float operations in here.
  mathfu::Vector<T, 3> arbitrary_1_reversed = reverse * arbitrary_1;
  EXPECT_NEAR_VEC3(arbitrary_1_reversed, -arbitrary_1, precision * 2.0);
}
TEST_ALL_F(RotateFromTo)

// Test the compilation of basic quaternion operations given in the sample
// file. This will test interpolating two rotations.
TEST_F(QuaternionTests, QuaternionSample) {
  using namespace mathfu;
  /// @doxysnippetstart Chapter03_Quaternions.md Quaternion_Sample
  // Use radians for angles
  Vector<float, 3> angles1(0.66f, 1.3f, 0.76f);
  Vector<float, 3> angles2(0.85f, 0.33f, 1.6f);

  Quaternion<float> quat1 = Quaternion<float>::FromEulerAngles(angles1);
  Quaternion<float> quat2 = Quaternion<float>::FromEulerAngles(angles2);

  Quaternion<float> quatSlerp = Quaternion<float>::Slerp(quat1, quat2, 0.5);
  Vector<float, 3> angleSlerp = quatSlerp.ToEulerAngles();
  /// @doxysnippetend
  const float precision = 1e-2f;
  EXPECT_NEAR(0.93f, angleSlerp[0], precision);
  EXPECT_NEAR(0.82f, angleSlerp[1], precision);
  EXPECT_NEAR(1.33f, angleSlerp[2], precision);
}

// Test that the quaternion identity constants give the identity transform.
TEST_F(QuaternionTests, IdentityConst) {
  EXPECT_EQ_QUAT(mathfu::kQuatIdentityf, mathfu::Quaternion<float>::identity);
  EXPECT_EQ_QUAT(mathfu::kQuatIdentityf,
                 mathfu::Quaternion<float>(1.0f, 0.0f, 0.0f, 0.0f));
  EXPECT_EQ(mathfu::kQuatIdentityf.ToEulerAngles(), mathfu::kZeros3f);

  EXPECT_EQ_QUAT(mathfu::kQuatIdentityd, mathfu::Quaternion<double>::identity);
  EXPECT_EQ_QUAT(mathfu::kQuatIdentityd,
                 mathfu::Quaternion<double>(1.0, 0.0, 0.0, 0.0));
  EXPECT_EQ(mathfu::kQuatIdentityd.ToEulerAngles(), mathfu::kZeros3d);
}

template <class T>
void OutputStream_Test(const T&) {
  mathfu::Quaternion<T> q =
      mathfu::Quaternion<T>(static_cast<T>(1), static_cast<T>(2),
                            static_cast<T>(3), static_cast<T>(4));
  std::stringstream ss;
  ss << q;
  EXPECT_EQ("(1, 2, 3, 4)", ss.str());
}
TEST_ALL_F(OutputStream)

template <class T>
void LookAt_Test(const T& precision) {
  const T one = static_cast<T>(1);
  const T zero = static_cast<T>(0);
  EXPECT_NEAR_QUAT(mathfu::Quaternion<T>::identity,
                   mathfu::Quaternion<T>::LookAt(
                       mathfu::Vector<T, 3>(zero, zero, one),
                       mathfu::Vector<T, 3>(zero, one, zero)),
                   precision);
}
TEST_ALL_F(LookAt)

template <class T>
void FromEulerAnglesSplit_Test(const T& precision) {
  mathfu::Vector<T, 3> eulers(static_cast<T>(0.1), static_cast<T>(0.2),
                              static_cast<T>(0.3));
  EXPECT_NEAR_QUAT(
      mathfu::Quaternion<T>::FromEulerAngles(eulers),
      mathfu::Quaternion<T>::FromEulerAngles(eulers[0], eulers[1], eulers[2]),
      precision);
}
TEST_ALL_F(FromEulerAnglesSplit)

const float kSlerpTestAnglesInDegrees[] {
  // Slerp algorithms commonly have trouble with angles near zero.
  // To give a sense of what that means for common quaternion-dot cutoffs:
  // - Quaternion dot of .99999 = .512 degrees
  // - Quaternion dot of .9999 = 1.62 degrees
  // - Quaternion dot of .9995 = 3.62 degrees
  0, .5f, 1.5f, 3.5f,
  80,
  // 180 has no numerical problems, unless there's a bug. But worth checking.
  179, 180, 181,
  // Slerp is ill-defined at angles near 360.
  359, 359.5f, 360, 360.5f, 361,
};

// Tests that Slerp returns unit-length quaternions.
template <class T>
void SlerpResultIsUnit_Test(const T& precision) {
  using Quaternion = mathfu::Quaternion<T>;

  const mathfu::Vector<T, 3> axis(0, 1, 0);
  const float kLengthEpsilon = 5e-6f;

  for (float angle : kSlerpTestAnglesInDegrees) {
    const Quaternion q2 = Quaternion::FromAngleAxis(
        angle * mathfu::kDegreesToRadians, axis);

    Quaternion slerp_result = Quaternion::Slerp(Quaternion::identity, q2, .5f);
    const T slerp_length = slerp_result.Normalize();
    EXPECT_NEAR(1.0f, slerp_length, kLengthEpsilon) << " for angle " << angle;

    // Alternate spelling for Slerp
    Quaternion mul_result = q2 * .5f;
    const T mul_length = mul_result.Normalize();
    EXPECT_NEAR(1.0f, mul_length, kLengthEpsilon) << " for angle " << angle;
  }
}
TEST_ALL_F(SlerpResultIsUnit)

// Checks equality of
// - quat(<some axis>, expected_angle) vs
// - Slerp(identity, quat(<some axis>, angle), t) vs
// - Slerp(quat(<some axis>, angle), identity, 1-t)
// Angles are in degrees.
template <class T>
void CheckSlerp(float angle, float t, float expected_angle) {
  using Quaternion = mathfu::Quaternion<T>;
  using Vector3 = mathfu::Vector<T, 3>;

  // Transcendentals are involved, so be lenient on the epsilon.
  const T epsilon = 1e-6f;
  const Vector3 up(0, 1, 0);  // Could be any axis, really.
  const Quaternion original =
      Quaternion::FromAngleAxis(angle * mathfu::kDegreesToRadians, up);
  const Quaternion expected =
      Quaternion::FromAngleAxis(expected_angle * mathfu::kDegreesToRadians, up);

  // These are looser EXPECT_NEAR_ORIENTATION checks because Slerp() treats
  // quats as orientations. For checking a mathematical Slerp(), they can
  // (and should) be tightened back to EXPECT_NEAR_QUAT.

  Quaternion slerp_result =
      Quaternion::Slerp(Quaternion::identity, original, t);
  EXPECT_NEAR_ORIENTATION(expected, slerp_result, epsilon)
      << " for angle " << angle << " and t " << t;

  // Apply the invariant that slerp(a, b, t) == slerp(b, a, 1-t).
  Quaternion slerp_backwards_result =
      Quaternion::Slerp(original, Quaternion::identity, 1 - t);
  EXPECT_NEAR_ORIENTATION(expected, slerp_backwards_result, epsilon)
      << " for angle " << angle << " and t " << t;

  Quaternion mul_result = original * t;
  EXPECT_NEAR_ORIENTATION(expected, mul_result, epsilon)
      << " for angle " << angle << " and t " << t;
}

// This doubles as a test of both Slerp() and operator*(quat, float),
// since the two are pretty much the same operation with different spelling.
template <class T>
void Slerp_Test(const T& precision) {
  // Easy and unambiguous cases.
  CheckSlerp<T>(+160, 0.375f, +60);
  CheckSlerp<T>(-160, 0.375f, -60);

  // Shortening a "long way around" (> 180 degree) rotation
  // NOTE: These results are different from the mathematical quat slerp
  CheckSlerp<T>(+320, 0.375f, -15);  // Mathematically, should be +120
  CheckSlerp<T>(-320, 0.375f, +15);  // Mathematically, should be -120

  // Lengthening a "long way around" rotation
  CheckSlerp<T>(320, 1.5f, -60);  // Mathematically, should be 480 (ie -240)

  // Lengthening to a "long way around" (> 180 degree) rotation
  CheckSlerp<T>(+70, 3, +210);
  CheckSlerp<T>(-70, 3, -210);

  // An edge case that often causes NaNs
  CheckSlerp<T>(0, .5f, 0);

  // This edge case is ill-defined for "intuitive" slerp and can't be tested.
  // CheckSlerp<T>(180, .25f, 45);

  // Conversely, this edge case is well-defined for "intuitive" slerp.
  // For mathematical slerp, the axis is ill-defined and can take many values.
  CheckSlerp<T>(360, .25f, 0);
}
TEST_ALL_F(Slerp)

}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  printf("%s (%s)\n", argv[0], MATHFU_BUILD_OPTIONS_STRING);
  return RUN_ALL_TESTS();
}
