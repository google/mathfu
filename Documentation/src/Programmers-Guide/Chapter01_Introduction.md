# Introduction

[About](#about)<br/>
[Prerequisites](#pre)<br/>
[About this manual](#atm)<br/>
[Feedback and reporting bugs](#frb)<br/>
[Core concepts](#cc)<br/>


<a name="About"></a><br/>

## About

GooMath is a basic math library for games. It contains data structures
and utilities for Vectors, Matrices, and Quaternions. It also has the option
to use simd instructions (sse or neon) for improvded efficiency.

GooMath is written in portable C++. The classes are within the goomath
namespace. Hopefully this is sufficient to avoid name clashing with your
existing code.

<a name="pre"></a><br/>
## Prerequisites

GooMath is built ontop of the vectorial math library which contains an
abstraction layer around common simd instructions.

Since GooMath is written in C++, you are expected to be experienced in C++
programming. GooMath should not be your first C++ programming project! You
should be comfortable with compiling, linking, and debugging.

	Caution

	GooMath should not be your first C++ project. Please learn C++
programming, compiling, linking, and debugging before working with LiquidFun.
There are many resources for this on the net.

<a name="atm"></a><br/>

## About this manual

This manual covers the majority of the GooMath API. However, not every
aspect is covered. You are encouraged to look at the Unittests included with
GooMath to learn more. Also, the GooMath code base has comments formatted
for Doxygen, so it is easy to create a hyper-linked API document.

This manual is only updated with new releases. The version in source control
is likely to be out of date.

<a name="frb"></a><br/>
## Feedback and Reporting Bugs

If you have a question or feedback about GooMath, please leave a comment in
the forum. This is also a great place for community discussion.

GooMath issues are tracked using a Google code project. This is a great way
to track issues and ensures that your issue will not be lost in the depths of
the forums.

Please file bugs and feature requests here:
[http://google.github.io/goomath/](http://google.github.io/goomath/)

 You can help to ensure your issue gets fixed if you provide sufficient
detail. 

<a name="cc"></a><br/>
## Core Concepts

GooMath comprises three primary datatypes: Vector, Quaternion,
and Matrix. We briefly define these objects here and more details are given
later in this document.<br/>
<br/>
### Vector
A vector is a geometric concept with a magnitude and direction. It can be
defined in any dimensional space.

### Quaternion
A Quaternion is a specific type of four dimensional vector and defines a
rotation in three dimensional space.

### Matrix
A matrix is a set of data organized in rows and columns. A general matrix
may have any number of rows and columns.

[ ][Chapter02]
[ ][Chapter03]
[ ][Chapter04]

  [Chapter02]: md__chapter02__vectors.html
  [Chapter03]: md__chapter03__quaternions.html
  [Chapter04]: md__chapter04__matrices.html
