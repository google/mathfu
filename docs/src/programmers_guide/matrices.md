Matrices    {#mathfu_guide_matrices}
========

TODO outline
   * Declaring matrices
   * How are elements accessed?
   * What operations are supported?
   * Arithmetic
   * Translation
   * Scaling
   * Rotation
   * Camera matrices (Perspective / Orthographic / Lookat)

# About    {#mathfu_guide_matrices_about}
In the distribution of MathFu is a Matrix class which stores data and
defines a number of operations that can be performed.

<a name="data"></a><br/>
# Data

The data in a Matrix is stored as a columns sized array of Vector<T, rows>.
T can be any element type and the size and the Matrix class is created using
Matrix<T, rows, columns>.

<a name="op"></a><br/>
# Operations

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

