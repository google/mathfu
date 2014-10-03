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
#ifndef MATHFU_MATRIX_H_
#define MATHFU_MATRIX_H_

#include "mathfu/vector.h"
#include "mathfu/vector_2.h"
#include "mathfu/vector_3.h"
#include "mathfu/vector_4.h"
#include "mathfu/utilities.h"

#include <cmath>

#include <assert.h>

#ifdef _MSC_VER
  #pragma warning(push)
  // The following disables warnings for MATHFU_MAT_OPERATION.
  // The buffer overrun warning must be disabled as MSVC doesn't treat
  // "columns" as constant and therefore assumes that it's possible
  // to overrun arrays indexed by "i".
  // The conditional expression is constant warning is disabled since
  // MSVC decides that "columns" *is* constant when unrolling the operation
  // loop.
  #pragma warning(disable:4127) // conditional expression is constant
  #pragma warning(disable:4789) // buffer overrun
#endif

// The stride of a vector (e.g Vector<T, 3>) when cast as an array of floats.
#define MATHFU_VECTOR_STRIDE_FLOATS(vector) (sizeof(vector) / sizeof(float))

// This will unroll loops for matrices with <= 4 columns
#define MATHFU_MAT_OPERATION(OP) MATHFU_UNROLLED_LOOP(i, columns, OP)

// This will perform a given OP on each matrix column and return the result
#define MATHFU_MAT_OPERATOR(OP) \
  { \
    Matrix<T, rows, columns> result; \
    MATHFU_MAT_OPERATION(result.data_[i] = (OP)); \
    return result; \
  }

// This will perform a given OP on each matrix column
#define MATHFU_MAT_SELF_OPERATOR(OP) \
  { \
    MATHFU_MAT_OPERATION(OP); \
    return *this; \
  }

// This macro will take the dot product for a row from data1 and a column from
// data2.
#define MATHFU_MATRIX_4X4_DOT(data1, data2, r) \
  ((data1)[r] * (data2)[0] + \
   (data1)[(r) + 4] * (data2)[1] + \
   (data1)[(r) + 8] * (data2)[2] + \
   (data1)[(r) + 12] * (data2)[3])

#define MATHFU_MATRIX_3X3_DOT(data1, data2, r, size) \
  ((data1)[r] * (data2)[0] + \
   (data1)[(r) + (size)] * (data2)[1] +   \
   (data1)[(r) + 2 * (size)] * (data2)[2])

