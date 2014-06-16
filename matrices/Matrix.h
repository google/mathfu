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
#ifndef GOOMATH_MATRICES_MATRIX_
#define GOOMATH_MATRICES_MATRIX_

#include <vectors/Vector.h>
#include <vectors/Vector_2D.h>
#include <vectors/Vector_3D.h>
#include <vectors/Vector_4D.h>
#include <utilities/Utilities.h>
#include <assert.h>

#if defined(COMPILE_WITH_SIMD) && defined(COMPILE_WITH_PADDING)
#define VEC3_SIZE 4
#else
#define VEC3_SIZE 3
#endif

// This will unroll loops for matrices with <= 4 columns
#define GOOMATH_MAT_OPERATION(OP) \
  const int i = 0; OP; \
  if (columns > 1) { \
    const int i = 1; OP; \
    if (columns > 2) { \
      const int i = 2; OP; \
      if (columns > 3) { \
        const int i = 3; OP; \
        if (columns > 4) { \
          for (int i = 4; i < columns; ++i) OP; \
        } \
      } \
    } \
  }

// This will perform a given OP on each matrix column and return the result
#define GOOMATH_MAT_OPERATOR(OP) \
  { Matrix<T, rows, columns> result; \
  GOOMATH_MAT_OPERATION(result.data_[i] = OP); \
  return result; }

// This will perform a given OP on each matrix column
#define GOOMATH_MAT_SELF_OPERATOR(OP) \
  { GOOMATH_MAT_OPERATION(OP); \
  return *this; }

// This macro will take the dot product for a row from data1 and a column from
// data2.
#define GOOMATH_MATRIX_4X4_DOT(data1, data2, r) \
  (data1[r] * data2[0] + data1[r + 4] * data2[1] + \
  data1[r + 8] * data2[2] + data1[r + 12] * data2[3])

#define GOOMATH_MATRIX_3X3_DOT(data1, data2, r, size) \
  (data1[r] * data2[0] + data1[r + size] * data2[1] + \
  data1[r + 2 * size] * data2[2])

namespace goomath {

template<class T, int rows, int columns = rows> class Matrix;
template<class T, int rows, int columns>
inline T& GetHelper(Matrix<T, rows, columns>& m, const int i);
template<class T, int rows, int columns>
inline const T ConstGetHelper(Matrix<T, rows, columns>& m, const int i);
template<class T, int rows, int columns>
inline Matrix<T, rows, columns> IdentityHelper();
template<class T, int rows, int columns>
inline Matrix<T, rows, columns> InverseHelper();
template<class T, int rows, int columns>
inline void TimesHelper(
  const Matrix<T, rows, columns>& m1, const Matrix<T, rows, columns>& m2,
  Matrix<T, rows, columns>* out_m);
template<class T, int rows, int columns>
static inline Matrix<T, rows, columns> OuterProductHelper(
  const Vector<T, rows>& v1, const Vector<T, columns>& v2);

/// @class Matrix
/// Stores a Matrix of rows*columns elements with type T and provides a set of
/// utility operations on each matrix.
template<class T, int rows, int columns>
class Matrix {
 public:
  /// Create a matrix of uninitialized values.
  inline Matrix() {}

  /// Create a matrix from another matrix copying each element.
  /// @param m Matrix that the data will be copied from.
  inline Matrix(const Matrix<T, rows, columns>& m) {
    GOOMATH_MAT_OPERATION(data_[i] = m.data_[i]);
  }

  /// Create a matrix from a single float. Each elements is set to be equal to
  /// the value given.
  /// @param s Scalar value that the matrix will be initialized to.
  explicit inline Matrix(const T& s) {
    GOOMATH_MAT_OPERATION((data_[i] = Vector<T, rows>(s)));
  }

  /// Create a matrix from four floats. This method only works for a 2x2 matrix.
  /// @param s00 A scalar for the element in the first row and column.
  /// @param s10 A scalar for the element in the second row, first column.
  /// @param s01 A scalar for the element in the first row, second column.
  /// @param s11 A scalar for the element in the second row and column.
  inline Matrix(const T& s00, const T& s10, const T& s01, const T& s11) {
    STATIC_ASSERT(rows == 2 && columns == 2);
    data_[0] = Vector<T, rows>(s00, s10);
    data_[1] = Vector<T, rows>(s01, s11);
  }

