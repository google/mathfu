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
#include "mathfu/vector.h"
#include "mathfu/vector_2.h"
#include "mathfu/vector_3.h"
#include "mathfu/vector_4.h"
#include "mathfu/constants.h"

#include "gtest/gtest.h"

#include "precision.h"

class VectorTests : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

// This will automatically generate tests for each template parameter.
#define TEST_ALL_F(MY_TEST) \
  TEST_F(VectorTests, MY_TEST) { \
    MY_TEST##_Test<float, 2>(FLOAT_PRECISION); \
    MY_TEST##_Test<double, 2>(DOUBLE_PRECISION); \
    MY_TEST##_Test<float, 3>(FLOAT_PRECISION); \
    MY_TEST##_Test<double, 3>(DOUBLE_PRECISION); \
    MY_TEST##_Test<float, 4>(FLOAT_PRECISION); \
    MY_TEST##_Test<double, 4>(DOUBLE_PRECISION); \
    MY_TEST##_Test<float, 5>(FLOAT_PRECISION); \
    MY_TEST##_Test<double, 5>(DOUBLE_PRECISION); \
  }

// This will automatically generate tests for each scalar template parameter.
#define TEST_SCALAR_F(MY_TEST) \
  TEST_F(VectorTests, MY_TEST) { \
    MY_TEST##_Test<float>(FLOAT_PRECISION); \
    MY_TEST##_Test<double>(DOUBLE_PRECISION); \
  }