namespace mathfu {

template<class T, int rows, int columns = rows> class Matrix;
template<class T, int rows, int columns>
inline Matrix<T, rows, columns> IdentityHelper();
template<bool check_invertible, class T, int rows, int columns>
inline bool InverseHelper(const Matrix<T, rows, columns>& m,
                          Matrix<T, rows, columns>* const inverse);
template<class T, int rows, int columns>
inline void TimesHelper(const Matrix<T, rows, columns>& m1,
                        const Matrix<T, rows, columns>& m2,
                        Matrix<T, rows, columns>* out_m);
template<class T, int rows, int columns>
static inline Matrix<T, rows, columns> OuterProductHelper(
    const Vector<T, rows>& v1, const Vector<T, columns>& v2);
template<class T>
inline Matrix<T, 4, 4> PerspectiveHelper(T fovy, T aspect, T znear, T zfar,
                                         T handedness);
template<class T>
static inline Matrix<T, 4, 4> OrthoHelper(T left, T right, T bottom, T top,
                                          T znear, T zfar);
template<class T>
static inline Matrix<T, 4, 4> LookAtHelper(
    const Vector<T, 3>& at, const Vector<T, 3>& eye, const Vector<T, 3>& up);


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
    MATHFU_MAT_OPERATION(data_[i] = m.data_[i]);
  }

  /// Create a matrix from a single float. Each elements is set to be equal to
  /// the value given.
  /// @param s Scalar value that the matrix will be initialized to.
  explicit inline Matrix(const T& s) {
    MATHFU_MAT_OPERATION((data_[i] = Vector<T, rows>(s)));
  }

  /// Create a matrix from four floats. This method only works for a 2x2
  /// matrix.
  /// @param s00 A scalar for the element in the first row and column.
  /// @param s10 A scalar for the element in the second row, first column.
  /// @param s01 A scalar for the element in the first row, second column.
  /// @param s11 A scalar for the element in the second row and column.
  inline Matrix(const T& s00, const T& s10, const T& s01, const T& s11) {
    MATHFU_STATIC_ASSERT(rows == 2 && columns == 2);
    data_[0] = Vector<T, rows>(s00, s10);
    data_[1] = Vector<T, rows>(s01, s11);
  }

  /// Create a matrix from nine floats. This method only works for a 3x3
  /// matrix.
  /// @param s00 A scalar for the element in the first row and column.
  /// @param s10 A scalar for the element in the second row, first column.
  /// @param s20 A scalar for the element in the third row, first column.
  /// @param s01 A scalar for the element in the first row, second column.
  /// @param s11 A scalar for the element in the second row and column.
  /// @param s21 A scalar for the element in the third row, second column.
  /// @param s02 A scalar for the element in the first row, third column.
  /// @param s12 A scalar for the element in the second row, third column.
  /// @param s22 A scalar for the element in the third row and column.
  inline Matrix(const T& s00, const T& s10, const T& s20,
                const T& s01, const T& s11, const T& s21,
                const T& s02, const T& s12, const T& s22) {
    MATHFU_STATIC_ASSERT(rows == 3 && columns == 3);
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
  inline Matrix(const T& s00, const T& s10, const T& s20, const T& s30,
                const T& s01, const T& s11, const T& s21, const T& s31,
                const T& s02, const T& s12, const T& s22, const T& s32,
                const T& s03, const T& s13, const T& s23, const T& s33) {
    MATHFU_STATIC_ASSERT(rows == 4 && columns == 4);
    data_[0] = Vector<T, rows>(s00, s10, s20, s30);
    data_[1] = Vector<T, rows>(s01, s11, s21, s31);
    data_[2] = Vector<T, rows>(s02, s12, s22, s32);
    data_[3] = Vector<T, rows>(s03, s13, s23, s33);
  }

  /// Create 4x4 matrix from 4, 4 element vectors.
  /// @param column0 Vector used for the first column.
  /// @param column1 Vector used for the second column.
  /// @param column2 Vector used for the third column.
  /// @param column3 Vector used for the fourth column.
  inline Matrix(const Vector<T, 4>& column0, const Vector<T, 4>& column1,
                const Vector<T, 4>& column2, const Vector<T, 4>& column3) {
    MATHFU_STATIC_ASSERT(rows == 4 && columns == 4);
    data_[0] = column0;
    data_[1] = column1;
    data_[2] = column2;
    data_[3] = column3;
  }

  /// Create a matrix form the first row*column elements of an array.
  /// @param a Array of values that the matrix will be iniitlized to.
  explicit inline Matrix(const T* a) {
    MATHFU_MAT_OPERATION((data_[i] = Vector<T, rows>(&a[i*columns])));
  }

  /// Create a matrix from an array of "columns", "rows" element packed
  /// vectors.
  /// @param vectors Array of "columns", "rows" element packed vectors.
  explicit inline Matrix(const VectorPacked<T, rows> * const vectors) {
    MATHFU_MAT_OPERATION((data_[i] = Vector<T, rows>(vectors[i])));
  }

  /// Access an element of the matrix.
  /// @param i The index of the row where the element is located.
  /// @param j The index of the column where the element is located.
  /// @return A const reference to the accessed data that cannot be modified
  /// by the caller.
  inline const T& operator()(const int i, const int j) const {
    return data_[j][i];
  }

  /// Access an element of the matrix.
  /// @param i The index of the row where the element is located.
  /// @param j The index of the column where the element is located.
  /// @return A reference to the accessed data that can be modified by the
  /// caller.
  inline T& operator()(const int i, const int j) {
    return data_[j][i];
  }

  inline const T &operator()(const int i) const {
    return operator[](i);
  }

  inline T& operator()(const int i) {
    return operator[](i);
  }

  /// Access an element of the matrix.
  /// @param i The index of the element in flattened memory.
  /// @return A const reference to the accessed data that cannot be modified
  /// by the caller.
  inline const T &operator[](const int i) const {
    return const_cast<Matrix<T, rows, columns>* >(this)->operator[](i);
  }

  /// Access an element of the matrix.
  /// @param i The index of the element in flattened memory.
  /// @return A reference to the accessed data that can be modified by the
  /// caller.
  inline T& operator[](const int i) {
#if defined(MATHFU_COMPILE_WITH_PADDING)
    // In this case Vector<T, 3> is padded, so the element offset must be
    // accessed using the array operator.
    if (rows == 3) {
      const int row = i % rows;
      const int col = i / rows;
      return data_[col][row];
    } else {
      return reinterpret_cast<T*>(data_)[i];
    }
#else
    return reinterpret_cast<T*>(data_)[i];
#endif  // defined(MATHFU_COMPILE_WITH_PADDING)
  }

  /// Pack the matrix to an array of "rows" element vectors,
  /// one vector per matrix column.
  /// @param vector Array of "columns" in size to write to.
  inline void Pack(VectorPacked<T, rows> * const vector) const {
    MATHFU_MAT_OPERATION(GetColumn(i).Pack(&vector[i]));
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
    MATHFU_MAT_OPERATOR(-data_[i]);
  }

  /// Matrix addition.
  /// @param m A matrix to add this matrix with.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator+(
      const Matrix<T, rows, columns>& m) const {
    MATHFU_MAT_OPERATOR(data_[i] + m.data_[i]);
  }

  /// Matrix subtraction.
  /// @param m A matrix to subtract this matrix with.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator-(
      const Matrix<T, rows, columns>& m) const {
    MATHFU_MAT_OPERATOR(data_[i] - m.data_[i]);
  }

  /// Matrix/Scalar addition. Note that this is defined as addition between
  /// the origional matrix and the scalar multiplied by a matrix of ones.
  /// @param s A scalar to add to this matrix.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator+(const T& s) const {
    MATHFU_MAT_OPERATOR(data_[i] + s);
  }

  /// Matrix/Scalar subtraction. Note that this is defined as subtraction
  /// between the origional matrix and the scalar multiplied by a matrix of
  /// ones.
  /// @param s A scalar to subtract from this matrix.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator-(const T& s) const {
    MATHFU_MAT_OPERATOR(data_[i] - s);
  }

  /// Matrix/Scalar multiplication.
  /// @param s A scalar to multiply this matrix with.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator*(const T& s) const {
    MATHFU_MAT_OPERATOR(data_[i] * s);
  }

  /// Matrix/Scalar division. Note that this is defined as multiplication by
  /// the inverse of the scalar.
  /// @param s A scalar to divide this matrix with.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator/(const T& s) const {
    return (*this) * (1 / s);
  }

  /// Matrix mulitplication.
  /// @param m A matrix to multiply this matrix with.
  /// @return A new matrix that stores the result.
  inline Matrix<T, rows, columns> operator*(
      const Matrix<T, rows, columns>& m) const {
    Matrix<T, rows, columns> result;
    TimesHelper(*this, m, &result);
    return result;
  }

  /// In place matrix addition.
  /// @param m A matrix to add this matrix with.
  /// @return A reference to this class.
  inline Matrix<T, rows, columns>& operator+=(
      const Matrix<T, rows, columns>& m) {
    MATHFU_MAT_SELF_OPERATOR(data_[i] += m.data_[i]);
  }

  /// In place matrix subtraction.
  /// @param m A matrix to subtract this matrix with.
  /// @return A reference to this class.
  inline Matrix<T, rows, columns>& operator-=(
      const Matrix<T, rows, columns>& m) {
    MATHFU_MAT_SELF_OPERATOR(data_[i] -= m.data_[i]);
  }

  /// In place matrix/scalar addition. Note that this is defined as addition
  /// between the origional matrix and the scalar multiplied by a matrix of
  /// ones.
  /// @param s A scalar to add to this matrix.
  /// @return A reference to this class.
  inline Matrix<T, rows, columns>& operator+=(const T& s) {
    MATHFU_MAT_SELF_OPERATOR(data_[i] += s);
  }

  /// In place matrix/scalar subtraction. Note that this is defined as
  /// subtraction between the origional matrix and the scalar multiplied by
  /// a matrix of ones.
  /// @param s A scalar to subtract from this matrix.
  /// @return A reference to this class.
  inline Matrix<T, rows, columns>& operator-=(const T& s) {
    MATHFU_MAT_SELF_OPERATOR(data_[i] -= s);
  }

  /// In place matrix/scalar multiplication.
  /// @param s A scalar to multiply this matrix with.
  /// @return A reference to this class.
  inline Matrix<T, rows, columns>& operator*=(const T& s) {
    MATHFU_MAT_SELF_OPERATOR(data_[i] *= s);
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
    Matrix<T, rows, columns> inverse;
    InverseHelper<false>(*this, &inverse);
    return inverse;
  }

  /// Find the inverse matrix such that m*m.Inverse() is the identity returning
  /// whether the matrix is invertible.
  /// The invertible check simply compares the calculated determinant with
  /// Constants<T>::GetDeterminantThreshold() to roughly determine whether the
  /// matrix is invertible.  This simple check works in common cases but will
  /// fail for corner cases where the matrix is a combination of huge and tiny
  /// values that can't be accurately represented by the floating point
  /// datatype T.  More extensive checks (relative to the input values) are
  /// possible but *far* more expensive, complicated and difficult to test.
  /// @return Whether the matrix is invertible.
  inline bool InverseWithDeterminantCheck(
      Matrix<T, rows, columns>* const inverse) const {
    return InverseHelper<true>(*this, inverse);
  }

  /// Calculate the transpose of matrix.
  /// @return The transpose of the specified matrix.
  inline Matrix<T, columns, rows> Transpose() const {
    Matrix<T, columns, rows> transpose;
    MATHFU_UNROLLED_LOOP(i, columns, MATHFU_UNROLLED_LOOP(
        j, rows, transpose.GetColumn(j)[i] = GetColumn(i)[j]))
    return transpose;
  }

  /// Return the 2 dimensional translation of a 2 dimensional affine transform.
  /// Note: 2 dimensional affine transforms are represented by 3x3 matrices.
  /// @return A new Vector with the first two components of column 2.
  inline Vector<T, 2> TranslationVector2D() const {
    MATHFU_STATIC_ASSERT(rows == 3 && columns == 3);
    return Vector<T, 2>(data_[2][0], data_[2][1]);
  }

  /// Return the 3 dimensional translation of a 3 dimensional affine transform.
  /// Note: 3 dimensional affine transforms are represented by 4x4 matrices.
  /// @return A new Vector with the first three components of column 3.
  inline Vector<T, 3> TranslationVector3D() const {
    MATHFU_STATIC_ASSERT(rows == 4 && columns == 4);
    return Vector<T, 3>(data_[3][0], data_[3][1], data_[3][2]);
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
    MATHFU_MAT_OPERATOR(m1[i] * m2[i]);
  }

  /// Calculate the identity matrix.
  /// @return A new matrix that stores the result.
  static inline Matrix<T, rows, columns> Identity() {
    return IdentityHelper<T, rows, columns>();
  }

  /// Create a 3x3 matrix from a Vector of size 2. This matrix will have an
  /// empty or zero rotation component.
  /// @param m The vector of size 2.
  /// @return A new matrix that stores the result.
  static inline Matrix<T, 3> FromTranslationVector(const Vector<T, 2>& v) {
    return Matrix<T, 3>(
      1, 0, 0, 0, 1, 0, v[0], v[1], 1);
  }

  /// Create a 4x4 matrix from a Vector of size 3. This matrix will have an
  /// empty or zero rotation component.
  /// @param m The vector of size 3.
  /// @return A new matrix that stores the result.
  static inline Matrix<T, 4> FromTranslationVector(const Vector<T, 3>& v) {
    return Matrix<T, 4>(
        1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, v[0], v[1], v[2], 1);
  }

  /// Create a square matrix with the diagonal component set to v.
  /// This is an affine transform matrix, so the dimension of the vector is
  /// one less than the dimension of the matrix.
  /// @param v The vector containing components for scaling.
  /// @return A new matrix with v along the diagonal, and 1 in the bottom
  /// right.
  static inline Matrix<T, rows> FromScaleVector(const Vector<T, rows - 1>& v) {
    // TODO OPT: Use a helper function in a similar way to Identity to
    // construct the matrix for the specialized cases 2, 3, 4, and only run
    // this method in the general case. This will also allow you to use the
    // helper methods from specialized classes like Matrix<T, 4, 4>.
    Matrix<T, rows> return_matrix(Identity());
    for (int i = 0; i < rows - 1; ++i) return_matrix(i,i) = v[i];
    return return_matrix;
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


  /// Create a 3x3 rotation matrix from a 2D normalized directional vector
  /// around the X axis.
  /// @param v The 2D normalized directional vector.
  /// @return A new matrix that stores the result.
  static inline Matrix<T, 3> RotationX(const Vector<T, 2>& v) {
    return Matrix<T, 3>(
        1, 0,     0,
        0, v.x(), v.y(),
        0,-v.y(), v.x());
  }

  /// Create a 3x3 rotation matrix from a 2D normalized directional vector
  /// around the Y axis.
  /// @param v The 2D normalized directional vector.
  /// @return A new matrix that stores the result.
  static inline Matrix<T, 3> RotationY(const Vector<T, 2>& v) {
    return Matrix<T, 3>(
        v.x(), 0,-v.y(),
        0,     1, 0,
        v.y(), 0, v.x());
  }

  /// Create a 3x3 rotation matrix from a 2D normalized directional vector
  /// around the Z axis.
  /// @param v The 2D normalized directional vector.
  /// @return A new matrix that stores the result.
  static inline Matrix<T, 3> RotationZ(const Vector<T, 2>& v) {
    return Matrix<T, 3>(
         v.x(), v.y(), 0,
        -v.y(), v.x(), 0,
         0,     0,     1);
  }

  /// Create a 3x3 rotation matrix from an angle (in radians)
  /// around the X axis.
  /// @param a The angle (in radians).
  /// @return A new matrix that stores the result.
  static inline Matrix<T, 3> RotationX(T a) {
    return RotationX(Vector<T, 2>(cosf(a), sinf(a)));
  }

  /// Create a 3x3 rotation matrix from an angle (in radians)
  /// around the Y axis.
  /// @param a The angle (in radians).
  /// @return A new matrix that stores the result.
  static inline Matrix<T, 3> RotationY(T a) {
    return RotationY(Vector<T, 2>(cosf(a), sinf(a)));
  }

  /// Create a 3x3 rotation matrix from an angle (in radians)
  /// around the Z axis.
  /// @param a The angle (in radians).
  /// @return A new matrix that stores the result.
  static inline Matrix<T, 3> RotationZ(T a) {
    return RotationZ(Vector<T, 2>(cosf(a), sinf(a)));
  }

  /// Create a 4x4 perpective matrix.
  /// @handedness: 1.0f for RH, -1.0f for LH
  static inline Matrix<T, 4, 4> Perspective(T fovy, T aspect, T znear, T zfar,
                                            T handedness = 1) {
    return PerspectiveHelper(fovy, aspect, znear, zfar, handedness);
  }

  /// Create a 4x4 orthographic matrix.
  static inline Matrix<T, 4, 4> Ortho(T left, T right, T bottom, T top,
                                      T znear, T zfar) {
    return OrthoHelper(left, right, bottom, top, znear, zfar);
  }

  /// Create a 3-dimensional camera matrix.
  /// @param at The look-at target of the camera.
  /// @param eye The position of the camera.
  /// @param up The up vector in the world, for example (0, 1, 0) if the
  /// y-axis is up.
  static inline Matrix<T, 4, 4> LookAt(const Vector<T, 3>& at,
      const Vector<T, 3>& eye, const Vector<T, 3>& up) {
    return LookAtHelper(at, eye, up);
  }

  /// Vector/Matrix multiplication.
  /// @param v The vector to use for mulitplication.
  /// @param m The matrix to use for multiplication.
  /// @return A new vector that stores the result.
  friend inline Vector<T, columns> operator*(
      const Vector<T, rows>& v, const Matrix<T, rows, columns>& m) {
    const int d = columns;
    MATHFU_VECTOR_OPERATOR((Vector<T, rows>::DotProduct(m.data_[i], v)));
  }

  // Dimensions of the matrix.
  /// Number of rows in the matrix.
  static const int kRows = rows;
  /// Number of columns in the matrix.
  static const int kColumns = columns;
  /// Total number of elements in the matrix.
  static const int kElements = rows * columns;

 private:
  Vector<T, rows> data_[columns];
};

