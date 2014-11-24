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
#include "mathfu/matrix.h"
#include "mathfu/matrix_4x4.h"
#include "mathfu/quaternion.h"
#include "mathfu/vector.h"
#include "mathfu/utilities.h"

#include <cmath>
#include <string>
#include <sstream>

#include "gtest/gtest.h"

#include "precision.h"

class MatrixTests : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

template<class T, int rows, int columns>
struct MatrixExpectation {
  const char *description;
  mathfu::Matrix<T, rows, columns> calculated;
  mathfu::Matrix<T, rows, columns> expected;
};


// This will automatically generate tests for each template parameter.
#define TEST_ALL_F(MY_TEST, FLOAT_PRECISION_VALUE, DOUBLE_PRECISION_VALUE) \
  TEST_F(MatrixTests, MY_TEST##_float_2) { \
    MY_TEST##_Test<float, 2>(FLOAT_PRECISION_VALUE); \
  } \
  TEST_F(MatrixTests, MY_TEST##_double_2) { \
    MY_TEST##_Test<double, 2>(DOUBLE_PRECISION_VALUE); \
  } \
  TEST_F(MatrixTests, MY_TEST##_float_3) { \
    MY_TEST##_Test<float, 3>(FLOAT_PRECISION_VALUE); \
  } \
  TEST_F(MatrixTests, MY_TEST##_double_3) { \
    MY_TEST##_Test<double, 3>(DOUBLE_PRECISION_VALUE); \
  } \
  TEST_F(MatrixTests, MY_TEST##_float_4) { \
    MY_TEST##_Test<float, 4>(FLOAT_PRECISION_VALUE); \
  } \
  TEST_F(MatrixTests, MY_TEST##_double_4) { \
    MY_TEST##_Test<double, 4>(DOUBLE_PRECISION_VALUE); \
  }

// This will automatically generate tests for each scalar template parameter.
#define TEST_SCALAR_F(MY_TEST, FLOAT_PRECISION_VALUE, DOUBLE_PRECISION_VALUE) \
  TEST_F(MatrixTests, MY_TEST##_float) { \
    MY_TEST##_Test<float>(FLOAT_PRECISION_VALUE); \
  } \
  TEST_F(MatrixTests, MY_TEST##_double) { \
    MY_TEST##_Test<double>(DOUBLE_PRECISION_VALUE); \
  }

// This will test initialization by passing in values. The template parameter d
// corresponds to the number of rows and columns.
template<class T, int d>
void Initialize_Test(const T& precision) {
  // This will test initialization of the matrix using a random single value.
  // The expected result is that all entries equal the given value.
  mathfu::Matrix<T, d> matrix_splat(static_cast<T>(3.1));
  for (int i = 0; i < d * d; ++i) {
    EXPECT_NEAR(3.1, matrix_splat[i], precision);
  }
  // This will verify that the value is correct when using the (i, j) form
  // of indexing.
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      EXPECT_NEAR(3.1, matrix_splat(i, j), precision);
    }
  }
  // This will test initialization of the matrix using a c style array of
  // values.
  T x[d * d];
  for (int i = 0; i < d * d; ++i) {
    x[i] = rand() / static_cast<T>(RAND_MAX) * 100.f;
  }
  mathfu::Matrix<T, d> matrix_arr(x);
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      EXPECT_NEAR(x[i + d * j], matrix_arr(i, j), precision);
    }
  }
  // This will test the copy constructor making sure that the new matrix
  // equals the old one.
  mathfu::Matrix<T, d> matrix_copy(matrix_arr);
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      EXPECT_NEAR(x[i + d * j], matrix_copy(i, j), precision);
    }
  }
  // This will verify that the copy was deep and changing the values of the
  // copied matrix does not effect the original.
  matrix_copy = matrix_copy - mathfu::Matrix<T, d>(1);
  EXPECT_NE(matrix_copy(0, 0), matrix_arr(0, 0));
  // This will test creation of the identity matrix.
  mathfu::Matrix<T, d> identity(mathfu::Matrix<T, d>::Identity());
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      EXPECT_NEAR(i == j ? 1 : 0, identity(i, j), precision);
    }
  }
}
TEST_ALL_F(Initialize, FLOAT_PRECISION, DOUBLE_PRECISION);

