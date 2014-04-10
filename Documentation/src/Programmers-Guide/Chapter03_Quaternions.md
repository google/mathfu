# Quaternions

[About](#about)<br/>
[Data](#data)<br/>
[Operations](#op)<br/>
[SIMD](#simd)<br/>
<br/>

<a name="about">
## About
In the distribution of GooMath is a Quaternion class which stores data and
defines a number of operations that can be performed. For more details on
what a quaternion is please see http://en.wikipedia.org/wiki/Quaternion.

<a name="data"></a><br/>
## Data

The data in a Quaternion is stored as scalar of type T and Vector<T, 3>.
Please see the vector documentation for more information about Vector.
Creation of a Quaternion looks like Quaternion<T>.

<a name="op"></a><br/>
## Operations

Once a quaternion is created, a number of operations can be applied.
Quaternions define arithmatic multiplication, inversion, interpolation,
and a number of conversion operations between a 3x3 rotation matrix,
euler angles around the x, y, and z axes, and an axis and angle of rotation
about that axis.

As an example, here is how to perform slerp of two angles specified by 3 euler
angles and store the result as angles:

    // Use radians for angles
    Vector<float, 3> angles1(0.66f, 1.3f, 0.76f);
    Vector<float, 3> angles2(0.85f, 0.33f, 1.6f);

    Quaternion<float> quat1 = Quaternion<float>::FromEulerAngles(angles1);
    Quaternion<float> quat2 = Quaternion<float>::FromEulerAngles(angles2);

    Quaternion<float> quatSlerp = Quaternion<float>::Slerp(quat1, quat2, 0.5);
    Vector<float, 3> angleSlerp = quatSlerp.ToEulerAngles();

For complete details please visit the api docs.

<a name="simd"></a><br/>
## SIMD

The quaternion class itself does not explicitly use any simd data types
or operations but because it stores the vector component as a Vector<T, 3>
the Quaternion<float> class will take advantage of simd intrinsics through
the vector class. Please see the documentation on Vector for more information
about how it uses simd types and intrinsics.
