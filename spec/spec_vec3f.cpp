#include "spec_helper.h"
#include <iostream>
using vectorial::vec3f;

const int epsilon = 1;

describe(vec3f, "constructing") {
    it("should have default constructor that does nothing..") {
        vec3f x;
    }

    it("should have constructor with element values") {
        vec3f x(10,20,30);
        // octave vec3f: [10,20,30]
        should_be_equal_vec3f(x, simd4f_create(10.000000000000000f, 20.000000000000000f, 30.000000000000000f, 0.0f), epsilon );
        
    }

    it("should have constructor that loads from a float array") {
        float ary[3] = { 1,2,3 };
        vec3f x(ary);
        // octave vec3f: [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 3.000000000000000f, 0.0f), epsilon );
    }

}

describe(vec3f, "loads and stores") {

    it("should have method for loading from a float array") {
        float ary[3] = { 1,2,3 };
        vec3f x(-1, -1, -1 );
        x.load(ary);
        // octave vec3f: [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 3.000000000000000f, 0.0f), epsilon );
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
        should_be_equal_vec3f(x, simd4f_create(11.000000000000000f, 22.000000000000000f, 33.000000000000000f, 0.0f), epsilon );

    }

    it("should have operator- for component-wise subtraction") {
        vec3f a(1,2,3);
        vec3f b(10,20,30);
        vec3f x = b - a;
        // octave vec3f:  [10,20,30] - [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(9.000000000000000f, 18.000000000000000f, 27.000000000000000f, 0.0f), epsilon );

    }

    it("should have operator* for component-wise multiplication") {
        vec3f a(1,2,3);
        vec3f b(10,20,30);
        vec3f x = a * b;
        // octave vec3f: [1,2,3] .* [10,20,30]
        should_be_equal_vec3f(x, simd4f_create(10.000000000000000f, 40.000000000000000f, 90.000000000000000f, 0.0f), epsilon );

    }

    it("should have operator/ for component-wise division") {
        vec3f a(1,2,3);
        vec3f b(10,20,30);
        vec3f x = b / a;
        // octave vec3f:  [10,20,30] ./ [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(10.000000000000000f, 10.000000000000000f, 10.000000000000000f, 0.0f), epsilon );

    }



    it("should have operator+= for component-wise addition") {
        vec3f x(1,2,3);
        vec3f b(10,20,30);
        x += b;
        // octave vec3f: [1,2,3] + [10,20,30]
        should_be_equal_vec3f(x, simd4f_create(11.000000000000000f, 22.000000000000000f, 33.000000000000000f, 0.0f), epsilon );

    }

    it("should have operator-= for component-wise subtraction") {
        vec3f a(1,2,3);
        vec3f x(10,20,30);
        x -= a;
        // octave vec3f:  [10,20,30] - [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(9.000000000000000f, 18.000000000000000f, 27.000000000000000f, 0.0f), epsilon );

    }

    it("should have operator*= for component-wise multiplication") {
        vec3f x(1,2,3);
        vec3f b(10,20,30);
        x *= b;
        // octave vec3f: [1,2,3] .* [10,20,30]
        should_be_equal_vec3f(x, simd4f_create(10.000000000000000f, 40.000000000000000f, 90.000000000000000f, 0.0f), epsilon );

    }

    it("should have operator/= for component-wise division") {
        vec3f a(1,2,3);
        vec3f x(10,20,30);
        x /= a;
        // octave vec3f:  [10,20,30] ./ [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(10.000000000000000f, 10.000000000000000f, 10.000000000000000f, 0.0f), epsilon );

    }
    
}


