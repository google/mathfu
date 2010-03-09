
#include "spec_helper.h"

const float epsilon = 0.000001f;

describe(simd4f, "creating") {
    
    it("should be possible to create with params") {
        
        simd4f x = simd4f_create(1, 2, 3, 4);

        should_be_close_to( simd4f_getX(x), 1, epsilon);
        should_be_close_to( simd4f_getY(x), 2, epsilon);
        should_be_close_to( simd4f_getZ(x), 3, epsilon);
        should_be_close_to( simd4f_getW(x), 4, epsilon);

        // octave simd4f: [1,2,3,4]
        should_be_equal_simd4f(x, simd4f_create(1.000000, 2.000000, 3.000000, 4.000000), epsilon );
        
    }
    
    it("should have splatting") {
        
        simd4f x = simd4f_splat(42);
        // octave simd4f: [42,42,42,42]
        should_be_equal_simd4f(x, simd4f_create(42.000000, 42.000000, 42.000000, 42.000000), epsilon );
        
    }
    
}

