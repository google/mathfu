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
#include <math.h>

#include "vectors/Quaternion.h"
#include "gtest/gtest.h"

#include "AndroidUtil/AndroidMainWrapper.h"
#include "Precision.h"

class QuaternionTests : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

// This will automatically generate tests for each template parameter.
#define TEST_ALL_F(MY_TEST) \
  TEST_F(QuaternionTests, MY_TEST) { \
    MY_TEST##_Test<float>(FLOAT_PRECISION); \
    MY_TEST##_Test<double>(DOUBLE_PRECISION); \
  }

// This will test converting a Quaternion to and from Angle/Axis,
// Euler Angles, and Matrices
template<class T>
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
  // This will create a Quaternion from a Matrix, convert back to a Matrix,
  // and verify that they match.
  mathfu::Matrix<T, 3> rx(
    1, 0, 0, 0, cos(angles[0]), sin(angles[0]),
    0, -sin(angles[0]), cos(angles[0]));
  mathfu::Matrix<T, 3> ry
    (cos(angles[1]), 0, -sin(angles[1]), 0, 1, 0,
    sin(angles[1]), 0, cos(angles[1]));
  mathfu::Matrix<T, 3> rz(
    cos(angles[2]), sin(angles[2]), 0,
    -sin(angles[2]), cos(angles[2]), 0, 0, 0, 1);
  mathfu::Matrix<T, 3> m(rz * ry * rx);
  mathfu::Quaternion<T> qm(mathfu::Quaternion<T>::FromMatrix(m));
  mathfu::Matrix<T, 3> convertedM(qm.ToMatrix());
  for (int i = 0; i < 9; ++i) EXPECT_NEAR(m[i], convertedM[i], precision);
}
TEST_ALL_F(Conversion);

// This will test inverting a quaternion and verify that their combination
// corresponds to a rotation of 0.
template<class T>
void Inverse_Test(const T& precision) {
  mathfu::Quaternion<T> q(static_cast<T>(1.4), static_cast<T>(6.3),
                          static_cast<T>(8.5), static_cast<T>(5.9));
  mathfu::Vector<T, 3> v((q.Inverse()*q).ToEulerAngles());
  EXPECT_NEAR(0, v[0], precision);
  EXPECT_NEAR(0, v[1], precision);
  EXPECT_NEAR(0, v[2], precision);
}
TEST_ALL_F(Inverse);

// This will test the multiplcation of quaternions.
template<class T>
void Mult_Test(const T& precision) {
  mathfu::Vector<T, 3> axis(static_cast<T>(4.3), static_cast<T>(7.6),
                            static_cast<T>(1.2));
  axis.Normalize();
  T angle1 = static_cast<T>(1.2), angle2 = static_cast<T>(0.7),
    angle3 = angle2 + precision * 10;
  mathfu::Quaternion<T> qaa1(mathfu::Quaternion<T>::FromAngleAxis(angle1,
                                                                  axis));
  mathfu::Quaternion<T> qaa2(mathfu::Quaternion<T>::FromAngleAxis(angle2,
                                                                  axis));
  mathfu::Quaternion<T> qaa3(mathfu::Quaternion<T>::FromAngleAxis(angle3,
                                                                  axis));
  mathfu::Vector<T, 3> convertedAxis;
  T convertedAngle;
  // This will verify that mutliplying two quaternions corresponds to the sum
  // of the rotations.
  (qaa1 * qaa2).ToAngleAxis(&convertedAngle, &convertedAxis);
  EXPECT_NEAR(angle1 + angle2, convertedAngle, precision);
  // This will verify that mutliplying a quaternions with a scalar corresponds
  // to scalaing the rotation.
  (qaa1 * 2).ToAngleAxis(&convertedAngle, &convertedAxis);
  EXPECT_NEAR(angle1 * 2, convertedAngle, precision);
  mathfu::Vector<T, 3> v(3.5f, 6.4f, 7.0f);
  // This will verify that multiplying by a vector corresponds to applying
  // the rotation to that vector.
  mathfu::Vector<T, 3> quatRotatedV(qaa1 * v);
  mathfu::Vector<T, 3> matRotatedV(qaa1.ToMatrix()*v);
  EXPECT_NEAR(quatRotatedV[0], matRotatedV[0], 10 * precision);
  EXPECT_NEAR(quatRotatedV[1], matRotatedV[1], 10 * precision);
  EXPECT_NEAR(quatRotatedV[2], matRotatedV[2], 10 * precision);
  // This will verify that interpolating two quaternions corresponds to
  // interpolating the angle.
  mathfu::Quaternion<T> slerp1(mathfu::Quaternion<T>::Slerp(qaa1, qaa2, 0.5));
  slerp1.ToAngleAxis(&convertedAngle, &convertedAxis);
  EXPECT_NEAR(.5*(angle1 + angle2), convertedAngle, precision);
  mathfu::Quaternion<T> slerp2(mathfu::Quaternion<T>::Slerp(qaa2, qaa3, 0.5));
  slerp2.ToAngleAxis(&convertedAngle, &convertedAxis);
  EXPECT_NEAR(.5*(angle2 + angle3), convertedAngle, precision);
  mathfu::Quaternion<T> slerp3(mathfu::Quaternion<T>::Slerp(qaa2, qaa2, 0.5));
  slerp3.ToAngleAxis(&convertedAngle, &convertedAxis);
  EXPECT_NEAR(angle2, convertedAngle, precision);

}
TEST_ALL_F(Mult);

// Test the compilation of basic quaternion opertations given in the sample
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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