// This will test initialization by specifying all values explicitly.
template<class T>
void InitializePerDimension_Test(const T& precision) {
  mathfu::Matrix<T, 2> matrix_f2x2(static_cast<T>(4.5), static_cast<T>(3.4),
                                   static_cast<T>(2.6), static_cast<T>(9.8));
  EXPECT_NEAR(4.5, matrix_f2x2(0, 0), precision);
  EXPECT_NEAR(3.4, matrix_f2x2(1, 0), precision);
  EXPECT_NEAR(2.6, matrix_f2x2(0, 1), precision);
  EXPECT_NEAR(9.8, matrix_f2x2(1, 1), precision);
  mathfu::Matrix<T, 3> matrix_f3x3(
    static_cast<T>(3.7), static_cast<T>(2.4), static_cast<T>(6.4),
    static_cast<T>(1.1), static_cast<T>(5.2), static_cast<T>(6.4),
    static_cast<T>(2.7), static_cast<T>(7.4), static_cast<T>(0.1));
  EXPECT_NEAR(3.7, matrix_f3x3(0, 0), precision);
  EXPECT_NEAR(2.4, matrix_f3x3(1, 0), precision);
  EXPECT_NEAR(6.4, matrix_f3x3(2, 0), precision);
  EXPECT_NEAR(1.1, matrix_f3x3(0, 1), precision);
  EXPECT_NEAR(5.2, matrix_f3x3(1, 1), precision);
  EXPECT_NEAR(6.4, matrix_f3x3(2, 1), precision);
  EXPECT_NEAR(2.7, matrix_f3x3(0, 2), precision);
  EXPECT_NEAR(7.4, matrix_f3x3(1, 2), precision);
  EXPECT_NEAR(0.1, matrix_f3x3(2, 2), precision);
  mathfu::Matrix<T, 4> matrix_f4x4(
    static_cast<T>(4.1), static_cast<T>(8.4), static_cast<T>(7.2),
    static_cast<T>(4.8), static_cast<T>(0.9), static_cast<T>(7.8),
    static_cast<T>(5.6), static_cast<T>(8.7), static_cast<T>(2.3),
    static_cast<T>(4.2), static_cast<T>(6.1), static_cast<T>(2.7),
    static_cast<T>(0.1), static_cast<T>(1.4), static_cast<T>(9.4),
    static_cast<T>(3.6));
  EXPECT_NEAR(4.1, matrix_f4x4(0, 0), precision);
  EXPECT_NEAR(8.4, matrix_f4x4(1, 0), precision);
  EXPECT_NEAR(7.2, matrix_f4x4(2, 0), precision);
  EXPECT_NEAR(4.8, matrix_f4x4(3, 0), precision);
  EXPECT_NEAR(0.9, matrix_f4x4(0, 1), precision);
  EXPECT_NEAR(7.8, matrix_f4x4(1, 1), precision);
  EXPECT_NEAR(5.6, matrix_f4x4(2, 1), precision);
  EXPECT_NEAR(8.7, matrix_f4x4(3, 1), precision);
  EXPECT_NEAR(2.3, matrix_f4x4(0, 2), precision);
  EXPECT_NEAR(4.2, matrix_f4x4(1, 2), precision);
  EXPECT_NEAR(6.1, matrix_f4x4(2, 2), precision);
  EXPECT_NEAR(2.7, matrix_f4x4(3, 2), precision);
  EXPECT_NEAR(0.1, matrix_f4x4(0, 3), precision);
  EXPECT_NEAR(1.4, matrix_f4x4(1, 3), precision);
  EXPECT_NEAR(9.4, matrix_f4x4(2, 3), precision);
  EXPECT_NEAR(3.6, matrix_f4x4(3, 3), precision);
}
TEST_SCALAR_F(InitializePerDimension, FLOAT_PRECISION, DOUBLE_PRECISION)