  /// Create a matrix from nine floats. This method only works for a 3x3 matrix.
  /// @param s00 A scalar for the element in the first row and column.
  /// @param s10 A scalar for the element in the second row, first column.
  /// @param s20 A scalar for the element in the third row, first column.
  /// @param s01 A scalar for the element in the first row, second column.
  /// @param s11 A scalar for the element in the second row and column.
  /// @param s21 A scalar for the element in the third row, second column.
  /// @param s02 A scalar for the element in the first row, third column.
  /// @param s12 A scalar for the element in the second row, third column.
  /// @param s22 A scalar for the element in the third row and column.
  inline Matrix(
    const T& s00, const T& s10, const T& s20, const T& s01, const T& s11,
    const T& s21, const T& s02, const T& s12, const T& s22) {
    STATIC_ASSERT(rows == 3 && columns == 3);
    data_[0] = Vector<T, rows>(s00, s10, s20);
    data_[1] = Vector<T, rows>(s01, s11, s21);
    data_[2] = Vector<T, rows>(s02, s12, s22);
  }

  /// Create a matrix from sixteen floats. This method only works for a
  /// 4x4 matrix.
  /// @param s00 A scalar for the element in the first row and column.
  /// @param s10 A scalar for the element in the second row, first column.
  /// @param s20 A scalar for the element in the third row, first column.
  /// @param s30 A scalar for the element in the fourth row, first column.
  /// @param s01 A scalar for the element in the first row, second column.
  /// @param s11 A scalar for the element in the second row and column.
  /// @param s21 A scalar for the element in the third row, second column.
  /// @param s31 A scalar for the element in the fourth row, second column.
  /// @param s02 A scalar for the element in the first row, third column.
  /// @param s12 A scalar for the element in the second row, third column.
  /// @param s22 A scalar for the element in the third row and column.
  /// @param s32 A scalar for the element in the fourth row, third column.
  /// @param s03 A scalar for the element in the first row, fourth column.
  /// @param s13 A scalar for the element in the second row, fourth column.
  /// @param s23 A scalar for the element in the third row, fourth column.
  /// @param s33 A scalar for the element in the fourth row and column.
  inline Matrix(
    const T& s00, const T& s10, const T& s20, const T& s30,
    const T& s01, const T& s11, const T& s21, const T& s31,
    const T& s02, const T& s12, const T& s22, const T& s32,
    const T& s03, const T& s13, const T& s23, const T& s33) {
    STATIC_ASSERT(rows == 4 && columns == 4);
    data_[0] = Vector<T, rows>(s00, s10, s20, s30);
    data_[1] = Vector<T, rows>(s01, s11, s21, s31);
    data_[2] = Vector<T, rows>(s02, s12, s22, s32);
    data_[3] = Vector<T, rows>(s03, s13, s23, s33);
  }

  /// Create a matirx form the first row*column elements of an array.
  /// @param a Array of values that the matrix will be iniitlized to.
  explicit inline Matrix(const T* a) {
    GOOMATH_MAT_OPERATION((data_[i] = Vector<T, rows>(&a[i*columns])));
  }

  /// Access an element of the matrix.
  /// @param i The index of the row where the elment is located.
  /// @param j The index of the column where the elment is located.
  /// @return A const reference to the accessed data that cannot be modified
  /// by the caller.
  inline const T operator()(const int i, const int j) const {
    return data_[j][i];
  }

  /// Access an element of the matrix.
  /// @param i The index of the row where the elment is located.
  /// @param j The index of the column where the elment is located.
  /// @return A reference to the accessed data that can be modified by the
  /// caller.
  inline T& operator()(const int i, const int j) {
    return data_[j][i];
  }

  inline const T operator()(const int i) const {
    return ConstGetHelper(*this, i - 1);
  }

  inline T& operator()(const int i) {
    return GetHelper(*this, i - 1);
  }

  /// Access an element of the matrix.
  /// @param i The index of the elment in flattened memory.
  /// @return A const reference to the accessed data that cannot be modified
  /// by the caller.
  inline const T operator[](const int i) const {
    return ConstGetHelper(*this, i);
  }

  /// Access an element of the matrix.
  /// @param i The index of the elment in flattened memory.
  /// @return A reference to the accessed data that can be modified by the
  /// caller.
  inline T& operator[](const int i) {
    return GetHelper(*this, i);
  }

  /// Access a column vector of the matrix.
  /// @param i The index of the column.
  /// @return A reference to the accessed data that can be modified by the
  /// caller.
  inline Vector<T, rows>& GetColumn(const int i) {
    return data_[i];
  }

  /// Access a column vector of the matrix.
  /// @param i The index of the column.
  /// @return A const reference to the accessed data that cannot be modified
  /// by the caller.
  inline const Vector<T, rows>& GetColumn(const int i) const {
    return data_[i];
  }

