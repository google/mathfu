#include "spec_helper.h"
#include <iostream>
using vectorial::vec3f;

const float epsilon = 0.00001f;

describe(vec3f, "constructing") {
    it("should have default constructor that does nothing..") {
        vec3f x;
    }

    it("should have constructor with element values") {
        vec3f x(10,20,30);
        // octave vec3f: [10,20,30]
        should_be_equal_vec3f(x, simd4f_create(10.000000, 20.000000, 30.000000, 0), epsilon );
        
    }

    it("should have constructor that loads from a float array") {
        float ary[3] = { 1,2,3 };
        vec3f x(ary);
        // octave vec3f: [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(1.000000, 2.000000, 3.000000, 0), epsilon );
    }

}

describe(vec3f, "loads and stores") {

    it("should have method for loading from a float array") {
        float ary[3] = { 1,2,3 };
        vec3f x(-1, -1, -1 );
        x.load(ary);
        // octave vec3f: [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(1.000000, 2.000000, 3.000000, 0), epsilon );
    }

    it("should have method for storing to a float array") {
        float ary[3] = { -1, -1, -1 };
        vec3f x(1, 2, 3);
        x.store(ary);
        should_be_close_to(ary[0], 1, epsilon);
        should_be_close_to(ary[1], 2, epsilon);
        should_be_close_to(ary[2], 3, epsilon);
    }

}

describe(vec3f, "arithmetic with another vec3f") {
    
    it("should have operator+ for component-wise addition") {
        vec3f a(1,2,3);
        vec3f b(10,20,30);
        vec3f x = a + b;
        // octave vec3f: [1,2,3] + [10,20,30]
        should_be_equal_vec3f(x, simd4f_create(11.000000, 22.000000, 33.000000, 0), epsilon );

    }

    it("should have operator- for component-wise subtraction") {
        vec3f a(1,2,3);
        vec3f b(10,20,30);
        vec3f x = b - a;
        // octave vec3f:  [10,20,30] - [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(9.000000, 18.000000, 27.000000, 0), epsilon );

    }

    it("should have operator* for component-wise multiplication") {
        vec3f a(1,2,3);
        vec3f b(10,20,30);
        vec3f x = a * b;
        // octave vec3f: [1,2,3] .* [10,20,30]
        should_be_equal_vec3f(x, simd4f_create(10.000000, 40.000000, 90.000000, 0), epsilon );

    }

    it("should have operator/ for component-wise division") {
        vec3f a(1,2,3);
        vec3f b(10,20,30);
        vec3f x = b / a;
        // octave vec3f:  [10,20,30] ./ [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(10.000000, 10.000000, 10.000000, 0), epsilon );

    }



    it("should have operator+= for component-wise addition") {
        vec3f x(1,2,3);
        vec3f b(10,20,30);
        x += b;
        // octave vec3f: [1,2,3] + [10,20,30]
        should_be_equal_vec3f(x, simd4f_create(11.000000, 22.000000, 33.000000, 0), epsilon );

    }

    it("should have operator-= for component-wise subtraction") {
        vec3f a(1,2,3);
        vec3f x(10,20,30);
        x -= a;
        // octave vec3f:  [10,20,30] - [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(9.000000, 18.000000, 27.000000, 0), epsilon );

    }

    it("should have operator*= for component-wise multiplication") {
        vec3f x(1,2,3);
        vec3f b(10,20,30);
        x *= b;
        // octave vec3f: [1,2,3] .* [10,20,30]
        should_be_equal_vec3f(x, simd4f_create(10.000000, 40.000000, 90.000000, 0), epsilon );

    }

    it("should have operator/= for component-wise division") {
        vec3f a(1,2,3);
        vec3f x(10,20,30);
        x /= a;
        // octave vec3f:  [10,20,30] ./ [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(10.000000, 10.000000, 10.000000, 0), epsilon );

    }
    
}


