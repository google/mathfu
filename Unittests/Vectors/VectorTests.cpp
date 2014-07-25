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
#include "vectors/Vector.h"
#include "vectors/Vector_2D.h"
#include "vectors/Vector_3D.h"
#include "vectors/Vector_4D.h"
#include "gtest/gtest.h"

#include "AndroidUtil/AndroidMainWrapper.h"
#include "Precision.h"

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

// This will test initializaiton by passing in values. The template paramter d
// corresponds to the size of the vector.
template<class T, int d>
void Initialization_Test(const T& precision) {
  // This will test initialization of the vector using a random single value.
  // The expected result is that all entries equal the given value.
  mathfu::Vector<T, d> vector_splat(3.1);
  for (int i = 0; i < d; ++i) EXPECT_NEAR(3.1, vector_splat[i], precision);
  T x[d];
  for (int i = 0; i < d; ++i) x[i] = rand()/static_cast<T>(RAND_MAX)*100.f;
  // This will test initialization of the vector using a c style array of
  // values.
  mathfu::Vector<T, d> vector_arr(x);
  for (int i = 0; i < d; ++i) EXPECT_NEAR(x[i], vector_arr[i], precision);
  // This will test copy constructor making sure that the new matrix equals
  // the old one.
  mathfu::Vector<T, d> vector_copy(vector_arr);
  for (int i = 0; i < d; ++i) EXPECT_NEAR(x[i], vector_copy[i], precision);
  // This will make sure the copy was deep and chaning the values of the
  // copied matrix does not effect the origional.
  vector_copy -= mathfu::Vector<T, d>(1);
  EXPECT_NE(vector_copy[0], vector_arr[0]);
}
TEST_ALL_F(Initialization)

// This will test initializaiton by specifying all values explictly.
template<class T>
void InitializationPerDimension_Test(const T& precision) {
  mathfu::Vector<T, 2> f2_vector(5.3, 7.1);
  EXPECT_NEAR(5.3, f2_vector[0], precision);
  EXPECT_NEAR(7.1, f2_vector[1], precision);
  mathfu::Vector<T, 3> f3_vector(4.3, 1.1, 3.2);
  EXPECT_NEAR(4.3, f3_vector[0], precision);
  EXPECT_NEAR(1.1, f3_vector[1], precision);
  EXPECT_NEAR(3.2, f3_vector[2], precision);
  mathfu::Vector<T, 4> f4_vector(2.3, 4.6, 9.2, 15.5);
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
  for (int i = 0; i < d; ++i) x1[i] = rand()/static_cast<T>(RAND_MAX)*100.f;
  for (int i = 0; i < d; ++i) x2[i] = rand()/static_cast<T>(RAND_MAX)*100.f;
  mathfu::Vector<T, d> vector1(x1), vector2(x2);
  // This will test the negation of a vector and make sure each element is
  // negated.
  mathfu::Vector<T, d> neg_vector1(-vector1);
  for (int i = 0; i < d; ++i) EXPECT_NEAR(-x1[i], neg_vector1[i], precision);
  // This will test the addition of vectors and make such each element is
  // equal to the sum of the input values.
  mathfu::Vector<T, d> sum_vector(vector1 + vector2);
  for (int i = 0; i < d; ++i)
    EXPECT_NEAR(x1[i] + x2[i], sum_vector[i], precision);
  // This will test the subtraction of vectors and make such each element is
  // equal to the difference of the input values.
  mathfu::Vector<T, d> diff_vector(vector1 - vector2);
  for (int i = 0; i < d; ++i)
    EXPECT_NEAR(x1[i] - x2[i], diff_vector[i], precision);
}
TEST_ALL_F(AddSub)

// This will test the mutiplication of vectors by vectors and scalars. The
// template paramter d corresponds to the size of the vector.
template<class T, int d>
void Mult_Test(const T& precision) {
  T x1[d], x2[d], scalar(1.4);
  for (int i = 0; i < d; ++i) x1[i] = rand()/static_cast<T>(RAND_MAX);
  for (int i = 0; i < d; ++i) x2[i] = rand()/static_cast<T>(RAND_MAX);
  mathfu::Vector<T, d> vector1(x1), vector2(x2);
  // This will test the Hadamard Product of two vectors and verify that each
  // element is the product of the input elements.
  mathfu::Vector<T, d> mult_vec(
    mathfu::Vector<T, d>::HadamardProduct(vector1, vector2));
  for (int i = 0; i < d; ++i) EXPECT_NEAR(x1[i] * x2[i], mult_vec[i], precision);
  // This will test multiplication by a scalar and verify that each
  // element is the input element multipled by the scalar.
  mathfu::Vector<T, d> smult_vec1(vector1 * scalar);
  for (int i = 0; i < d; ++i) EXPECT_NEAR(x1[i] * 1.4, smult_vec1[i], precision);
  mathfu::Vector<T, d> smult_vec2(scalar * vector2);
  for (int i = 0; i < d; ++i) EXPECT_NEAR(x2[i] * 1.4, smult_vec2[i], precision);
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

// This will test the cross prodcut of two vectors.
template<class T>
void Cross_Test(const T& precision) {
  mathfu::Vector<T, 3> f1_vector(1.1, 4.5, 9.8);
  mathfu::Vector<T, 3> f2_vector(-1.4, 9.5, 3.2);
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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