  /// Matrix negation.
  /// @return A new matrix that stores the negation result.
  inline Matrix<T, rows, columns> operator-() const {
    GOOMATH_MAT_OPERATOR(-data_[i]);
  }

  /// Matrix addition.
  /// @param m A matrix to add this matrix with.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator+(
    const Matrix<T, rows, columns>& m) const {
    GOOMATH_MAT_OPERATOR(data_[i] + m.data_[i]);
  }

  /// Matrix subtraction.
  /// @param m A matrix to subtract this matrix with.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator-(
    const Matrix<T, rows, columns>& m) const {
    GOOMATH_MAT_OPERATOR(data_[i] - m.data_[i]);
  }

  /// Matrix/Scalar addition. Note that this is defined as addition between
  /// the origional matrix and the scalar multiplied by a matrix of ones.
  /// @param s A scalar to add to this matrix.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator+(const T& s) const {
    GOOMATH_MAT_OPERATOR(data_[i] + s);
  }

  /// Matrix/Scalar subtraction. Note that this is defined as subtraction
  /// between the origional matrix and the scalar multiplied by a matrix of
  /// ones.
  /// @param s A scalar to subtract from this matrix.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator-(const T& s) const {
    GOOMATH_MAT_OPERATOR(data_[i] - s);
  }

  /// Matrix/Scalar multiplication.
  /// @param s A scalar to multiply this matrix with.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator*(const T& s) const {
    GOOMATH_MAT_OPERATOR(data_[i] * s);
  }

  /// Matrix/Scalar division. Note that this is defined as multiplication by the
  /// inverse of the scalar.
  /// @param s A scalar to divide this matrix with.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator/(const T& s) const {
    return (*this) * (1 / s);
  }

  /// Matrix mulitplication.
  /// @param m A matrix to multiply this matrix with.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator*(
    const Matrix<T, rows, columns>& m) {
    Matrix<T, rows, columns> result;
    TimesHelper(*this, m, &result);
    return result;
  }

  /// In place matrix addition.
  /// @param m A matrix to add this matrix with.
  /// @return A reference to this class.
  inline Matrix<T, rows, columns>& operator+=(
    const Matrix<T, rows, columns>& m) {
    GOOMATH_MAT_SELF_OPERATOR(data_[i] += m.data_[i]);
  }

  /// In place matrix subtraction.
  /// @param m A matrix to subtract this matrix with.
  /// @return A reference to this class.
  inline Matrix<T, rows, columns>& operator-=(
    const Matrix<T, rows, columns>& m) {
    GOOMATH_MAT_SELF_OPERATOR(data_[i] -= m.data_[i]);
  }

  /// In place matrix/scalar addition. Note that this is defined as addition
  /// between the origional matrix and the scalar multiplied by a matrix of
  /// ones.
  /// @param s A scalar to add to this matrix.
  /// @return A reference to this class.
  inline Matrix<T, rows, columns>& operator+=(const T& s) {
    GOOMATH_MAT_SELF_OPERATOR(data_[i] += s);
  }

  /// In place matrix/scalar subtraction. Note that this is defined as
  /// subtraction between the origional matrix and the scalar multiplied by
  /// a matrix of ones.
  /// @param s A scalar to subtract from this matrix.
  /// @return A reference to this class.
  inline Matrix<T, rows, columns>& operator-=(const T& s) {
    GOOMATH_MAT_SELF_OPERATOR(data_[i] -= s);
  }

  /// In place matrix/scalar multiplication.
  /// @param s A scalar to multiply this matrix with.
  /// @return A reference to this class.
  inline Matrix<T, rows, columns>& operator*=(const T& s) {
    GOOMATH_MAT_SELF_OPERATOR(data_[i] *= s);
  }

  /// In place matrix/scalar division. Note that this is defined as
  /// multiplication by the inverse of the scalar.
  /// @param s A scalar to divide this matrix with.
  /// @return A reference to this class.
  inline Matrix<T, rows, columns>& operator/=(const T& s) {
    return (*this) *= (1 / s);
  }

  /// In place matrix mulitplication.
  /// @param m A matrix to multiply this matrix with.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns>& operator*=(
    const Matrix<T, rows, columns>& m) {
    TimesHelper(*this, m, this);
    return *this;
  }

  /// Find the inverse matrix such that m*m.Inverse() is the identity.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> Inverse() const {
    return InverseHelper(*this);
  }

  /// Find the outer product of two vectors.
  /// @return A new matrix that stores the result.
  static inline Matrix<T, rows, columns> OuterProduct(
    const Vector<T, rows>& v1, const Vector<T, columns>& v2) {
    return OuterProductHelper(v1, v2);
  }

  /// Calculate the hadamard or componentwise product of two matrices.
  /// @param m1 First matrix.
  /// @param m2 Second matrix.
  /// @return A new matrix that stores the result.
  static inline Matrix<T, rows, columns> HadamardProduct(
    const Matrix<T, rows, columns>& m1, const Matrix<T, rows, columns>& m2) {
    GOOMATH_MAT_OPERATOR(m1[i] * m2[i]);
  }

  /// Calculate the identity matrix.
  /// @return A new matrix that stores the result.
  static inline Matrix<T, rows, columns> Identity() {
    return IdentityHelper<T, rows, columns>();
  }

  /// Create a 4x4 matrix from a Vector of size 3. This matrix will have an
  /// empty or zero rotation component.
  /// @param m The vector of size 3.
  /// @return A new matrix that stores the result.
  static inline Matrix<T, 4> FromTranslationVector(const Vector<T, 3>& v) {
    return Matrix<T, 4>(
      1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, v[0], v[1], v[2], 1);
  }

  /// Create a 4x4 matrix from a 3x3 rotation matrix. This matrix will have an
  /// empty or zero translation component.
  /// @param m The 3x3 rotation matrix.
  /// @return A new matrix that stores the result.
  static inline Matrix<T, 4> FromRotationMatrix(const Matrix<T, 3>& m) {
    return Matrix<T, 4>(
      m[0], m[1], m[2], 0, m[3], m[4], m[5], 0,
      m[6], m[7], m[8], 0, 0, 0, 0, 1);
  }

  /// Vector/Matrix multiplication.
  /// @param v The vector to use for mulitplication.
  /// @param m The matrix to use for multiplication.
  /// @return A new vector that stores the result.
  friend inline Vector<T, columns> operator*(
    const Vector<T, rows>& v, const Matrix<T, rows, columns>& m) {
    const int d = columns;
    GOOMATH_VEC_OPERATOR((Vector<T, rows>::DotProduct(m.data_[i], v)));
  }

 private:
  Vector<T, rows> data_[columns];
};

