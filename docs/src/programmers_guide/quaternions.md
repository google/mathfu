Quaternions    {#mathfu_guide_quaternions}
===========

TODO outline
   * Declaring quaternions
   * Convert to euler angles
   * Rotation matrices
   * Interpolation

# About
In the distribution of MathFu is a Quaternion class which stores data and
defines a number of operations that can be performed. For more details on
what a quaternion is please see http://en.wikipedia.org/wiki/Quaternion.

# Data

The data in a Quaternion is stored as scalar of type T and Vector<T, 3>.
Please see the vector documentation for more information about Vector.
Creation of a Quaternion looks like Quaternion<T>.

# Operations

Once a quaternion is created, a number of operations can be applied.
Quaternions define arithmatic multiplication, inversion, interpolation,
and a number of conversion operations between a 3x3 rotation matrix,
euler angles around the x, y, and z axes, and an axis and angle of rotation
about that axis.

As an example, here is how to perform slerp of two angles specified by 3 euler
angles and store the result as angles:

    <!-- @doxysnippetstart Quaternion_Sample -->
    // Use radians for angles
    Vector<float, 3> angles1(0.66f, 1.3f, 0.76f);
    Vector<float, 3> angles2(0.85f, 0.33f, 1.6f);

    Quaternion<float> quat1 = Quaternion<float>::FromEulerAngles(angles1);
    Quaternion<float> quat2 = Quaternion<float>::FromEulerAngles(angles2);

    Quaternion<float> quatSlerp = Quaternion<float>::Slerp(quat1, quat2, 0.5);
    Vector<float, 3> angleSlerp = quatSlerp.ToEulerAngles();
    <!-- @doxysnippetend -->

For complete details please visit the api docs.

