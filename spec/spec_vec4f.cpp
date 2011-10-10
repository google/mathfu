#include "spec_helper.h"
#include <iostream>
using vectorial::vec4f;

const int epsilon = 1;

describe(vec4f, "constructing") {
    it("should have default constructor that does nothing..") {
        vec4f x;
    }

    it("should have constructor with element values") {
        vec4f x(10,20,30,40);
        // octave vec4f: [10,20,30,40]
        should_be_equal_vec4f(x, simd4f_create(10.000000000000000f, 20.000000000000000f, 30.000000000000000f, 40.000000000000000f), epsilon );
        
    }

    it("should have constructor that loads from a float array") {
        float ary[4] = { 1,2,3,4 };
        vec4f x(ary);
        // octave vec4f: [1,2,3,4]
        should_be_equal_vec4f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 3.000000000000000f, 4.000000000000000f), epsilon );
    }

}

describe(vec4f, "loads and stores") {


    it("should have method for loading from a float array") {
        float ary[4] = { 1,2,3,4 };
        vec4f x(-1, -1, -1, -1);
        x.load(ary);
        // octave vec4f: [1,2,3,4]
        should_be_equal_vec4f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 3.000000000000000f, 4.000000000000000f), epsilon );
    }

    it("should have method for storing to a float array") {
        float ary[4] = { -1, -1, -1, -1 };
        vec4f x(1, 2, 3, 4);
        x.store(ary);
        should_be_close_to(ary[0], 1, epsilon);
        should_be_close_to(ary[1], 2, epsilon);
        should_be_close_to(ary[2], 3, epsilon);
        should_be_close_to(ary[3], 4, epsilon);
    }

}

describe(vec4f, "arithmetic with another vec4f") {
    
    it("should have operator+ for component-wise addition") {
        vec4f a(1,2,3,4);
        vec4f b(10,20,30,40);
        vec4f x = a + b;
        // octave vec4f: [1,2,3,4] + [10,20,30,40]
        should_be_equal_vec4f(x, simd4f_create(11.000000000000000f, 22.000000000000000f, 33.000000000000000f, 44.000000000000000f), epsilon );

    }

    it("should have operator- for component-wise subtraction") {
        vec4f a(1,2,3,4);
        vec4f b(10,20,30,40);
        vec4f x = b - a;
        // octave vec4f:  [10,20,30,40] - [1,2,3,4]
        should_be_equal_vec4f(x, simd4f_create(9.000000000000000f, 18.000000000000000f, 27.000000000000000f, 36.000000000000000f), epsilon );

    }

    it("should have operator* for component-wise multiplication") {
        vec4f a(1,2,3,4);
        vec4f b(10,20,30,40);
        vec4f x = a * b;
        // octave vec4f: [1,2,3,4] .* [10,20,30,40]
        should_be_equal_vec4f(x, simd4f_create(10.000000000000000f, 40.000000000000000f, 90.000000000000000f, 160.000000000000000f), epsilon );

    }

    it("should have operator/ for component-wise division") {
        vec4f a(1,2,3,4);
        vec4f b(10,20,30,40);
        vec4f x = b / a;
        // octave vec4f:  [10,20,30,40] ./ [1,2,3,4]
        should_be_equal_vec4f(x, simd4f_create(10.000000000000000f, 10.000000000000000f, 10.000000000000000f, 10.000000000000000f), epsilon );

    }



	
    it("should have operator+= for component-wise addition") {
        vec4f x(1,2,3,4);
        vec4f b(10,20,30,40);
        x += b;
        // octave vec4f: [1,2,3,4] + [10,20,30,40]
        should_be_equal_vec4f(x, simd4f_create(11.000000000000000f, 22.000000000000000f, 33.000000000000000f, 44.000000000000000f), epsilon );

    }

    it("should have operator-= for component-wise subtraction") {
        vec4f a(1,2,3,4);
        vec4f x(10,20,30,40);
        x -= a;
        // octave vec4f:  [10,20,30,40] - [1,2,3,4]
        should_be_equal_vec4f(x, simd4f_create(9.000000000000000f, 18.000000000000000f, 27.000000000000000f, 36.000000000000000f), epsilon );

    }

    it("should have operator*= for component-wise multiplication") {
        vec4f x(1,2,3,4);
        vec4f b(10,20,30,40);
        x *= b;
        // octave vec4f: [1,2,3,4] .* [10,20,30,40]
        should_be_equal_vec4f(x, simd4f_create(10.000000000000000f, 40.000000000000000f, 90.000000000000000f, 160.000000000000000f), epsilon );

    }

    it("should have operator/= for component-wise division") {
        vec4f a(1,2,3,4);
        vec4f x(10,20,30,40);
        x /= a;
        // octave vec4f:  [10,20,30,40] ./ [1,2,3,4]
        should_be_equal_vec4f(x, simd4f_create(10.000000000000000f, 10.000000000000000f, 10.000000000000000f, 10.000000000000000f), epsilon );

    }


    
}