// Scalar/Matrix multiplication
template<class T, int rows, int columns>
inline Matrix<T, rows, columns> operator*(
  const T& s, const Matrix<T, columns, rows>& m) {
  return m * s;
}

// Matrix/Vector multiplication. Template specialized versions are
// implemented for 2x2, 3x3, and 4x4 matrices to increase performance.
// 3x3 float is also specialized because if SIMD is used the vectors of
// this type of length 4.
template<class T, int rows, int columns>
inline Vector<T, rows> operator*(
  const Matrix<T, rows, columns>& m, const Vector<T, columns>& v) {
  const Vector<T, rows> result(0);
  int offset = 0;
  for (int column = 0; column < columns; column++) {
    for (int row = 0; row < rows; row++) {
      result[row] += m[offset + row] * v[column];
    }
    offset += rows;
  }
  return result;
}

template<class T>
inline Vector<T, 2> operator*(
  const Matrix<T, 2, 2>& m, const Vector<T, 2>& v) {
  const T* data1 = CAST<const T*>(&m);
    return Vector<T, 2>(
      data1[0] * v[0] + data1[2] * v[1], data1[1] * v[0] + data1[3] * v[1]);
}

template<class T>
inline Vector<T, 3> operator*(
  const Matrix<T, 3, 3>& m, const Vector<T, 3>& v) {
  const T* data1 = CAST<const T*>(&m);
  return Vector<T, 3>(
    GOOMATH_MATRIX_3X3_DOT(data1, v, 0, 3),
    GOOMATH_MATRIX_3X3_DOT(data1, v, 1, 3),
    GOOMATH_MATRIX_3X3_DOT(data1, v, 2, 3));
}

template<>
inline Vector<float, 3> operator*(
  const Matrix<float, 3, 3>& m, const Vector<float, 3>& v) {
  const float* data1 = CAST<const float*>(&m);
  return Vector<float, 3>(
    GOOMATH_MATRIX_3X3_DOT(data1, v, 0, VEC3_SIZE),
    GOOMATH_MATRIX_3X3_DOT(data1, v, 1, VEC3_SIZE),
    GOOMATH_MATRIX_3X3_DOT(data1, v, 2, VEC3_SIZE));
}