// Test initialization of a matrix from an array of packed vectors.
template<class T, int d>
void InitializePacked_Test(const T& precision) {
  (void)precision;
  mathfu::VectorPacked<T, d> packed[d];
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      packed[i].data[j] = static_cast<T>((i * d) + j);
    }
  }
  mathfu::Matrix<T, d> matrix(packed);
  for (int i = 0; i < d * d; ++i) {
    EXPECT_NEAR(packed[i / d].data[i % d], matrix[i],
                static_cast<T>(0)) << "Element " << i;
  }
}
TEST_ALL_F(InitializePacked, FLOAT_PRECISION, DOUBLE_PRECISION);

// Test serialization to a packed array of vectors.
template<class T, int d>
void PackedSerialization_Test(const T& precision) {
  (void)precision;
  mathfu::Matrix<T, d> matrix;
  for (int i = 0; i < d * d; ++i) {
    matrix[i] = static_cast<T>(i);
  }
  mathfu::VectorPacked<T, d> packed[d];
  matrix.Pack(packed);
  for (int i = 0; i < d * d; ++i) {
    EXPECT_NEAR(matrix[i], packed[i / d].data[i % d],
                static_cast<T>(0)) << "Element " << i;
  }
}
TEST_ALL_F(PackedSerialization, FLOAT_PRECISION, DOUBLE_PRECISION);

// This will test the Addition and Subtraction of matrices. The template
// parameter d corresponds to the number of rows and columns.
template<class T, int d>
void AddSub_Test(const T& precision) {
  T x1[d * d], x2[d * d];
  for (int i = 0; i < d * d; ++i) {
    x1[i] = rand() / static_cast<T>(RAND_MAX) * 100.f;
  }
  for (int i = 0; i < d * d; ++i) {
    x2[i] = rand() / static_cast<T>(RAND_MAX) * 100.f;
  }
  mathfu::Matrix<T, d> matrix1(x1), matrix2(x2);
  // This will test the negation of a matrix and verify that each element
  // is negated.
  mathfu::Matrix<T, d> neg_mat1(-matrix1);
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      EXPECT_NEAR(-x1[i + d * j], neg_mat1(i, j), precision);
    }
  }
  // This will test the addition of two matrices and verify that each element
  // equal to the sum of the input values.
  mathfu::Matrix<T, d> matrix_add(matrix1 + matrix2);
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      EXPECT_NEAR(x1[i + d * j] + x2[i + d * j], matrix_add(i, j), precision);
    }
  }
  // This will test the subtraction of two matrices and verify that each
  // element equal to the difference of the input values.
  mathfu::Matrix<T, d> matrix_sub(matrix1 - matrix2);
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      EXPECT_NEAR(x1[i + d * j] - x2[i + d * j], matrix_sub(i, j), precision);
    }
  }
}
TEST_ALL_F(AddSub, FLOAT_PRECISION, DOUBLE_PRECISION);