// Scalar/Matrix multiplication
template<class T, int rows, int columns>
inline Matrix<T, rows, columns> operator*(const T& s,
                                          const Matrix<T, columns, rows>& m) {
  return m * s;
}

// Matrix/Vector multiplication. Template specialized versions are
// implemented for 2x2, 3x3, and 4x4 matrices to increase performance.
// 3x3 float is also specialized because if SIMD is used the vectors of
// this type of length 4.
template<class T, int rows, int columns>
inline Vector<T, rows> operator*(const Matrix<T, rows, columns>& m,
                                 const Vector<T, columns>& v) {
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
inline Vector<T, 2> operator*(const Matrix<T, 2, 2>& m,
                              const Vector<T, 2>& v) {
    return Vector<T, 2>(m[0] * v[0] + m[2] * v[1], m[1] * v[0] + m[3] * v[1]);
}

template<class T>
inline Vector<T, 3> operator*(const Matrix<T, 3, 3>& m,
                              const Vector<T, 3>& v) {
  return Vector<T, 3>(MATHFU_MATRIX_3X3_DOT(&m[0], v, 0, 3),
                      MATHFU_MATRIX_3X3_DOT(&m[0], v, 1, 3),
                      MATHFU_MATRIX_3X3_DOT(&m[0], v, 2, 3));
}

template<>
inline Vector<float, 3> operator*(const Matrix<float, 3, 3>& m,
                                  const Vector<float, 3>& v) {
  return Vector<float, 3>(
      MATHFU_MATRIX_3X3_DOT(&m[0], v, 0, MATHFU_VECTOR_STRIDE_FLOATS(v)),
      MATHFU_MATRIX_3X3_DOT(&m[0], v, 1, MATHFU_VECTOR_STRIDE_FLOATS(v)),
      MATHFU_MATRIX_3X3_DOT(&m[0], v, 2, MATHFU_VECTOR_STRIDE_FLOATS(v)));
}

template<class T>
inline Vector<T, 4> operator*(const Matrix<T, 4, 4>& m,
                              const Vector<T, 4>& v) {
  return Vector<T, 4>(MATHFU_MATRIX_4X4_DOT(&m[0], v, 0),
                      MATHFU_MATRIX_4X4_DOT(&m[0], v, 1),
                      MATHFU_MATRIX_4X4_DOT(&m[0], v, 2),
                      MATHFU_MATRIX_4X4_DOT(&m[0], v, 3));
}

// Matrix/Vector multiplication of a 4x4 matrix with a vector of size 3.
// This is provided as a convenience and assumes the vector has a fourth
// component equal to 1.
template<class T>
inline Vector<T, 3> operator*(const Matrix<T, 4, 4>& m,
                              const Vector<T, 3>& v) {
  Vector<T, 4> v4(v[0], v[1], v[2], 1);
  v4 = m * v4;
  return Vector<T, 3>(v4[0] / v4[3], v4[1] / v4[3], v4[2] / v4[3]);
}

// Matrix/Matrix Multiplication. Template specialized versions are implemented
// for 2x2, 3x3, and 4x4 matrices to improve performance. 3x3 float is also
// specialized because if SIMD is used the vectors of this type of length 4.
template<class T, int size1, int size2, int size3>
inline void TimesHelper(const Matrix<T, size1, size2>& m1,
                        const Matrix<T, size2, size3>& m2,
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
inline void TimesHelper(const Matrix<T, 2, 2>& m1, const Matrix<T, 2, 2>& m2,
                        Matrix<T, 2, 2>* out_m) {
  Matrix<T, 2, 2>& out = *out_m;
  out[0] = m1[0] * m2[0] + m1[2] * m2[1];
  out[1] = m1[1] * m2[0] + m1[3] * m2[1];
  out[2] = m1[0] * m2[2] + m1[2] * m2[3];
  out[3] = m1[1] * m2[2] + m1[3] * m2[3];
}

template<typename T>
inline void TimesHelper(const Matrix<T, 3, 3>& m1, const Matrix<T, 3, 3>& m2,
                        Matrix<T, 3, 3>* out_m) {
  Matrix<T, 3, 3>& out = *out_m;
  {
    Vector<T, 3> row(m1[0], m1[3], m1[6]);
    out[0] = Vector<T, 3>::DotProduct(m2.GetColumn(0), row);
    out[3] = Vector<T, 3>::DotProduct(m2.GetColumn(1), row);
    out[6] = Vector<T, 3>::DotProduct(m2.GetColumn(2), row);
  }
  {
    Vector<T, 3> row(m1[1], m1[4], m1[7]);
    out[1] = Vector<T, 3>::DotProduct(m2.GetColumn(0), row);
    out[4] = Vector<T, 3>::DotProduct(m2.GetColumn(1), row);
    out[7] = Vector<T, 3>::DotProduct(m2.GetColumn(2), row);
  }
  {
    Vector<T, 3> row(m1[2], m1[5], m1[8]);
    out[2] = Vector<T, 3>::DotProduct(m2.GetColumn(0), row);
    out[5] = Vector<T, 3>::DotProduct(m2.GetColumn(1), row);
    out[8] = Vector<T, 3>::DotProduct(m2.GetColumn(2), row);
  }
}

template<class T>
inline void TimesHelper(const Matrix<T, 4, 4>& m1, const Matrix<T, 4, 4>& m2,
                        Matrix<T, 4, 4>* out_m) {
  Matrix<T, 4, 4> &out = *out_m;
  {
    Vector<T, 4> row(m1[0], m1[4], m1[8], m1[12]);
    out[0] = Vector<T, 4>::DotProduct(m2.GetColumn(0), row);
    out[4] = Vector<T, 4>::DotProduct(m2.GetColumn(1), row);
    out[8] = Vector<T, 4>::DotProduct(m2.GetColumn(2), row);
    out[12] = Vector<T, 4>::DotProduct(m2.GetColumn(3), row);
  }
  {
    Vector<T, 4> row(m1[1], m1[5], m1[9], m1[13]);
    out[1] = Vector<T, 4>::DotProduct(m2.GetColumn(0), row);
    out[5] = Vector<T, 4>::DotProduct(m2.GetColumn(1), row);
    out[9] = Vector<T, 4>::DotProduct(m2.GetColumn(2), row);
    out[13] = Vector<T, 4>::DotProduct(m2.GetColumn(3), row);
  }
  {
    Vector<T, 4> row(m1[2], m1[6], m1[10], m1[14]);
    out[2] = Vector<T, 4>::DotProduct(m2.GetColumn(0), row);
    out[6] = Vector<T, 4>::DotProduct(m2.GetColumn(1), row);
    out[10] = Vector<T, 4>::DotProduct(m2.GetColumn(2), row);
    out[14] = Vector<T, 4>::DotProduct(m2.GetColumn(3), row);
  }
  {
    Vector<T, 4> row(m1[3], m1[7], m1[11], m1[15]);
    out[3] = Vector<T, 4>::DotProduct(m2.GetColumn(0), row);
    out[7] = Vector<T, 4>::DotProduct(m2.GetColumn(1), row);
    out[11] = Vector<T, 4>::DotProduct(m2.GetColumn(2), row);
    out[15] = Vector<T, 4>::DotProduct(m2.GetColumn(3), row);
  }
}

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

// Struct used for template specialization for functions that return constants.
template<class T>
class Constants {
 public:
  // Minimum absolute value of the determinant of an invertible matrix.
  static T GetDeterminantThreshold()
  {
    // No constant defined for the general case.
    assert(false);
    return 0;
  }
};

template<>
class Constants<float> {
 public:
  // Minimum absolute value of the determinant of an invertible float matrix.
  // float values have 23 bits of precision which is roughly 1e7f, given that
  // the final step of matrix inversion is multiplication with the inverse of
  // the determinant, the minimum value of the determinant is 1e-7f before
  // the precision too low to accurately calculate the inverse.
  static float GetDeterminantThreshold() { return 1e-7f; }
};

template<>
class Constants<double> {
 public:
  // Minimum absolute value of the determinant of an invertible float matrix.
  // float values have  bits of precision which is roughly 1e15f, given that
  // the final step of matrix inversion is multiplication with the inverse of
  // the determinant, the minimum value of the determinant is 1e-15f before
  // the precision too low to accurately calculate the inverse.
  static double GetDeterminantThreshold() { return 1e-15; }
};

// Compute the inverse of a matrix. There is template specialization
// for 2x2, 3x3, and 4x4 matrices to increase performance. Inverse
// is not implemented for dense matrices that are not of size 2x2,
// 3x3, and 4x4.  If check_invertible is true the determine of the matrix
// is compared with Constants<T>::GetDeterminantThreshold() to roughly
// determine whether the matrix is invertible.
template<bool check_invertible, class T, int rows, int columns>
inline bool InverseHelper(const Matrix<T, rows, columns>& m,
                          Matrix<T, rows, columns>* const inverse) {
  assert(false);
  (void)m;
  *inverse = T::Identity();
  return false;
}

template<bool check_invertible, class T>
inline bool InverseHelper(const Matrix<T, 2, 2>& m,
                          Matrix<T, 2, 2>* const inverse) {
  T determinant = m[0] * m[3] - m[1] * m[2];
  if (check_invertible &&
      fabs(determinant) < Constants<T>::GetDeterminantThreshold()) {
    return false;
  }
  T inverseDeterminant = 1 / determinant;
  (*inverse)[0] = inverseDeterminant * m[3];
  (*inverse)[1] = -inverseDeterminant * m[1];
  (*inverse)[2] = -inverseDeterminant * m[2];
  (*inverse)[3] = inverseDeterminant * m[0];
  return true;
}

template<bool check_invertible, class T>
inline bool InverseHelper(const Matrix<T, 3, 3>& m,
                          Matrix<T, 3, 3>* const inverse) {
  // Find determinant of matrix.
  T sub11 = m[4] * m[8] - m[5] * m[7],
    sub12 = -m[1] * m[8] + m[2] * m[7],
    sub13 = m[1] * m[5] - m[2] * m[4];
  T determinant = m[0] * sub11 + m[3] * sub12 + m[6] * sub13;
  if (check_invertible &&
      fabs(determinant) < Constants<T>::GetDeterminantThreshold()) {
    return false;
  }
  // Find determinants of 2x2 submatrices for the elements of the inverse.
  *inverse = Matrix<T, 3, 3>(
      sub11, sub12, sub13,
      m[6] * m[5] - m[3] * m[8],
      m[0] * m[8] - m[6] * m[2],
      m[3] * m[2] - m[0] * m[5],
      m[3] * m[7] - m[6] * m[4],
      m[6] * m[1] - m[0] * m[7],
      m[0] * m[4] - m[3] * m[1]);
  *(inverse) *= 1 / determinant;
  return true;
}

template<class T>
inline int FindLargestPivotElem(const Matrix<T, 4, 4>& m) {
  Vector<T, 4> fabs_column(fabs(m[0]), fabs(m[1]), fabs(m[2]), fabs(m[3]));
  if (fabs_column[0] > fabs_column[1]) {
    if (fabs_column[0] > fabs_column[2]) {
      if (fabs_column[0] > fabs_column[3]) {
        return 0;
      }
      else {
        return 3;
      }
    } else if (fabs_column[2] > fabs_column[3]) {
      return 2;
    }
    else {
      return 3;
    }
  } else if (fabs_column[1] > fabs_column[2]) {
    if (fabs_column[1] > fabs_column[3]) {
      return 1;
    }
    else {
      return 3;
    }
  } else if (fabs_column[2] > fabs_column[3]) {
    return 2;
  }
  else {
    return 3;
  }
}

template<bool check_invertible, class T>
bool InverseHelper(const Matrix<T, 4, 4>& m, Matrix<T, 4, 4>* const inverse) {
  // This will find the pivot element.
  int pivot_elem = FindLargestPivotElem(m);
  // This will perform the pivot and find the row, column, and 3x3 submatrix
  // for this pivot.
  Vector<T, 3> row, column;
  Matrix<T, 3> matrix;
  if (pivot_elem == 0) {
    row = Vector<T, 3>(m[4], m[8], m[12]);
    column = Vector<T, 3>(m[1], m[2], m[3]);
    matrix = Matrix<T, 3>(m[5], m[6], m[7], m[9], m[10],
                          m[11], m[13], m[14], m[15]);
  } else if (pivot_elem == 1) {
    row = Vector<T, 3>(m[5], m[9], m[13]);
    column = Vector<T, 3>(m[0], m[2], m[3]);
    matrix = Matrix<T, 3>(m[4], m[6], m[7], m[8], m[10],
                          m[11], m[12], m[14], m[15]);
  } else if (pivot_elem == 2) {
    row = Vector<T, 3>(m[6], m[10], m[14]);
    column = Vector<T, 3>(m[0], m[1], m[3]);
    matrix = Matrix<T, 3>(m[4], m[5], m[7], m[8], m[9],
                          m[11], m[12], m[13], m[15]);
  } else {
    row = Vector<T, 3>(m[7], m[11], m[15]);
    column = Vector<T, 3>(m[0], m[1], m[2]);
    matrix = Matrix<T, 3>(m[4], m[5], m[6], m[8], m[9],
                          m[10], m[12], m[13], m[14]);
  }
  T pivot_value = m[pivot_elem];
  if (check_invertible &&
      fabs(pivot_value) < Constants<T>::GetDeterminantThreshold()) {
    return false;
  }
  // This will compute the inverse using the row, column, and 3x3 submatrix.
  T inv = -1 / pivot_value;
  row *= inv;
  matrix += Matrix<T, 3>::OuterProduct(column, row);
  Matrix<T, 3> mat_inverse;
  if (!InverseHelper<check_invertible>(matrix, &mat_inverse) &&
      check_invertible) {
    return false;
  }
  Vector<T, 3> col_inverse = mat_inverse * (column * inv);
  Vector<T, 3> row_inverse = row * mat_inverse;
  T pivot_inverse = Vector<T, 3>::DotProduct(row, col_inverse) - inv;
  if (pivot_elem == 0) {
    *inverse = Matrix<T, 4, 4>(
        pivot_inverse, col_inverse[0], col_inverse[1], col_inverse[2],
        row_inverse[0], mat_inverse[0], mat_inverse[1], mat_inverse[2],
        row_inverse[1], mat_inverse[3], mat_inverse[4], mat_inverse[5],
        row_inverse[2], mat_inverse[6], mat_inverse[7], mat_inverse[8]);
  } else if (pivot_elem == 1) {
    *inverse = Matrix<T, 4, 4>(
        row_inverse[0], mat_inverse[0], mat_inverse[1], mat_inverse[2],
        pivot_inverse, col_inverse[0], col_inverse[1], col_inverse[2],
        row_inverse[1], mat_inverse[3], mat_inverse[4], mat_inverse[5],
        row_inverse[2], mat_inverse[6], mat_inverse[7], mat_inverse[8]);
  } else if (pivot_elem == 2) {
    *inverse = Matrix<T, 4, 4>(
        row_inverse[0], mat_inverse[0], mat_inverse[1], mat_inverse[2],
        row_inverse[1], mat_inverse[3], mat_inverse[4], mat_inverse[5],
        pivot_inverse, col_inverse[0], col_inverse[1], col_inverse[2],
        row_inverse[2], mat_inverse[6], mat_inverse[7], mat_inverse[8]);
  } else {
    *inverse = Matrix<T, 4, 4>(
        row_inverse[0], mat_inverse[0], mat_inverse[1], mat_inverse[2],
        row_inverse[1], mat_inverse[3], mat_inverse[4], mat_inverse[5],
        row_inverse[2], mat_inverse[6], mat_inverse[7], mat_inverse[8],
        pivot_inverse, col_inverse[0], col_inverse[1], col_inverse[2]);
  }
  return true;
}

// Create a 4x4 perpective matrix.
template<class T>
inline Matrix<T, 4, 4> PerspectiveHelper(T fovy, T aspect, T znear, T zfar,
                                         T handedness) {
  const T y = 1 / tan(static_cast<T>(fovy) * static_cast<T>(.5));
  const T x = y / aspect;
  const T zdist = (znear - zfar) * handedness;
  const T zfar_per_zdist = zfar / zdist;
  return Matrix<T, 4, 4>(x, 0, 0, 0,
                         0, y, 0, 0,
                         0, 0, zfar_per_zdist, -1 * handedness,
                         0, 0, znear * zfar_per_zdist * handedness, 0);
}

/// Create a 4x4 orthographic matrix.
template<class T>
static inline Matrix<T, 4, 4> OrthoHelper(T left, T right, T bottom, T top,
                                          T znear, T zfar) {
  return Matrix<T, 4, 4>(
      static_cast<T>(2) / (right - left), 0, 0, 0,
      0, static_cast<T>(2) / (top - bottom), 0, 0,
      0, 0, static_cast<T>(-2) / (zfar - znear), 0,
      -(right + left) / (right - left), -(top + bottom) / (top - bottom),
      -(zfar + znear) / (zfar - znear), static_cast<T>(1));
}

// Calculate the axes required to construct a 3-dimensional camera matrix that
// looks at "at" from eye position "eye" with the up vector "up".  The axes
// are returned in a 4 element "axes" array.
template<class T>
static void LookAtHelperCalculateAxes(
    const Vector<T, 3>& at, const Vector<T, 3>& eye, const Vector<T, 3>& up,
    Vector<T, 3> * const axes) {
  axes[2] = (at - eye).Normalized();
  axes[0] = Vector<T, 3>::CrossProduct(up, axes[2]).Normalized();
  axes[1] = Vector<T, 3>::CrossProduct(axes[2], axes[0]);
  axes[3] = Vector<T, 3>(-Vector<T, 3>::DotProduct(axes[0], eye),
                         -Vector<T, 3>::DotProduct(axes[1], eye),
                         -Vector<T, 3>::DotProduct(axes[2], eye));
}

// Create a 3-dimensional camera matrix.
template<class T>
static inline Matrix<T, 4, 4> LookAtHelper(
    const Vector<T, 3>& at, const Vector<T, 3>& eye, const Vector<T, 3>& up) {
  Vector<T, 3> axes[4];
  LookAtHelperCalculateAxes(at, eye, up, axes);
  const Vector<T, 4> column0(axes[0][0], axes[1][0], axes[2][0], 0);
  const Vector<T, 4> column1(axes[0][1], axes[1][1], axes[2][1], 0);
  const Vector<T, 4> column2(axes[0][2], axes[1][2], axes[2][2], 0);
  const Vector<T, 4> column3(axes[3], 1);
  return Matrix<T, 4, 4>(column0, column1, column2, column3);
}


}  // namespace mathfu

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

#endif  // MATHFU_MATRIX_H_