describe(vec3f, "arithmetic with scalar") {
    
    it("should have operator+ for component-wise addition") {
        vec3f a(1,2,3);
        float b=10;
        vec3f x = a + b;
        // octave vec3f: [1,2,3] + 10
        should_be_equal_vec3f(x, simd4f_create(11.000000000000000f, 12.000000000000000f, 13.000000000000000f, 0.0f), epsilon );

    }

    it("should have operator- for component-wise subtraction") {
        float a=10;
        vec3f b(10,20,30);
        vec3f x = b - a;
        // octave vec3f:  [10,20,30] - 10
        should_be_equal_vec3f(x, simd4f_create(0.000000000000000f, 10.000000000000000f, 20.000000000000000f, 0.0f), epsilon );

    }

    it("should have operator* for component-wise multiplication") {
        vec3f a(1,2,3);
        float b=10;
        vec3f x = a * b;
        // octave vec3f: [1,2,3] .* 10
        should_be_equal_vec3f(x, simd4f_create(10.000000000000000f, 20.000000000000000f, 30.000000000000000f, 0.0f), epsilon );

    }

    it("should have operator/ for component-wise division") {
        vec3f a(10,20,30);
        float b=10;
        vec3f x = a / b;
        // octave vec3f: [10,20,30] ./ 10
        should_be_equal_vec3f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 3.000000000000000f, 0.0f), epsilon );

    }



    it("should have operator+ for component-wise addition (float as lhs)") {
        vec3f b(1,2,3);
        float a=10;
        vec3f x = a + b;
        // octave vec3f: 10 + [1,2,3]
        should_be_equal_vec3f(x, simd4f_create(11.000000000000000f, 12.000000000000000f, 13.000000000000000f, 0.0f), epsilon );

    }

    it("should have operator- for component-wise subtraction (float as lhs)") {
        float b=50;
        vec3f a(10,20,30);
        vec3f x = b - a;
        // octave vec3f:  50 - [10,20,30]
        should_be_equal_vec3f(x, simd4f_create(40.000000000000000f, 30.000000000000000f, 20.000000000000000f, 0.0f), epsilon );

    }

    it("should have operator* for component-wise multiplication (float as lhs)") {
        vec3f b(1,2,3);
        float a=10;
        vec3f x = a * b;
        // octave vec3f: 10 .* [1,2,3] 
        should_be_equal_vec3f(x, simd4f_create(10.000000000000000f, 20.000000000000000f, 30.000000000000000f, 0.0f), epsilon );

    }

    it("should have operator* for component-wise multiplication (float as lhs)") {
        vec3f b(10,20,30);
        float a=40;
        vec3f x = a / b;
        // octave vec3f: 40 ./ [10,20,30] 
        should_be_equal_vec3f(x, simd4f_create(4.000000000000000f, 2.000000000000000f, 1.333333333333333f, 0.0f), epsilon );

    }

    
}



describe(vec3f, "vector math") {

    it("should have unary minus operator") {
        vec3f a(1,2,3);
        vec3f x = -a;
        // octave vec3f: -[1,2,3]
        should_be_equal_vec3f(x, simd4f_create(-1.000000000000000f, -2.000000000000000f, -3.000000000000000f, 0.0f), epsilon );
    }


    it("should have dot function") {
        vec3f a(1,2,3);
        vec3f b(6,7,8);
        float x = vectorial::dot(a,b);
        
        // octave vec3f: dot([1,2,3],[6,7,8])
        should_be_close_to(x, 44.000000000000000f, epsilon );
    }

    it("should have cross function") {
        vec3f a(1,2,3);
        vec3f b(6,7,8);
        vec3f x = vectorial::cross(a,b);
        
        // octave vec3f: cross([1,2,3],[6,7,8])
        should_be_equal_vec3f(x, simd4f_create(-5.000000000000000f, 10.000000000000000f, -5.000000000000000f, 0.0f), epsilon );
    }

    it("should have length_squared function") {
        vec3f a(1,2,3);
        float x = vectorial::length_squared(a);
        
        // octave vec3f: dot([1,2,3],[1,2,3])
        should_be_close_to(x, 14.000000000000000f, epsilon );
    }

    it("should have length function") {
        vec3f a(1,2,3);
        float x = vectorial::length(a);
        
        // octave vec3f: norm([1,2,3])
        should_be_close_to(x, 3.741657386773941f, epsilon );
    }
    
    
    it("should have normalize function") {
        vec3f a(1,2,3);
        vec3f x = vectorial::normalize(a);
        // octave vec3f: [1,2,3] / norm([1,2,3])
        should_be_equal_vec3f(x, simd4f_create(0.267261241912424f, 0.534522483824849f, 0.801783725737273f, 0.0f), epsilon );
    }

}


