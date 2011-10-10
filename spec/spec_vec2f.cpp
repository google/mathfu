#include "spec_helper.h"
#include <iostream>
using vectorial::vec2f;

const int epsilon = 1;

describe(vec2f, "constructing") {
    it("should have default constructor that does nothing..") {
        vec2f x;
    }

    it("should have constructor with element values") {
        vec2f x(10,20);
        // octave vec2f: [10,20]
        should_be_equal_vec2f(x, simd4f_create(10.000000000000000f, 20.000000000000000f, 0.0f, 0.0f), epsilon );
        
    }

    it("should have constructor that loads from a float array") {
        float ary[2] = { 1,2 };
        vec2f x(ary);
        // octave vec2f: [1,2]
        should_be_equal_vec2f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 0.0f, 0.0f), epsilon );
    }

}

describe(vec2f, "loads and stores") {

    it("should have method for loading from a float array") {
        float ary[2] = { 1, 2 };
        vec2f x(-1, -1 );
        x.load(ary);
        // octave vec2f: [1,2]
        should_be_equal_vec2f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 0.0f, 0.0f), epsilon );
    }

    it("should have method for storing to a float array") {
        float ary[2] = { -1, -1 };
        vec2f x(1, 2);
        x.store(ary);
        should_be_close_to(ary[0], 1, epsilon);
        should_be_close_to(ary[1], 2, epsilon);
    }

}


describe(vec2f, "arithmetic with another vec2f") {
    
    it("should have operator+ for component-wise addition") {
        vec2f a(1,2);
        vec2f b(10,20);
        vec2f x = a + b;
        // octave vec2f: [1,2] + [10,20]
        should_be_equal_vec2f(x, simd4f_create(11.000000000000000f, 22.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    it("should have operator- for component-wise subtraction") {
        vec2f a(1,2);
        vec2f b(10,20);
        vec2f x = b - a;
        // octave vec2f:  [10,20] - [1,2]
        should_be_equal_vec2f(x, simd4f_create(9.000000000000000f, 18.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    it("should have operator* for component-wise multiplication") {
        vec2f a(1,2);
        vec2f b(10,20);
        vec2f x = a * b;
        // octave vec2f: [1,2] .* [10,20]
        should_be_equal_vec2f(x, simd4f_create(10.000000000000000f, 40.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    it("should have operator/ for component-wise division") {
        vec2f a(1,2);
        vec2f b(10,20);
        vec2f x = b / a;
        // octave vec2f:  [10,20] ./ [1,2]
        should_be_equal_vec2f(x, simd4f_create(10.000000000000000f, 10.000000000000000f, 0.0f, 0.0f), epsilon );

    }
   


    it("should have operator+= for component-wise addition") {
        vec2f x(1,2);
        vec2f b(10,20);
        x += b;
        // octave vec2f: [1,2] + [10,20]
        should_be_equal_vec2f(x, simd4f_create(11.000000000000000f, 22.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    it("should have operator-= for component-wise subtraction") {
        vec2f a(1,2);
        vec2f x(10,20);
        x -= a;
        // octave vec2f:  [10,20] - [1,2]
        should_be_equal_vec2f(x, simd4f_create(9.000000000000000f, 18.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    it("should have operator*= for component-wise multiplication") {
        vec2f x(1,2);
        vec2f b(10,20);
        x *= b;
        // octave vec2f: [1,2] .* [10,20]
        should_be_equal_vec2f(x, simd4f_create(10.000000000000000f, 40.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    it("should have operator/= for component-wise division") {
        vec2f a(1,2);
        vec2f x(10,20);
        x /= a;
        // octave vec2f:  [10,20] ./ [1,2]
        should_be_equal_vec2f(x, simd4f_create(10.000000000000000f, 10.000000000000000f, 0.0f, 0.0f), epsilon );

    }


}


describe(vec2f, "arithmetic with scalar") {
    
    it("should have operator+ for component-wise addition") {
        vec2f a(1,2);
        float b=10;
        vec2f x = a + b;
        // octave vec2f: [1,2] + 10
        should_be_equal_vec2f(x, simd4f_create(11.000000000000000f, 12.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    it("should have operator- for component-wise subtraction") {
        float a=10;
        vec2f b(10,20);
        vec2f x = b - a;
        // octave vec2f:  [10,20] - 10
        should_be_equal_vec2f(x, simd4f_create(0.000000000000000f, 10.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    it("should have operator* for component-wise multiplication") {
        vec2f a(1,2);
        float b=10;
        vec2f x = a * b;
        // octave vec2f: [1,2] .* 10
        should_be_equal_vec2f(x, simd4f_create(10.000000000000000f, 20.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    it("should have operator/ for component-wise division") {
        vec2f a(10,20);
        float b=10;
        vec2f x = a / b;
        // octave vec2f: [10,20] ./ 10
        should_be_equal_vec2f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 0.0f, 0.0f), epsilon );

    }



    it("should have operator+ for component-wise addition (float as lhs)") {
        vec2f b(1,2);
        float a=10;
        vec2f x = a + b;
        // octave vec2f: 10 + [1,2]
        should_be_equal_vec2f(x, simd4f_create(11.000000000000000f, 12.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    it("should have operator- for component-wise subtraction (float as lhs)") {
        float b=50;
        vec2f a(10,20);
        vec2f x = b - a;
        // octave vec2f:  50 - [10,20]
        should_be_equal_vec2f(x, simd4f_create(40.000000000000000f, 30.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    it("should have operator* for component-wise multiplication (float as lhs)") {
        vec2f b(1,2);
        float a=10;
        vec2f x = a * b;
        // octave vec2f: 10 .* [1,2] 
        should_be_equal_vec2f(x, simd4f_create(10.000000000000000f, 20.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    it("should have operator* for component-wise multiplication (float as lhs)") {
        vec2f b(10,20);
        float a=40;
        vec2f x = a / b;
        // octave vec2f: 40 ./ [10,20] 
        should_be_equal_vec2f(x, simd4f_create(4.000000000000000f, 2.000000000000000f, 0.0f, 0.0f), epsilon );

    }

    
}



describe(vec2f, "vector math") {

    it("should have unary minus operator") {
        vec2f a(1,2);
        vec2f x = -a;
        // octave vec2f: -[1,2]
        should_be_equal_vec2f(x, simd4f_create(-1.000000000000000f, -2.000000000000000f, 0.0f, 0.0f), epsilon );
    }


    it("should have dot function") {
        vec2f a(1,2);
        vec2f b(6,7);
        float x = vectorial::dot(a,b);
        
        // octave vec2f: dot([1,2],[6,7])
        should_be_close_to(x, 20.000000000000000f, epsilon );
    }

    it("should have length_squared function") {
        vec2f a(1,2);
        float x = vectorial::length_squared(a);
        
        // octave vec2f: dot([1,2],[1,2])
        should_be_close_to(x, 5.000000000000000f, epsilon );
    }

    it("should have length function") {
        vec2f a(1,2);
        float x = vectorial::length(a);
        
        // octave vec2f: norm([1,2])
        should_be_close_to(x, 2.236067977499790f, epsilon );
    }
    
    
    it("should have normalize function") {
        vec2f a(1,2);
        vec2f x = vectorial::normalize(a);
        // octave vec2f: [1,2] / norm([1,2])
        should_be_equal_vec2f(x, simd4f_create(0.447213595499958f, 0.894427190999916f, 0.0f, 0.0f), epsilon );
    }

}