// This will test the multiplication of matrices by matrices, vectors,
// and scalars. The template parameter d corresponds to the number of rows and
// columns.
template<class T, int d>
void Mult_Test(const T& precision) {
  T x1[d * d], x2[d * d];
  for (int i = 0; i < d * d; ++i) x1[i] = rand() / static_cast<T>(RAND_MAX);
  for (int i = 0; i < d * d; ++i) x2[i] = rand() / static_cast<T>(RAND_MAX);
  mathfu::Matrix<T, d> matrix1(x1), matrix2(x2);
  // This will test scalar matrix multiplication and verify that each element
  // is equal to multiplication by the scalar.
  mathfu::Matrix<T, d> matrix_mults(matrix1 * static_cast<T>(1.1));
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      EXPECT_NEAR(x1[i + d * j] * 1.1, matrix_mults(i, j), precision);
    }
  }
  T v[d];
  for (int i = 0; i < d; ++i) v[i] = rand() / static_cast<T>(RAND_MAX);
  mathfu::Vector<T, d> vector(v);
  // This will test matrix vector multiplication and verify that the resulting
  // vector is mathematically correct.
  mathfu::Vector<T, d> vector_multv(matrix1 * vector);
  for (int i = 0; i < d; ++i) {
    T v1[d];
    for (int k = 0; k < d; ++k) v1[k] = matrix1(i, k);
    mathfu::Vector<T, d> vec1(v1);
    T dot = mathfu::Vector<T, d>::DotProduct(vec1, vector);
    EXPECT_NEAR(dot, vector_multv[i], precision);
  }
  // This will test matrix multiplication and verify that the resulting
  // matrix is mathematically correct.
  mathfu::Matrix<T, d> matrix_multm(matrix1 * matrix2);
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      T v1[d], v2[d];
      for (int k = 0; k < d; ++k) v1[k] = matrix1(i, k);
      for (int k = 0; k < d; ++k) v2[k] = matrix2(k, j);
      mathfu::Vector<T, d> vec1(v1), vec2(v2);
      T dot = mathfu::Vector<T, d>::DotProduct(vec1, vec2);
      EXPECT_NEAR(dot, matrix_multm(i, j), precision);
    }
  }
}
TEST_ALL_F(Mult, FLOAT_PRECISION, DOUBLE_PRECISION);

// This will test the outer product of two vectors. The template parameter d
// corresponds to the number of rows and columns.
template<class T, int d>
void OuterProduct_Test(const T& precision) {
  T x1[d], x2[d];
  for (int i = 0; i < d; ++i) x1[i] = rand() / static_cast<T>(RAND_MAX);
  for (int i = 0; i < d; ++i) x2[i] = rand() / static_cast<T>(RAND_MAX);
  mathfu::Vector<T, d> vector1(x1), vector2(x2);
  mathfu::Matrix<T, d> matrix(
    mathfu::Matrix<T, d>::OuterProduct(vector1, vector2));
  // This will verify that each element is mathematically correct.
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      EXPECT_NEAR(vector1[i] * vector2[j], matrix(i, j), precision);
    }
  }
}
TEST_ALL_F(OuterProduct, FLOAT_PRECISION, DOUBLE_PRECISION);

// Print the specified matrix to output_string in the form.
template<class T, int rows, int columns>
std::string MatrixToString(const mathfu::Matrix<T, rows, columns> &matrix) {
  std::stringstream ss;
  ss.flags(std::ios::fixed);
  ss.precision(4);
  for (int col = 0; col < columns; ++col) {
    for (int row = 0; row < rows; ++row) {
      ss << (T)matrix(col, row) << " ";
    }
    ss << "\n";
  }
  return ss.str();
}

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif  // _MSC_VER

