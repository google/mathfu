#include "spec_helper.h"
#include <iostream>
using vectorial::vec4f;

const float epsilon = 0.00001f;

describe(vec4f, "sanity") {
    it("has simd-type") {
        std::cout << "Using simd: " << VECTORIAL_SIMD_TYPE << std::endl;
    }
}

describe(vec4f, "arithmetics") {
    
    
    it("should add") {
        
        vec4f a(1,2,3,4);
        vec4f b(2,3,4,5);
        
        vec4f x = a + b;
        
        // octave vec4: [1,2,3,4] + [2,3,4,5]
        should_be_equal_vec3(x, vec4f(3.000000, 5.000000, 7.000000, 9.000000), epsilon );
        
    }
    
}
