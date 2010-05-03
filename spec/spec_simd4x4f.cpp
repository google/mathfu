#include "spec_helper.h"

const float epsilon = 0.000001f;

describe(simd4x4f, "creating") {
    
    it("should be possible to create with params") {
        
        simd4x4f x = simd4x4f_create(simd4f_create(1,  2,  3,  4 ),
                                   simd4f_create(5,  6,  7,  8 ),
                                   simd4f_create(9,  10, 11, 12 ),
                                   simd4f_create(13, 14, 15, 16 ));

        should_be_equal_simd4f( x.x, simd4f_create(1,  2,  3,  4 ) , epsilon);
        should_be_equal_simd4f( x.y, simd4f_create(5,  6,  7,  8 ) , epsilon);
        should_be_equal_simd4f( x.z, simd4f_create(9,  10, 11, 12 ), epsilon);
        should_be_equal_simd4f( x.w, simd4f_create(13, 14, 15, 16 ), epsilon);

        // octave simd4x4f: [1,5,9,13 ; 2,6,10,14 ; 3,7,11,15 ; 4,8,12,16 ]
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(1.000000, 2.000000, 3.000000, 4.000000), simd4f_create(5.000000, 6.000000, 7.000000, 8.000000), simd4f_create(9.000000, 10.000000, 11.000000, 12.000000), simd4f_create(13.000000, 14.000000, 15.000000, 16.000000)), epsilon );
        
    }
    
    
}


describe(simd4x4f, "matrix utility") {
    
    it("should have simd4x4f_transpose_inplace for transpose") {
        
        simd4x4f x = simd4x4f_create(simd4f_create(1,  2,  3,  4 ),
                                   simd4f_create(5,  6,  7,  8 ),
                                   simd4f_create(9,  10, 11, 12 ),
                                   simd4f_create(13, 14, 15, 16 ));
        
        simd4x4f_transpose_inplace(&x);
        
        // octave simd4x4f: transpose([1,5,9,13 ; 2,6,10,14 ; 3,7,11,15 ; 4,8,12,16 ])
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(1.000000, 5.000000, 9.000000, 13.000000), simd4f_create(2.000000, 6.000000, 10.000000, 14.000000), simd4f_create(3.000000, 7.000000, 11.000000, 15.000000), simd4f_create(4.000000, 8.000000, 12.000000, 16.000000)), epsilon );
    }
    
    
}