describe(vec3f, "arithmetic with scalar") {
    
    it("should have operator+ for component-wise addition") {
        vec3f a(1,2,3);
        float b=10;
        vec3f x = a + b;
        // octave vec3f: [1,2,3] + 10
        should_be_equal_vec3f(x, simd4f_create(11.000000, 12.000000, 13.000000, 0), epsilon );

    }

    it("should have operator- for component-wise subtraction") {
        float a=10;
        vec3f b(10,20,30);
        vec3f x = b - a;
        // octave vec3f:  [10,20,30] - 10
        should_be_equal_vec3f(x, simd4f_create(0.000000, 10.000000, 20.000000, 0), epsilon );

    }

    it("should have operator* for component-wise multiplication") {
        vec3f a(1,2,3);
        float b=10;
        vec3f x = a * b;
        // octave vec3f: [1,2,3] .* 10
        should_be_equal_vec3f(x, simd4f_create(10.000000, 20.000000, 30.000000, 0), epsilon );

    }

    it("should have operator/ for component-wise division") {
        vec3f a(10,20,30);
        float b=10;
        vec3f x = a / b;
        // octave vec3f: [10,20,30] ./ 10
        should_be_equal_vec3f(x, simd4f_create(1.000000, 2.000000, 3.000000, 0), epsilon );

    }



    it("should have operator+ for component-wise addition (float as lhs)") {
        vec3f b(1,2,3);
        float a=10;
        vec3f x = a + b;
        // octave vec3f: 10 + [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(11.000000, 12.000000, 13.000000, 0), epsilon );

    }

    it("should have operator- for component-wise subtraction (float as lhs)") {
        float b=50;
        vec3f a(10,20,30);
        vec3f x = b - a;
        // octave vec3f:  50 - [10,20,30]
        should_be_equal_vec3f(x, simd4f_create(40.000000, 30.000000, 20.000000, 0), epsilon );

    }

    it("should have operator* for component-wise multiplication (float as lhs)") {
        vec3f b(1,2,3);
        float a=10;
        vec3f x = a * b;
        // octave vec3f: 10 .* [1,2,3] 
        should_be_equal_vec3f(x, simd4f_create(10.000000, 20.000000, 30.000000, 0), epsilon );

    }

    it("should have operator* for component-wise multiplication (float as lhs)") {
        vec3f b(10,20,30);
        float a=40;
        vec3f x = a / b;
        // octave vec3f: 40 ./ [10,20,30] 
        should_be_equal_vec3f(x, simd4f_create(4.000000, 2.000000, 1.333333, 0), epsilon );

    }

    
}



describe(vec3f, "vector math") {

    it("should have dot function") {
        vec3f a(1,2,3);
        vec3f b(6,7,8);
        float x = vectorial::dot(a,b);
        
        // octave vec3f: dot([1,2,3],[6,7,8])
        should_be_close_to(x, 44.000000, epsilon );
    }

    it("should have cross function") {
        vec3f a(1,2,3);
        vec3f b(6,7,8);
        vec3f x = vectorial::cross(a,b);
        
        // octave vec3f: cross([1,2,3],[6,7,8])
        should_be_equal_vec3f(x, simd4f_create(-5.000000, 10.000000, -5.000000, 0), epsilon );
    }

    it("should have length_squared function") {
        vec3f a(1,2,3);
        float x = vectorial::length_squared(a);
        
        // octave vec3f: dot([1,2,3],[1,2,3])
        should_be_close_to(x, 14.000000, epsilon );
    }

    it("should have length function") {
        vec3f a(1,2,3);
        float x = vectorial::length(a);
        
        // octave vec3f: norm([1,2,3])
        should_be_close_to(x, 3.741657, epsilon );
    }
    
    
    it("should have normalize function") {
        vec3f a(1,2,3);
        vec3f x = vectorial::normalize(a);
        const float epsilon = 0.001f;
        // octave vec3f: [1,2,3] / norm([1,2,3])
        should_be_equal_vec3f(x, simd4f_create(0.267261, 0.534522, 0.801784, 0), epsilon );
    }

}