// Test the inverse of a set of noninvertible matrices.
template<class T, int d>
void InverseNonInvertible_Test(const T& precision) {
  (void)precision;
  T m[d * d];
  const size_t matrix_size = sizeof(m) / sizeof(m[0]);
  static const T kDeterminantThreshold =
      mathfu::Constants<T>::GetDeterminantThreshold();
  static const T kDeterminantThresholdInverse = 1 / kDeterminantThreshold;
  static const T kDeterminantThresholdSmall =
      kDeterminantThresholdInverse / 100;
  static const T kDeterminantThresholdInverseLarge =
      kDeterminantThresholdInverse * 100;
  // Create a matrix with all zeros.
  for (size_t i = 0; i < matrix_size; ++i) m[i] = 0;
  // Verify that it's not possible to invert the matrix.
  {
    mathfu::Matrix<T, d> matrix(m);
    mathfu::Matrix<T, d> inverse_matrix;
    EXPECT_FALSE(matrix.InverseWithDeterminantCheck(&inverse_matrix));
  }
  // Check a matrix with all elements at the determinant threshold.
  for (size_t i = 0; i < matrix_size; ++i) m[i] = kDeterminantThreshold;
  {
    mathfu::Matrix<T, d> matrix(m);
    mathfu::Matrix<T, d> inverse_matrix;
    EXPECT_FALSE(matrix.InverseWithDeterminantCheck(&inverse_matrix));
  }
  // Check a matrix with all very large elements.
  for (size_t i = 0; i < matrix_size - 1; ++i) {
    m[i] = kDeterminantThresholdInverse;
  }
  m[matrix_size - 1] = kDeterminantThresholdInverseLarge;
  // NOTE: Due to precision, this case will pass the determinant check with a
  // 2x2 matrix since the determinant of the matrix will be calculated as 1.
  if (d != 2) {
    // Create a matrix with all elements at the determinant threshold and one
    // large value in the matrix.
    for (size_t i = 0; i < matrix_size - 1; ++i) {
      m[i] = kDeterminantThresholdSmall;
    }
    m[matrix_size - 1] = kDeterminantThresholdInverseLarge;
    {
      mathfu::Matrix<T, d> matrix(m);
      mathfu::Matrix<T, d> inverse_matrix;
      EXPECT_FALSE(matrix.InverseWithDeterminantCheck(&inverse_matrix));
    }
  }
}
TEST_ALL_F(InverseNonInvertible, FLOAT_PRECISION, DOUBLE_PRECISION);

#ifdef _MSC_VER
#pragma warning(pop)
#endif  // _MSC_VER

// This will test calculating the inverse of a matrix. The template parameter d
// corresponds to the number of rows and columns.
template<class T, int d>
void Inverse_Test(const T& precision) {
  T x[d * d];
  for (int iterations = 0; iterations < 1000; ++iterations) {
    // NOTE: This assumes that matrices generated here are invertible since
    // there is a tiny probability that a randomly generated matrix will be
    // noninvertible.  This does mean that this test can be flakey by
    // occasionally generating noninvertible matrices.
    for (int i = 0; i < mathfu::Matrix<T, d>::kElements; ++i) {
      x[i] = mathfu::RandomRange<T>(1);
    }
    mathfu::Matrix<T, d> matrix(x);
    std::string error_string = MatrixToString(matrix);
    error_string += "\n";
    mathfu::Matrix<T, d> inverse_matrix(matrix.Inverse());
    mathfu::Matrix<T, d> identity_matrix(matrix * inverse_matrix);

    error_string += MatrixToString(inverse_matrix);
    error_string += "\n";
    error_string += MatrixToString(identity_matrix);

    // This will verify that M * Minv is equal to the identity.
    for (int i = 0; i < d; ++i) {
      for (int j = 0; j < d; ++j) {
        EXPECT_NEAR(i == j ? 1 : 0, identity_matrix(i, j), 100 * precision) <<
            error_string << " row=" << i << " column=" << j;
      }
    }
  }
}
// Due to the number of operations involved and the random numbers used to
// generate the test matrices, the precision the inverse matrix is calculated
// to is relatively low.
TEST_ALL_F(Inverse, 1e-4f, 1e-8);

// This will test converting from a translation into a matrix and back again.
template<class T>
void TranslationVector3D_Test(const T& precision) {
  (void)precision;
  const mathfu::Vector<T, 3> trans(
      static_cast<T>(-100.0), static_cast<T>(0.0), static_cast<T>(0.00003));
  const mathfu::Matrix<T, 4> trans_matrix =
      mathfu::Matrix<T, 4>::FromTranslationVector(trans);
  const mathfu::Vector<T, 3> trans_back = trans_matrix.TranslationVector3D();

  // This will verify that the translation vector has not changed.
  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(trans[i], trans_back[i]);
  }
}
TEST_SCALAR_F(TranslationVector3D, FLOAT_PRECISION, DOUBLE_PRECISION);

