Vectors    {#mathfu_guide_vectors}
=======

TODO outline
   * Declaring vectors.
   * How are elements accessed?
   * What operations are supported?
   * Arithmetic
   * Cross Product
   * Interpolation
   * Packing / unpacking

# About    {#mathfu_guide_vectors_about}

In the distribution of MathFu is a Vector class which stores data and
defines a number of operations that can be performed.

# Data    {#mathfu_guide_vectors_data}

The data in a vector is stored as a d size array of type T where T and d
are template arguments known at compile time. Creation of a vector looks
like Vector<T, d>.

# Operations    {#mathfu_guide_vectors_operations}

Once a vector is created, a number of operations can be applied. Vectors
define arithmatic operations (+,-,*,/), accessors ([]), and a few others
such as cross product, dot product, and normalization.

For example, to create three points and compute the normal of the plane
defined by the points execute:

    <!-- @doxysnippetstart Vector_Sample -->
    Vector<float, 3> point1(0.5f, 0.4f, 0.1f);
    Vector<float, 3> point2(0.4f, 0.9f, 0.1f);
    Vector<float, 3> point3(0.1f, 0.8f, 0.6f);

    Vector<float, 3> vector1 = point2 - point1;
    Vector<float, 3> vector2 = point3 - point1;

    Vector<float, 3> normal = Vector<float, 3>::CrossProduct(vector2, vector1);
    <!-- @doxysnippetend -->

Note that Vector<float, 3> is typedef'd to vec3 for GLSL compatbility.
Similarly cross is defined to call CrossProduct. For more details please visit
the api docs.