describe(vec4f, "arithmetic with scalar") {
    
    it("should have operator+ for component-wise addition") {
        vec4f a(1,2,3,4);
        float b=10;
        vec4f x = a + b;
        // octave vec4f: [1,2,3,4] + 10
        should_be_equal_vec4f(x, simd4f_create(11.000000000000000f, 12.000000000000000f, 13.000000000000000f, 14.000000000000000f), epsilon );

    }

    it("should have operator- for component-wise subtraction") {
        float a=10;
        vec4f b(10,20,30,40);
        vec4f x = b - a;
        // octave vec4f:  [10,20,30,40] - 10
        should_be_equal_vec4f(x, simd4f_create(0.000000000000000f, 10.000000000000000f, 20.000000000000000f, 30.000000000000000f), epsilon );

    }

    it("should have operator* for component-wise multiplication") {
        vec4f a(1,2,3,4);
        float b=10;
        vec4f x = a * b;
        // octave vec4f: [1,2,3,4] .* 10
        should_be_equal_vec4f(x, simd4f_create(10.000000000000000f, 20.000000000000000f, 30.000000000000000f, 40.000000000000000f), epsilon );

    }

    it("should have operator/ for component-wise division") {
        vec4f a(10,20,30,40);
        float b=10;
        vec4f x = a / b;
        // octave vec4f: [10,20,30,40] ./ 10
        should_be_equal_vec4f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 3.000000000000000f, 4.000000000000000f), epsilon );

    }



    it("should have operator+ for component-wise addition (float as lhs)") {
        vec4f b(1,2,3,4);
        float a=10;
        vec4f x = a + b;
        // octave vec4f: 10 + [1,2,3,4]
        should_be_equal_vec4f(x, simd4f_create(11.000000000000000f, 12.000000000000000f, 13.000000000000000f, 14.000000000000000f), epsilon );

    }

    it("should have operator- for component-wise subtraction (float as lhs)") {
        float b=50;
        vec4f a(10,20,30,40);
        vec4f x = b - a;
        // octave vec4f:  50 - [10,20,30,40]
        should_be_equal_vec4f(x, simd4f_create(40.000000000000000f, 30.000000000000000f, 20.000000000000000f, 10.000000000000000f), epsilon );

    }

    it("should have operator* for component-wise multiplication (float as lhs)") {
        vec4f b(1,2,3,4);
        float a=10;
        vec4f x = a * b;
        // octave vec4f: 10 .* [1,2,3,4] 
        should_be_equal_vec4f(x, simd4f_create(10.000000000000000f, 20.000000000000000f, 30.000000000000000f, 40.000000000000000f), epsilon );

    }

    it("should have operator* for component-wise multiplication (float as lhs)") {
        vec4f b(10,20,30,40);
        float a=40;
        vec4f x = a / b;
        // octave vec4f: 40 ./ [10,20,30,40] 
        should_be_equal_vec4f(x, simd4f_create(4.000000000000000f, 2.000000000000000f, 1.333333333333333f, 1.000000000000000f), epsilon );

    }

    
}



describe(vec4f, "vector math") {

    it("should have unary minus operator") {
        vec4f a(1,2,3,4);
        vec4f x = -a;
        // octave vec4f: -[1,2,3,4]
        should_be_equal_vec4f(x, simd4f_create(-1.000000000000000f, -2.000000000000000f, -3.000000000000000f, -4.000000000000000f), epsilon );
    }

    it("should have dot function") {
        vec4f a(1,2,3,4);
        vec4f b(6,7,8,9);
        float x = vectorial::dot(a,b);
        
        // octave vec4f: dot([1,2,3,4],[6,7,8,9])
        should_be_close_to(x, 80.000000000000000f, epsilon );
    }

    it("should have length_squared function") {
        vec4f a(1,2,3,4);
        float x = vectorial::length_squared(a);
        
        // octave vec4f: dot([1,2,3,4],[1,2,3,4])
        should_be_close_to(x, 30.000000000000000f, epsilon );
    }

    it("should have length function") {
        vec4f a(1,2,3,4);
        float x = vectorial::length(a);
        
        // octave vec4f: norm([1,2,3,4])
        should_be_close_to(x, 5.477225575051661f, epsilon );
    }
    
    
    it("should have normalize function") {
        vec4f a(1,2,3,4);
        vec4f x = vectorial::normalize(a);
        // octave vec4f: [1,2,3,4] / norm([1,2,3,4])
        should_be_equal_vec4f(x, simd4f_create(0.182574185835055f, 0.365148371670111f, 0.547722557505166f, 0.730296743340221f), epsilon );
    }

}