// This will test converting from a translation into a matrix and back again.
template<class T>
void TranslationVector2D_Test(const T& precision) {
  (void)precision;
  const mathfu::Vector<T, 2> trans(
      static_cast<T>(-100.0), static_cast<T>(0.00003));
  const mathfu::Matrix<T, 3> trans_matrix =
      mathfu::Matrix<T, 3>::FromTranslationVector(trans);
  const mathfu::Vector<T, 2> trans_back = trans_matrix.TranslationVector2D();

  // This will verify that the translation vector has not changed.
  for (int i = 0; i < 2; ++i) {
    EXPECT_EQ(trans[i], trans_back[i]);
  }
}
TEST_SCALAR_F(TranslationVector2D, FLOAT_PRECISION, DOUBLE_PRECISION);

// This will test converting from a scale into a matrix, then multiply by
// a vector of 1's, which should produce the original scale again.
template<class T, int d>
void FromScaleVector_Test(const T& precision) {
  mathfu::Vector<T, d> ones(static_cast<T>(1));
  mathfu::Vector<T, d - 1> v;

  // Tests that the scale vector is placed in the correct order in the matrix.
  for (int i = 0; i < d - 1; ++i) {
    v[i] = static_cast<T>(i + 10);
  }
  mathfu::Matrix<T, d> m = mathfu::Matrix<T, d>::FromScaleVector(v);

  // Ensure that the v is on the diagonal.
  for (int i = 0; i < d - 1; ++i) {
    EXPECT_NEAR(v[i], m(i, i), precision);
  }

  // Ensure that the last diagonal element is one.
  EXPECT_NEAR(m(d - 1, d - 1), 1, precision);

  // Ensure that all non-diagonal elements are zero.
  for (int i = 0; i < d - 1; ++i) {
    for (int j = 0; j < d - 1; ++j) {
      if (i == j)
        continue;
      EXPECT_NEAR(m(i,j), 0, precision);
    }
  }
}
// Precision is zero. Results must be perfect for this test.
TEST_ALL_F(FromScaleVector, 0.0f, 0.0);

// Compare a set of Matrix<T, rows, columns> with expected values.
template<class T, int rows, int columns>
void VerifyMatrixExpectations(
    const MatrixExpectation<T, rows, columns>* test_cases,
    const size_t number_of_test_cases, const T& precision) {
  for (size_t i = 0; i < number_of_test_cases; ++i) {
    const MatrixExpectation<T, rows, columns> &test = test_cases[i];
    for (int j = 0; j < mathfu::Matrix<T, rows, columns>::kElements; ++j) {
      const mathfu::Matrix<T, rows, columns> &calculated = test.calculated;
      const mathfu::Matrix<T, rows, columns> &expected = test.expected;
      EXPECT_NEAR(calculated[j], expected[j], precision) <<
          "element " << j << " (" << (j / columns) << ", " << (j % columns) <<
          "), case " << test.description << "\n" <<
          MatrixToString(calculated) << "vs expected\n" <<
          MatrixToString(expected);
    }
  }
}

