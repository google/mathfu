
#include "spec_helper.h"

const float TOLERANCE = 0.001f;

describe(simd4f, "creating") {
    
    it("should be possible to create with params") {
        
        simd4f s = simd4f_create(1, 2, 3, 4);

        should_be_close_to( simd4f_getX(s), 1, TOLERANCE);
        should_be_close_to( simd4f_getY(s), 2, TOLERANCE);
        should_be_close_to( simd4f_getZ(s), 3, TOLERANCE);
        should_be_close_to( simd4f_getW(s), 4, TOLERANCE);

        should_be_equal_simd4f( s, simd4f_create(1,2,3,4), TOLERANCE );

        
    }    
}

