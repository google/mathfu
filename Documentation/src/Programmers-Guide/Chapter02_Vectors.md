# Vectors

[About](#about)<br/>
[Data](#data)<br/>
[Operations](#op)<br/>
[SIMD](#simd)<br/>
<br/>

<a name="about">
## About
In the distribution of GooMath is a Vector class which stores data and
defines a number of operations that can be performed.

<a name="data"></a><br/>
## Data

The data in a vector is stored as a d size array of type T where T and d
are template arguments known at compile time. Creation of a vector looks
like Vector<T, d>.

<a name="op"></a><br/>
## Operations

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

<a name="simd"></a><br/>
## SIMD

To compile with [http://en.wikipedia.org/wiki/SIMD](SIMD) support simply
provide the architecture specific flag. On Android/iOS use -mfpu=neon and on
Linux/Windows/MacOS use -msse4.1. If you would like to enable simd support
but compile without it you can define the flag COMPILE_WITHOUT_SIMD_SUPPORT
which will compile the scalar version of vector.

When using neon, vec2 or Vector<float, 2> is defined using float32x2_t. 2D
vectors do not have a SIMD approach using
[http://en.wikipedia.org/wiki/SSE4](SSE) as the cost outweights the
benefit.

3D vectors defined as vec3 or Vector<float, 3> have two options. When
compiling without any flags it will default to using float data[3] which is
not a simd datatype. It will then use load and store operations to use this
data in simd functions such as vector addition. This has a performance hit
compared to using simd datatypes but this allows the use of vectors that are
three wide.

The other option is to use a four wide datatype to store each
vector. To enable this compile with the flag COMPILE_WITH_PADDING. Using this
option the vector will use the datatype float32x4_t for neon or __m128 for sse.
In this case the fourth element will be ignored and is undefined if the
user tries to access it. This form leads to faster performance on smaller
applications as it avoids extra loads and stores but can lead to worse
cache performance due to the data taking up more memory.

4D vectors defined as vec4 or Vector<float, 4> use the datatype float32x4_t
for neon and __m128 for sse.
