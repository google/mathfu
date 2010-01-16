#include "spec_helper.h"
#include <iostream>
using vectorial::vec4f;
using vectorial::simd4f;

const float epsilon = 0.00001f;



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

}