// Tests float, double, and integer constants in one line.
#define VECTOR_TEST_CONSTANT_EQ(kConst, index, value) \
  EXPECT_FLOAT_EQ(mathfu::kConst##f[(index)], static_cast<float>(value)); \
  EXPECT_DOUBLE_EQ(mathfu::kConst##d[(index)], static_cast<double>(value)); \
  EXPECT_EQ(mathfu::kConst##i[(index)], static_cast<int>(value))


// This will test initializaiton by passing in values. The template paramter d
// corresponds to the size of the vector.
template<class T, int d>
void Initialization_Test(const T& precision) {
  // This will test initialization of the vector using a random single value.
  // The expected result is that all entries equal the given value.
  mathfu::Vector<T, d> vector_splat(static_cast<T>(3.1));
  for (int i = 0; i < d; ++i) {
    EXPECT_NEAR(3.1, vector_splat[i], precision);
  }
  T x[d];
  for (int i = 0; i < d; ++i) {
    x[i] = rand() / static_cast<T>(RAND_MAX) * 100.f;
  }
  // This will test initialization of the vector using a c style array of
  // values.
  mathfu::Vector<T, d> vector_arr(x);
  for (int i = 0; i < d; ++i) {
    EXPECT_NEAR(x[i], vector_arr[i], precision);
  }
  // This will test copy constructor making sure that the new matrix equals
  // the old one.
  mathfu::Vector<T, d> vector_copy(vector_arr);
  for (int i = 0; i < d; ++i) {
    EXPECT_NEAR(x[i], vector_copy[i], precision);
  }
  // This will make sure the copy was deep and chaning the values of the
  // copied matrix does not effect the origional.
  vector_copy -= mathfu::Vector<T, d>(1);
  EXPECT_NE(vector_copy[0], vector_arr[0]);
}
TEST_ALL_F(Initialization)

// This will test initializaiton by specifying all values explictly.
template<class T>
void InitializationPerDimension_Test(const T& precision) {
  mathfu::Vector<T, 2> f2_vector(static_cast<T>(5.3), static_cast<T>(7.1));
  EXPECT_NEAR(5.3, f2_vector[0], precision);
  EXPECT_NEAR(7.1, f2_vector[1], precision);
  mathfu::Vector<T, 3> f3_vector(static_cast<T>(4.3), static_cast<T>(1.1),
                                 static_cast<T>(3.2));
  EXPECT_NEAR(4.3, f3_vector[0], precision);
  EXPECT_NEAR(1.1, f3_vector[1], precision);
  EXPECT_NEAR(3.2, f3_vector[2], precision);
  mathfu::Vector<T, 4> f4_vector(static_cast<T>(2.3), static_cast<T>(4.6),
                                 static_cast<T>(9.2), static_cast<T>(15.5));
  EXPECT_NEAR(2.3, f4_vector[0], precision);
  EXPECT_NEAR(4.6, f4_vector[1], precision);
  EXPECT_NEAR(9.2, f4_vector[2], precision);
  EXPECT_NEAR(15.5, f4_vector[3], precision);
}
TEST_SCALAR_F(InitializationPerDimension);

// This will test the Addition and Subtraction of vectors. The template
// parameter d corresponds to the size of the vector.
template<class T, int d>
void AddSub_Test(const T& precision) {
  T x1[d], x2[d];
  for (int i = 0; i < d; ++i) {
    x1[i] = rand() / static_cast<T>(RAND_MAX) * 100.f;
  }
  for (int i = 0; i < d; ++i) {
    x2[i] = rand() / static_cast<T>(RAND_MAX) * 100.f;
  }
  mathfu::Vector<T, d> vector1(x1), vector2(x2);
  // This will test the negation of a vector and make sure each element is
  // negated.
  mathfu::Vector<T, d> neg_vector1(-vector1);
  for (int i = 0; i < d; ++i) {
    EXPECT_NEAR(-x1[i], neg_vector1[i], precision);
  }
  // This will test the addition of vectors and make such each element is
  // equal to the sum of the input values.
  mathfu::Vector<T, d> sum_vector(vector1 + vector2);
  for (int i = 0; i < d; ++i) {
    EXPECT_NEAR(x1[i] + x2[i], sum_vector[i], precision);
  }
  // This will test the subtraction of vectors and make such each element is
  // equal to the difference of the input values.
  mathfu::Vector<T, d> diff_vector(vector1 - vector2);
  for (int i = 0; i < d; ++i) {
    EXPECT_NEAR(x1[i] - x2[i], diff_vector[i], precision);
  }
}
TEST_ALL_F(AddSub)

// This will test the mutiplication of vectors by vectors and scalars. The
// template paramter d corresponds to the size of the vector.
template<class T, int d>
void Mult_Test(const T& precision) {
  T x1[d], x2[d], scalar(static_cast<T>(1.4));
  for (int i = 0; i < d; ++i) x1[i] = rand() / static_cast<T>(RAND_MAX);
  for (int i = 0; i < d; ++i) x2[i] = rand() / static_cast<T>(RAND_MAX);
  mathfu::Vector<T, d> vector1(x1), vector2(x2);
  // This will test the Hadamard Product of two vectors and verify that each
  // element is the product of the input elements.
  mathfu::Vector<T, d> mult_vec(
    mathfu::Vector<T, d>::HadamardProduct(vector1, vector2));
  for (int i = 0; i < d; ++i) {
    EXPECT_NEAR(x1[i] * x2[i], mult_vec[i], precision);
  }
  // This will test multiplication by a scalar and verify that each
  // element is the input element multipled by the scalar.
  mathfu::Vector<T, d> smult_vec1(vector1 * scalar);
  for (int i = 0; i < d; ++i) {
    EXPECT_NEAR(x1[i] * 1.4, smult_vec1[i], precision);
  }
  mathfu::Vector<T, d> smult_vec2(scalar * vector2);
  for (int i = 0; i < d; ++i) {
    EXPECT_NEAR(x2[i] * 1.4, smult_vec2[i], precision);
  }
  // This will test the dot product of two vectors and verify the result
  // is mathematically correct.
  T my_dot = 0;
  for (int i = 0; i < d; ++i) my_dot += x1[i] * x2[i];
  T vec_dot = mathfu::Vector<T, d>::DotProduct(vector1, vector2);
  EXPECT_NEAR(my_dot, vec_dot, precision);
}
TEST_ALL_F(Mult)

// This will test normalizing a vector. The template parameter d corresponds to
// the size of the vector.
template<class T, int d>
void Norm_Test(const T& precision) {
  T x[d];
  for (int i = 0; i < d; ++i) x[i] = rand()/static_cast<T>(RAND_MAX);
  mathfu::Vector<T, d> vector(x);
  vector.Normalize();
  // This will verify that the dot product is 1.
  T dot = mathfu::Vector<T, d>::DotProduct(vector, vector);
  EXPECT_NEAR(dot, 1, precision);
}
TEST_ALL_F(Norm)

// This will test the cross product of two vectors.
template<class T>
void Cross_Test(const T& precision) {
  mathfu::Vector<T, 3> f1_vector(static_cast<T>(1.1), static_cast<T>(4.5),
                                 static_cast<T>(9.8));
  mathfu::Vector<T, 3> f2_vector(-static_cast<T>(1.4), static_cast<T>(9.5),
                                 static_cast<T>(3.2));
  f1_vector.Normalize(); f2_vector.Normalize();
  mathfu::Vector<T, 3> fcross_vector(
    mathfu::Vector<T, 3>::CrossProduct(f1_vector, f2_vector));
  // This will verify that v1*(v1xv2) and v2*(v1xv2) are 0.
  T f1_dot = mathfu::Vector<T, 3>::DotProduct(fcross_vector, f1_vector);
  T f2_dot = mathfu::Vector<T, 3>::DotProduct(fcross_vector, f2_vector);
  EXPECT_NEAR(f1_dot, 0, precision * 10);
  EXPECT_NEAR(f2_dot, 0, precision * 10);
}
TEST_SCALAR_F(Cross)

// Create a vector with random values between 0~1.
template<class T, int d>
mathfu::Vector<T, d> RandomVector() {
  T x[d];
  for (int i = 0; i < d; ++i) {
    x[i] = rand() / static_cast<T>(RAND_MAX);
  }
  return mathfu::Vector<T, d>(x);
}

// This will test an equal lerp of two vectors gives their average.
template<class T, int d>
void LerpHalf_Test(const T& precision) {
  mathfu::Vector<T, d> vector1(RandomVector<T, d>());
  mathfu::Vector<T, d> vector2(RandomVector<T, d>());
  mathfu::Vector<T, d> flerp_vector(
    mathfu::Vector<T, d>::Lerp(vector1, vector2, static_cast<T>(0.5)));
  // This will verify f1_vector.x + f2_vector.x == 2 * flerp_vector
  for (int i = 0; i < d; ++i) {
    EXPECT_NEAR(vector1[i] + vector2[i],
                static_cast<T>(2.0) * flerp_vector[i], precision * 10);
  }
}
TEST_ALL_F(LerpHalf)

// This will test that lerp with weight 0 returns the first vector.
template<class T, int d>
void Lerp0_Test(const T& precision) {
  mathfu::Vector<T, d> vector1(RandomVector<T, d>());
  mathfu::Vector<T, d> vector2(RandomVector<T, d>());
  mathfu::Vector<T, d> flerp_vector(
    mathfu::Vector<T, d>::Lerp(vector1, vector2, static_cast<T>(0.0)));
  // This will verify f1_vector.x + f2_vector.x == 2 * flerp_vector
  for (int i = 0; i < d; ++i) {
    EXPECT_NEAR(vector1[i], flerp_vector[i], precision * 10);
  }
}
TEST_ALL_F(Lerp0)

// This will test that lerp with weight 1 returns the second vector.
template<class T, int d>
void Lerp1_Test(const T& precision) {
  mathfu::Vector<T, d> vector1(RandomVector<T, d>());
  mathfu::Vector<T, d> vector2(RandomVector<T, d>());
  mathfu::Vector<T, d> flerp_vector(
    mathfu::Vector<T, d>::Lerp(vector1, vector2, static_cast<T>(1.0)));
  // This will verify f1_vector.x + f2_vector.x == 2 * flerp_vector
  for (int i = 0; i < d; ++i) {
    EXPECT_NEAR(vector2[i], flerp_vector[i], precision * 10);
  }
}
TEST_ALL_F(Lerp1)

// This will test initializaiton by specifying all values explictly.
template<class T>
void Clamp_Test() {
  const T min = static_cast<T>(-1);
  const T max = static_cast<T>(8);
  const T inside = static_cast<T>(7);
  const T above = static_cast<T>(9);
  const T below = static_cast<T>(-11);

  EXPECT_EQ(mathfu::Clamp<T>(inside, min, max), inside);
  EXPECT_EQ(mathfu::Clamp<T>(above, min, max), max);
  EXPECT_EQ(mathfu::Clamp<T>(below, min, max), min);
  EXPECT_EQ(mathfu::Clamp<T>(max, min, max), max);
  EXPECT_EQ(mathfu::Clamp<T>(min, min, max), min);
}
TEST_F(VectorTests, Clamp) { \
  Clamp_Test<float>(); \
  Clamp_Test<double>(); \
  Clamp_Test<int>(); \
}

// Tests for int/float/double based lerp.  (i. e. not part of a vector)
template<class T>
void Numeric_Lerp_Test(const T& precision) {
  const T zero = static_cast<T>(0);
  const T one = static_cast<T>(1);

  const T a = static_cast<T>(10);
  const T b = static_cast<T>(20);
  const T midpoint = static_cast<T>(0.5);
  const T two_fifths = static_cast<T>(0.4);
  const T seven_tenths = static_cast<T>(0.7);
  const T midpoint_result = static_cast<T>(15);
  const T two_fifths_result = static_cast<T>(14);
  const T seven_tenths_result = static_cast<T>(17);

  EXPECT_EQ(mathfu::Lerp<T>(a, b, zero), a);
  EXPECT_EQ(mathfu::Lerp<T>(a, b, one), b);
  EXPECT_EQ(mathfu::Lerp<T>(-a, b, zero), -a);
  EXPECT_EQ(mathfu::Lerp<T>(-a, b, one), b);
  EXPECT_EQ(mathfu::Lerp<T>(a, -b, zero), a);
  EXPECT_EQ(mathfu::Lerp<T>(a, -b, one), -b);
  EXPECT_EQ(mathfu::Lerp<T>(-a, -b, zero), -a);
  EXPECT_EQ(mathfu::Lerp<T>(-a, -b, one), -b);

  EXPECT_NE(mathfu::Lerp<T>(a, b, midpoint), a);

  EXPECT_NEAR(mathfu::Lerp<T>(a, b, midpoint), midpoint_result,
                              precision);
  EXPECT_NEAR(mathfu::Lerp<T>(a, b, two_fifths), two_fifths_result,
                              precision);
  EXPECT_NEAR(mathfu::Lerp<T>(a, b, seven_tenths), seven_tenths_result,
                                precision);
}

TEST_F(VectorTests, Lerp) { \
  Numeric_Lerp_Test<float>(FLOAT_PRECISION); \
  Numeric_Lerp_Test<double>(DOUBLE_PRECISION); \
}

// Test the compilation of basic vector opertations given in the sample file.
// This will test creation of two vectors and computing their cross product.
TEST_F(VectorTests, SampleTest) {
    using namespace mathfu;
    /// @doxysnippetstart Chapter02_Vectors.md Vector_Sample
    Vector<float, 3> point1(0.5f, 0.4f, 0.1f);
    Vector<float, 3> point2(0.4f, 0.9f, 0.1f);
    Vector<float, 3> point3(0.1f, 0.8f, 0.6f);

    Vector<float, 3> vector1 = point2 - point1;
    Vector<float, 3> vector2 = point3 - point1;

    Vector<float, 3> normal = Vector<float, 3>::CrossProduct(vector2, vector1);
    /// @doxysnippetend
    const float precision = 1e-2f;
    EXPECT_NEAR(-0.25f, normal[0], precision);
    EXPECT_NEAR(-0.05f, normal[1], precision);
    EXPECT_NEAR(-0.16f, normal[2], precision);
}

// This will test that the constants have the correct values.
TEST_F(VectorTests, ConstantTest) {
  // Check values of various kinds of Vector2s.
  for (int i = 0; i < 2; ++i) {
    VECTOR_TEST_CONSTANT_EQ(kZeros2, i, 0);
    VECTOR_TEST_CONSTANT_EQ(kOnes2, i, 1);
    VECTOR_TEST_CONSTANT_EQ(kAxisX2, i, i == 0 ? 1 : 0);
    VECTOR_TEST_CONSTANT_EQ(kAxisY2, i, i == 1 ? 1 : 0);
  }

  // Check values of various kinds of Vector3s.
  for (int i = 0; i < 3; ++i) {
    VECTOR_TEST_CONSTANT_EQ(kZeros3, i, 0);
    VECTOR_TEST_CONSTANT_EQ(kOnes3, i, 1);
    VECTOR_TEST_CONSTANT_EQ(kAxisX3, i, i == 0 ? 1 : 0);
    VECTOR_TEST_CONSTANT_EQ(kAxisY3, i, i == 1 ? 1 : 0);
    VECTOR_TEST_CONSTANT_EQ(kAxisZ3, i, i == 2 ? 1 : 0);
  }

  // Check values of various kinds of Vector4s.
  for (int i = 0; i < 4; ++i) {
    VECTOR_TEST_CONSTANT_EQ(kZeros4, i, 0);
    VECTOR_TEST_CONSTANT_EQ(kOnes4, i, 1);
    VECTOR_TEST_CONSTANT_EQ(kAxisX4, i, i == 0 ? 1 : 0);
    VECTOR_TEST_CONSTANT_EQ(kAxisY4, i, i == 1 ? 1 : 0);
    VECTOR_TEST_CONSTANT_EQ(kAxisZ4, i, i == 2 ? 1 : 0);
    VECTOR_TEST_CONSTANT_EQ(kAxisW4, i, i == 3 ? 1 : 0);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  printf("%s (%s)\n", argv[0], MATHFU_BUILD_OPTIONS_STRING);
  return RUN_ALL_TESTS();
}
