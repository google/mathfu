# Matrices

[About](#about)<br/>
[Data](#data)<br/>
[Operations](#op)<br/>
[SIMD](#simd)<br/>
<br/>

<a name="about">
## About
In the distribution of GooMath is a Matrix class which stores data and
defines a number of operations that can be performed.

<a name="data"></a><br/>
## Data

The data in a Matrix is stored as a columns sized array of Vector<T, rows>.
T can be any element type and the size and the Matrix class is created using
Matrix<T, rows, columns>.

<a name="op"></a><br/>
## Operations

Once a matrix is created, a number of operations can be applied. Matrices
define arithmatic operations (+,-,*,/), accessors ((),[]), and a few others
such as identity, outer product, and inverse.

Here is an example for rotating a vector in homogenious coordinates by
a transformation matrix:

    <!-- @doxysnippetstart Matrix_Sample -->
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
    <!-- @doxysnippetend -->

For complete details please visit the api docs.

<a name="simd"></a><br/>
## SIMD

The matrix class uses simd intrinsics in a number of different ways. First,
if the Matrix is a Matrix<float, 4, 4> (or Matrix<float, 4>) the data is stored
as an array of 4 float32x4_ts. Each operation is then defined to use intrinsics
on this data. For other sized matrices intrinsics are not explicitly used
but due to the use of an array of Vectors the operations will use simd
datatypes and instrinsics through that class. Please see the documentation for
Vector for more information about how it uses simd types and intrinsics.