// Test perspective matrix calculation.
template<class T>
void Perspective_Test(const T& precision) {
  static const MatrixExpectation<T, 4, 4> kTestCases[] = {
    {
      "normalized handedness=1",
      mathfu::Matrix<T, 4>::Perspective(
          atan(static_cast<T>(1)) * 2, 1, 0, 1, 1),
      mathfu::Matrix<T, 4>(1, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, -1, -1,
                           0, 0, 0, 0),
    },
    {
      "normalized handedness=-1",
      mathfu::Matrix<T, 4>::Perspective(
          atan(static_cast<T>(1)) * 2, 1, 0, 1, -1),
      mathfu::Matrix<T, 4>(1, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, 1, 1,
                           0, 0, 0, 0),
    },
    {
      "widefov",
      mathfu::Matrix<T, 4>::Perspective(
          atan(static_cast<T>(2)) * 2, 1, 0, 1, 1),
      mathfu::Matrix<T, 4>(0.5, 0, 0, 0,
                           0, 0.5, 0, 0,
                           0, 0, -1, -1,
                           0, 0, 0, 0),
    },
    {
      "narrowfov",
      mathfu::Matrix<T, 4>::Perspective(
          atan(static_cast<T>(0.1)) * 2, 1, 0, 1, 1),
      mathfu::Matrix<T, 4>(10, 0, 0, 0,
                           0, 10, 0, 0,
                           0, 0, -1, -1,
                           0, 0, 0, 0),
    },
    {
      "2:1 aspect ratio",
      mathfu::Matrix<T, 4>::Perspective(
          atan(static_cast<T>(1)) * 2, 0.5, 0, 1, 1),
      mathfu::Matrix<T, 4>(2, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, -1, -1,
                           0, 0, 0, 0),
    },
    {
      "deeper view frustrum",
      mathfu::Matrix<T, 4>::Perspective(
          atan(static_cast<T>(1)) * 2, 1, -2, 2, 1),
      mathfu::Matrix<T, 4>(1, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, -0.5, -1,
                           0, 0, 1, 0),
    },
  };
  VerifyMatrixExpectations(
      kTestCases, sizeof(kTestCases) / sizeof(kTestCases[0]), precision);
}
TEST_SCALAR_F(Perspective, FLOAT_PRECISION, DOUBLE_PRECISION * 10);

// Test orthographic matrix calculation.
template<class T>
void Ortho_Test(const T& precision) {
  static const MatrixExpectation<T, 4, 4> kTestCases[] = {
    {
      "normalized",
      mathfu::Matrix<T, 4, 4>::Ortho(0, 2, 0, 2, 2, 0),
      mathfu::Matrix<T, 4, 4>(1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              -1, -1, 1, 1),
    },
    {
      "narrow",
      mathfu::Matrix<T, 4, 4>::Ortho(1, 3, 0, 2, 2, 0),
      mathfu::Matrix<T, 4, 4>(1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              -2, -1, 1, 1),

    },
    {
      "squat",
      mathfu::Matrix<T, 4, 4>::Ortho(0, 2, 1, 3, 2, 0),
      mathfu::Matrix<T, 4, 4>(1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              -1, -2, 1, 1),

    },
    {
      "deep",
      mathfu::Matrix<T, 4, 4>::Ortho(0, 2, 0, 2, 3, 1),
      mathfu::Matrix<T, 4, 4>(1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              -1, -1, 2, 1),

    },
  };
  VerifyMatrixExpectations(
      kTestCases, sizeof(kTestCases) / sizeof(kTestCases[0]), precision);
}
TEST_SCALAR_F(Ortho, FLOAT_PRECISION, DOUBLE_PRECISION);