template<class T>
inline Vector<T, 4> operator*(
  const Matrix<T, 4, 4>& m, const Vector<T, 4>& v) {
  const T* data1 = CAST<const T*>(&m);
  return Vector<T, 4>(
    GOOMATH_MATRIX_4X4_DOT(data1, v, 0),
    GOOMATH_MATRIX_4X4_DOT(data1, v, 1),
    GOOMATH_MATRIX_4X4_DOT(data1, v, 2),
    GOOMATH_MATRIX_4X4_DOT(data1, v, 3));
}

// Matrix/Vector multiplication of a 4x4 matrix with a vector of size 3.
// This is provided as a convenience and assumes the vector has a fourth
// component equal to 1.
template<class T>
inline Vector<T, 3> operator*(
  const Matrix<T, 4, 4>& m, const Vector<T, 3>& v) {
  Vector<T, 4> v4(v[0], v[1], v[2], 1);
  v4 = m * v4;
  return Vector<T, 3>(v4[0] / v4[3], v4[1] / v4[3], v4[2] / v4[3]);
}

// Matrix/Matrix Multiplication. Template specialized versions are implemented
// for 2x2, 3x3, and 4x4 matrices to improve performance. 3x3 float is also
// specialized because if SIMD is used the vectors of this type of length 4.
template<class T, int size1, int size2, int size3>
inline void TimesHelper(
  const Matrix<T, size1, size2>& m1, const Matrix<T, size2, size3>& m2,
  Matrix<T, size1, size3>* out_m) {
  for(int i = 0; i < size1; i++) {
    for(int j = 0; j < size3; j++) {
      Vector<T, size2> row;
      for(int k = 0; k < size2; k++) {
        row[k] = m1(i, k);
      }
      (*out_m)(i, j) = Vector<T, size2>::DotProduct(m2.GetColumn(j), row);
    }
  }
}

template<class T>
inline void TimesHelper(
  const Matrix<T, 2, 2>& m1, const Matrix<T, 2, 2>& m2,
  Matrix<T, 2, 2>* out_m) {
  T* data_out = CAST<T*>(out_m);
  const T* data1 = CAST<const T*>(&m1);
  const T* data2 = CAST<const T*>(&m2);
  data_out[0] = data1[0] * data2[0] + data1[2] * data2[1];
  data_out[1] = data1[1] * data2[0] + data1[3] * data2[1];
  data_out[2] = data1[0] * data2[2] + data1[2] * data2[3];
  data_out[3] = data1[1] * data2[2] + data1[3] * data2[3];
}

template<class T>
inline void TimesHelper(
  const Matrix<T, 3, 3>& m1, const Matrix<T, 3, 3>& m2,
  Matrix<T, 3, 3>* out_m) {
  T* data_out = CAST<T*>(out_m);
  const T* data1 = CAST<const T*>(&m1);
  {Vector<T, 3> row(data1[0], data1[3], data1[6]);
  data_out[0] = Vector<T, 3>::DotProduct(m2.GetColumn(0),row);
  data_out[3] = Vector<T, 3>::DotProduct(m2.GetColumn(1),row);
  data_out[6] = Vector<T, 3>::DotProduct(m2.GetColumn(2),row);}
  {Vector<T, 3> row(data1[1], data1[4], data1[7]);
  data_out[1] = Vector<T, 3>::DotProduct(m2.GetColumn(0),row);
  data_out[4] = Vector<T, 3>::DotProduct(m2.GetColumn(1),row);
  data_out[7] = Vector<T, 3>::DotProduct(m2.GetColumn(2),row);}
  {Vector<T, 3> row(data1[2], data1[5], data1[8]);
  data_out[2] = Vector<T, 3>::DotProduct(m2.GetColumn(0),row);
  data_out[5] = Vector<T, 3>::DotProduct(m2.GetColumn(1),row);
  data_out[8] = Vector<T, 3>::DotProduct(m2.GetColumn(2),row);}
}

