#include "spec_helper.h"
#include <iostream>
using vectorial::vec4f;

const float epsilon = 0.00001f;

describe(vec4f, "constructing") {
    it("should have default constructor that does nothing..") {
        vec4f x;
    }

    it("should have constructor with element values") {
        vec4f x(10,20,30,40);
        // octave vec4f: [10,20,30,40]
        should_be_equal_vec4f(x, simd4f_create(10.000000, 20.000000, 30.000000, 40.000000), epsilon );
        
    }

}

describe(vec4f, "arithmetic with another vec4f") {
    
    it("should have operator+ for component-wise addition") {
        vec4f a(1,2,3,4);
        vec4f b(10,20,30,40);
        vec4f x = a + b;
        // octave vec4f: [1,2,3,4] + [10,20,30,40]
        should_be_equal_vec4f(x, simd4f_create(11.000000, 22.000000, 33.000000, 44.000000), epsilon );

    }

    it("should have operator- for component-wise subtraction") {
        vec4f a(1,2,3,4);
        vec4f b(10,20,30,40);
        vec4f x = b - a;
        // octave vec4f:  [10,20,30,40] - [1,2,3,4]
        should_be_equal_vec4f(x, simd4f_create(9.000000, 18.000000, 27.000000, 36.000000), epsilon );

    }

    it("should have operator* for component-wise multiplication") {
        vec4f a(1,2,3,4);
        vec4f b(10,20,30,40);
        vec4f x = a * b;
        // octave vec4f: [1,2,3,4] .* [10,20,30,40]
        should_be_equal_vec4f(x, simd4f_create(10.000000, 40.000000, 90.000000, 160.000000), epsilon );

    }

    it("should have operator/ for component-wise division") {
        vec4f a(1,2,3,4);
        vec4f b(10,20,30,40);
        vec4f x = b / a;
        // octave vec4f:  [10,20,30,40] ./ [1,2,3,4]
        should_be_equal_vec4f(x, simd4f_create(10.000000, 10.000000, 10.000000, 10.000000), epsilon );

    }
    
}

/*
describe(vec4f, "vector ops") {
    it("should have length_squared function") {
        vec4f a(1,2,3,4);
        float x = vectorial::length_squared(a);
        
        // octave: dot([1,2,3,4],[1,2,3,4])
        should_be_close_to(x, 30.000000, epsilon );
    }

    it("should have length function") {
        vec4f a(1,2,3,4);
        float x = vectorial::length(a);
        
        // octave: sqrt(dot([1,2,3,4],[1,2,3,4]))
        should_be_close_to(x, 5.477226, epsilon );
    }
    
    it("should have dot function") {
        vec4f a(1,2,3,4);
        vec4f b(6,7,8,9);
        float x = vectorial::dot(a,b);
        
        // octave: dot([1,2,3,4],[6,7,8,9])
        should_be_close_to(x, 80.000000, epsilon );
    }
    
    it("should have normalize function") {
        vec4f a(1,2,3,4);
        vec4f x = vectorial::normalize(a);
        // octave: [1,2,3,4] / sqrt(dot([1,2,3,4],[1,2,3,4]))
        should_be_equal_simd4f(x, simd4f(0.182574, 0.365148, 0.547723, 0.730297), epsilon );
    }

}


*/