// Test look-at matrix calculation.
template<class T>
void LookAt_Test(const T& precision) {
  static const MatrixExpectation<T, 4, 4> kTestCases[] = {
    {
      "origin along z",
      mathfu::Matrix<T, 4, 4>::LookAt(
          mathfu::Vector<T, 3>(0, 0, 1), mathfu::Vector<T, 3>(0, 0, 0),
          mathfu::Vector<T, 3>(0, 1, 0)),
      mathfu::Matrix<T, 4, 4>(1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              0, 0, 0, 1),
    },
    {
      "origin along 2z",
      mathfu::Matrix<T, 4, 4>::LookAt(
          mathfu::Vector<T, 3>(0, 0, 2), mathfu::Vector<T, 3>(0, 0, 0),
          mathfu::Vector<T, 3>(0, 1, 0)),
      mathfu::Matrix<T, 4, 4>(1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              0, 0, 0, 1),
    },
    {
      "origin along x",
      mathfu::Matrix<T, 4, 4>::LookAt(
          mathfu::Vector<T, 3>(1, 0, 0), mathfu::Vector<T, 3>(0, 0, 0),
          mathfu::Vector<T, 3>(0, 1, 0)),
      mathfu::Matrix<T, 4, 4>(0, 0, 1, 0,
                              0, 1, 0, 0,
                              -1, 0, 0, 0,
                              0, 0, 0, 1),
    },
    {
      "origin along y",
      mathfu::Matrix<T, 4, 4>::LookAt(
          mathfu::Vector<T, 3>(0, 1, 0), mathfu::Vector<T, 3>(0, 0, 0),
          mathfu::Vector<T, 3>(1, 0, 0)),
      mathfu::Matrix<T, 4, 4>(0, 1, 0, 0,
                              0, 0, 1, 0,
                              1, 0, 0, 0,
                              0, 0, 0, 1),
    },
    {
      "translated eye, looking along z",
      mathfu::Matrix<T, 4, 4>::LookAt(
          mathfu::Vector<T, 3>(1, 1, 2), mathfu::Vector<T, 3>(1, 1, 1),
          mathfu::Vector<T, 3>(0, 1, 0)),
      mathfu::Matrix<T, 4, 4>(1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              -1, -1, -1, 1),
    },
  };
  VerifyMatrixExpectations(
      kTestCases, sizeof(kTestCases) / sizeof(kTestCases[0]), precision);
}
TEST_SCALAR_F(LookAt, FLOAT_PRECISION, DOUBLE_PRECISION);

// Test matrix transposition.
template<class T, int d>
void Transpose_Test(const T& precision) {
  (void)precision;
  mathfu::Matrix<T, d> matrix;
  for (int i = 0; i < d * d; ++i) {
    matrix[i] = static_cast<T>(i);
  }
  mathfu::Matrix<T, d> transpose = matrix.Transpose();
  for (int i = 0; i < d; ++i) {
    for (int j = 0; j < d; ++j) {
      EXPECT_NEAR(matrix(i, j), transpose(j, i),
                  static_cast<T>(0));
    }
  }
}

TEST_ALL_F(Transpose, FLOAT_PRECISION, DOUBLE_PRECISION);

// This will test converting from a translation into a matrix and back again.
// Test the compilation of basic matrix operations given in the sample file.
// This will test transforming a vector with a matrix.
TEST_F(MatrixTests, MatrixSample) {
    using namespace mathfu;
    /// @doxysnippetstart Chapter04_Matrices.md Matrix_Sample
    Vector<float, 3> trans(3.f, 2.f, 8.f);
    Vector<float, 3> rotation(0.4f, 1.4f, 0.33f);
    Vector<float, 3> vector(4.f, 8.f, 1.f);

    Quaternion<float> rotQuat = Quaternion<float>::FromEulerAngles(rotation);
    Matrix<float, 3> rotMatrix = rotQuat.ToMatrix();
    Matrix<float, 4> transMatrix =
        Matrix<float, 4>::FromTranslationVector(trans);
    Matrix<float, 4> rotHMatrix =
        Matrix<float, 4>::FromRotationMatrix(rotMatrix);

    Matrix<float, 4> matrix = transMatrix * rotHMatrix;
    Vector<float, 3> rotatedVector = matrix * vector;
    /// @doxysnippetend
    const float precision = 1e-2f;
    EXPECT_NEAR(5.14f, rotatedVector[0], precision);
    EXPECT_NEAR(10.11f, rotatedVector[1], precision);
    EXPECT_NEAR(4.74f, rotatedVector[2], precision);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  printf("%s (%s)\n", argv[0], MATHFU_BUILD_OPTIONS_STRING);
  return RUN_ALL_TESTS();
}