template<>
inline void TimesHelper(
  const Matrix<float, 3, 3>& m1, const Matrix<float, 3, 3>& m2,
  Matrix<float, 3, 3>* out_m) {
  typedef float T;
  T* data_out = CAST<T*>(out_m);
  const T* data1 = CAST<const T*>(&m1);
  {Vector<T, 3> row(data1[0], data1[VEC3_SIZE], data1[2 * VEC3_SIZE]);
  data_out[0] = Vector<T, 3>::DotProduct(m2.GetColumn(0),row);
  data_out[VEC3_SIZE] = Vector<T, 3>::DotProduct(m2.GetColumn(1),row);
  data_out[2 * VEC3_SIZE] = Vector<T, 3>::DotProduct(m2.GetColumn(2),row);}
  {Vector<T, 3> row(data1[1], data1[1 + VEC3_SIZE], data1[1 + 2 * VEC3_SIZE]);
  data_out[1] = Vector<T, 3>::DotProduct(m2.GetColumn(0),row);
  data_out[1 + VEC3_SIZE] = Vector<T, 3>::DotProduct(m2.GetColumn(1),row);
  data_out[1 + 2 * VEC3_SIZE] = Vector<T, 3>::DotProduct(m2.GetColumn(2),row);}
  {Vector<T, 3> row(data1[2], data1[2 + VEC3_SIZE], data1[2 + 2 * VEC3_SIZE]);
  data_out[2] = Vector<T, 3>::DotProduct(m2.GetColumn(0),row);
  data_out[2 + VEC3_SIZE] = Vector<T, 3>::DotProduct(m2.GetColumn(1),row);
  data_out[2 + 2 * VEC3_SIZE] = Vector<T, 3>::DotProduct(m2.GetColumn(2),row);}
}

template<class T>
inline void TimesHelper(
  const Matrix<T, 4, 4>& m1, const Matrix<T, 4, 4>& m2,
  Matrix<T, 4, 4>* out_m) {
  T* data_out = CAST<T*>(out_m);
  const T* data1 = CAST<const T*>(&m1);
  {Vector<T, 4> row(data1[0], data1[4], data1[8], data1[12]);
  data_out[0] = Vector<T, 4>::DotProduct(m2.GetColumn(0),row);
  data_out[4] = Vector<T, 4>::DotProduct(m2.GetColumn(1),row);
  data_out[8] = Vector<T, 4>::DotProduct(m2.GetColumn(2),row);
  data_out[12] = Vector<T, 4>::DotProduct(m2.GetColumn(3),row);}
  {Vector<T, 4> row(data1[1], data1[5], data1[9], data1[13]);
  data_out[1] = Vector<T, 4>::DotProduct(m2.GetColumn(0),row);
  data_out[5] = Vector<T, 4>::DotProduct(m2.GetColumn(1),row);
  data_out[9] = Vector<T, 4>::DotProduct(m2.GetColumn(2),row);
  data_out[13] = Vector<T, 4>::DotProduct(m2.GetColumn(3),row);}
  {Vector<T, 4> row(data1[2], data1[6], data1[10], data1[14]);
  data_out[2] = Vector<T, 4>::DotProduct(m2.GetColumn(0),row);
  data_out[6] = Vector<T, 4>::DotProduct(m2.GetColumn(1),row);
  data_out[10] = Vector<T, 4>::DotProduct(m2.GetColumn(2),row);
  data_out[14] = Vector<T, 4>::DotProduct(m2.GetColumn(3),row);}
  {Vector<T, 4> row(data1[3], data1[7], data1[11], data1[15]);
  data_out[3] = Vector<T, 4>::DotProduct(m2.GetColumn(0),row);
  data_out[7] = Vector<T, 4>::DotProduct(m2.GetColumn(1),row);
  data_out[11] = Vector<T, 4>::DotProduct(m2.GetColumn(2),row);
  data_out[15] = Vector<T, 4>::DotProduct(m2.GetColumn(3),row);}
}

// Access an element of a matrix. There is a specialization for 3x3
// float matrices as the column vectors could be of length 4.
template<class T, int rows, int columns>
inline T& GetHelper(Matrix<T, rows, columns>& m, const int i) {
  return *(CAST<T*>(&m) + i);
}

template<class T, int rows, int columns>
inline const T& ConstGetHelper(const Matrix<T, rows, columns>& m, const int i) {
  return *(CAST<const T*>(&m) + i);
}

#if defined(COMPILE_WITH_SIMD) && defined(COMPILE_WITH_PADDING)
template<int columns>
inline float& GetHelper(Matrix<float, 3, columns>& m, const int i) {
  const int index = 4 * (i / 3) + i % 3;
  return *(CAST<float*>(&m) + index);
}

template<int columns>
inline const float& ConstGetHelper(const Matrix<float, 3, columns>& m, const int i) {
  const int index = 4 * (i / 3) + i % 3;
  return *(CAST<const float*>(&m) + index);
}
#endif

// Compute the identity matrix. There is template specialization for 2x2, 3x3,
// and 4x4 matrices to increase performance.
template<class T, int rows, int columns>
inline Matrix<T, rows, columns> IdentityHelper() {
  Matrix<T, rows, columns> return_matrix(0.f);
  int min_d = rows < columns ? rows : columns;
  for (int i = 0; i < min_d; ++i) return_matrix(i,i) = 1;
  return return_matrix;
}

