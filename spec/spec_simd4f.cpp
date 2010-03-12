
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

describe(simd4f, "arithmetic") {

    it("should have simd4f_add for addition with another simd4f") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(10,20,30,40);
        
        simd4f x = simd4f_add(a,b);
        // octave simd4f: [1,2,3,4] + [10,20,30,40]
        should_be_equal_simd4f(x, simd4f_create(11.000000, 22.000000, 33.000000, 44.000000), epsilon );
    }

    it("should have simd4f_sub for substraction with another simd4f") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(10,20,30,40);
        
        simd4f x = simd4f_sub(b,a);
        // octave simd4f: [10,20,30,40] - [1,2,3,4] 
        should_be_equal_simd4f(x, simd4f_create(9.000000, 18.000000, 27.000000, 36.000000), epsilon );
    }

}