template<class T>
inline Matrix<T, 2, 2> IdentityHelper() {
  return Matrix<T, 2, 2>(1, 0, 0, 1);
}

template<class T>
inline Matrix<T, 3, 3> IdentityHelper() {
  return Matrix<T, 3, 3>(1, 0, 0, 0, 1, 0, 0, 0, 1);
}

template<class T>
inline Matrix<T, 4, 4> IdentityHelper() {
  return Matrix<T, 4, 4>(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

// Compute the outer product of two vectors. There is template specialization
// for 2x2, 3x3, and 4x4 matrices to increase performance.
template<class T, int rows, int columns>
static inline Matrix<T, rows, columns> OuterProductHelper(
  const Vector<T, rows>& v1, const Vector<T, columns>& v2) {
  Matrix<T, rows, columns> result(0);
  int offset = 0;
  for (int column = 0; column < columns; column++) {
    for (int row = 0; row < rows; row++) {
      result[row + offset] = v1[row] * v2[column];
    }
    offset += rows;
  }
  return result;
}

template<class T>
static inline Matrix<T, 2, 2> OuterProductHelper(
  const Vector<T, 2>& v1, const Vector<T, 2>& v2) {
  return Matrix<T, 2, 2>(
    v1[0] * v2[0], v1[1] * v2[0], v1[0] * v2[1], v1[1] * v2[1]);
}

template<class T>
static inline Matrix<T, 3, 3> OuterProductHelper(
  const Vector<T, 3>& v1, const Vector<T, 3>& v2) {
  return Matrix<T, 3, 3>(
    v1[0] * v2[0], v1[1] * v2[0], v1[2] * v2[0],
    v1[0] * v2[1], v1[1] * v2[1], v1[2] * v2[1],
    v1[0] * v2[2], v1[1] * v2[2], v1[2] * v2[2]);
}

template<class T>
static inline Matrix<T, 4, 4> OuterProductHelper(
  const Vector<T, 4>& v1, const Vector<T, 4>& v2) {
  return Matrix<T, 4, 4>(
    v1[0] * v2[0], v1[1] * v2[0], v1[2] * v2[0], v1[3] * v2[0],
    v1[0] * v2[1], v1[1] * v2[1], v1[2] * v2[1], v1[3] * v2[1],
    v1[0] * v2[2], v1[1] * v2[2], v1[2] * v2[2], v1[3] * v2[2],
    v1[0] * v2[3], v1[1] * v2[3], v1[2] * v2[3], v1[3] * v2[3]);
}

// Compute the inverse of a matrix. There is template specialization
// for 2x2, 3x3, and 4x4 matrices to increase performance. Inverse
// is not implemented for dense matrices that are not of size 2x2,
// 3x3, and 4x4.
template<class T, int rows, int columns>
inline Matrix<T, rows, columns> InverseHelper(
  const Matrix<T, rows, columns>& m) {
  STATIC_ASSERT(false);
  return Matrix<T, rows, columns>::Identity();
}

template<class T>
inline Matrix<T, 2, 2> InverseHelper(const Matrix<T, 2, 2>& m) {
  T inverseDeterminant = 1 / (m[0] * m[3] - m[1] * m[2]);
  return Matrix<T, 2, 2>(
    inverseDeterminant * m[3], -inverseDeterminant * m[1],
    -inverseDeterminant * m[2], inverseDeterminant * m[0]);
}

template<class T>
inline Matrix<T, 3, 3> InverseHelper(const Matrix<T, 3, 3>& m) {
  //Find determinant of matrix.
  T sub11 = m[4] * m[8] - m[5] * m[7],
    sub12 = -m[1] * m[8] + m[2] * m[7],
    sub13 = m[1] * m[5] - m[2] * m[4];
  T det = m[0] * sub11 + m[3] * sub12 + m[6] * sub13;
  //Find determinants of 2x2 submatrices for the elements of the inverse.
  return 1 / det * Matrix<T, 3, 3>(
    sub11, sub12, sub13, m[6] * m[5] - m[3] * m[8],
    m[0] * m[8] - m[6] * m[2],
    m[3] * m[2] - m[0] * m[5],
    m[3] * m[7] - m[6] * m[4],
    m[6] * m[1] - m[0] * m[7],
    m[0] * m[4] - m[3] * m[1]);
}

template<class T>
inline int FindLargestPivotElem(const Matrix<T, 4, 4>& m) {
  if (m[0] > m[1]) {
    if (m[0] > m[2]) {
      if (m[0] > m[3]) {
        return 0;
      }
      else {
        return 3;
      }
    } else if (m[2] > m[3]) {
      return 2;
    }
    else {
      return 3;
    }
  } else if (m[1] > m[2]) {
    if (m[1] > m[3]) {
      return 1;
    }
    else {
      return 3;
    }
  } else if (m[2] > m[3]) {
    return 2;
  }
  else {
    return 3;
  }
  return 0;
}

template<class T>
Matrix<T, 4, 4> InverseHelper(const Matrix<T, 4, 4>& m) {
  // This will find the pivot element.
  int pivot_elem = FindLargestPivotElem(m);
  // This will perform the pivot and find the row, column, and 3x3 submatrix
  // for this pivot.
  Vector<T, 3> row, column;
  Matrix<T, 3> matrix;
  if (pivot_elem == 0) {
    row = Vector<T, 3>(m[4], m[8], m[12]);
    column = Vector<T, 3>(m[1], m[2], m[3]);
    matrix = Matrix<T, 3>(
      m[5], m[6], m[7], m[9], m[10],
      m[11], m[13], m[14], m[15]);
  } else if (pivot_elem == 1) {
    row = Vector<T, 3>(m[5], m[9], m[13]);
    column = Vector<T, 3>(m[0], m[2], m[3]);
    matrix = Matrix<T, 3>(
      m[4], m[6], m[7], m[8], m[10],
      m[11], m[12], m[14], m[15]);
  } else if (pivot_elem == 2) {
    row = Vector<T, 3>(m[6], m[10], m[14]);
    column = Vector<T, 3>(m[0], m[1], m[3]);
    matrix = Matrix<T, 3>(
      m[4], m[5], m[7], m[8], m[9],
      m[11], m[12], m[13], m[15]);
  } else {
    row = Vector<T, 3>(m[7], m[11], m[15]);
    column = Vector<T, 3>(m[0], m[1], m[2]);
    matrix = Matrix<T, 3>(
      m[4], m[5], m[6], m[8], m[9],
      m[10], m[12], m[13], m[14]);
  }
  // This will compute the inverse using the row, column, and 3x3 submatrix.
  T inv = -1 / m[pivot_elem];
  row *= inv;
  matrix += Matrix<T, 3>::OuterProduct(column, row);
  Matrix<T, 3> mat_inverse = matrix.Inverse();
  Vector<T, 3> col_inverse = mat_inverse * (column * inv);
  Vector<T, 3> row_inverse = row * mat_inverse;
  T pivot_inverse = Vector<T, 3>::DotProduct(row, col_inverse) - inv;
  if (pivot_elem == 0) {
    return Matrix<T, 4, 4>(
      pivot_inverse, col_inverse[0], col_inverse[1], col_inverse[2],
      row_inverse[0], mat_inverse[0], mat_inverse[1], mat_inverse[2],
      row_inverse[1], mat_inverse[3], mat_inverse[4], mat_inverse[5],
      row_inverse[2], mat_inverse[6], mat_inverse[7], mat_inverse[8]);
  } else if (pivot_elem == 1) {
    return Matrix<T, 4, 4>(
      row_inverse[0], mat_inverse[0], mat_inverse[1], mat_inverse[2],
      pivot_inverse, col_inverse[0], col_inverse[1], col_inverse[2],
      row_inverse[1], mat_inverse[3], mat_inverse[4], mat_inverse[5],
      row_inverse[2], mat_inverse[6], mat_inverse[7], mat_inverse[8]);
  } else if (pivot_elem == 2) {
    return Matrix<T, 4, 4>(
      row_inverse[0], mat_inverse[0], mat_inverse[1], mat_inverse[2],
      row_inverse[1], mat_inverse[3], mat_inverse[4], mat_inverse[5],
      pivot_inverse, col_inverse[0], col_inverse[1], col_inverse[2],
      row_inverse[2], mat_inverse[6], mat_inverse[7], mat_inverse[8]);
  } else {
    return Matrix<T, 4, 4>(
      row_inverse[0], mat_inverse[0], mat_inverse[1], mat_inverse[2],
      row_inverse[1], mat_inverse[3], mat_inverse[4], mat_inverse[5],
      row_inverse[2], mat_inverse[6], mat_inverse[7], mat_inverse[8],
      pivot_inverse, col_inverse[0], col_inverse[1], col_inverse[2]);
  }
  return Matrix<T, 4, 4>::Identity();
}

}  // namespace goomath
#endif  // GOOMATH_MATRICES_MATRIX_
