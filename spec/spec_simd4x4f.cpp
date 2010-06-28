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
    

    it("should be possible to set to identity") {
        simd4x4f x;
        simd4x4f_identity(&x);
        
        // octave simd4x4f: [1,0,0,0; 0,1,0,0; 0,0,1,0; 0,0,0,1]
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(1.000000, 0.000000, 0.000000, 0.000000), simd4f_create(0.000000, 1.000000, 0.000000, 0.000000), simd4f_create(0.000000, 0.000000, 1.000000, 0.000000), simd4f_create(0.000000, 0.000000, 0.000000, 1.000000)), epsilon );
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

    it("should have simd4x4f_transpose for transpose") {
        
        simd4x4f in = simd4x4f_create(simd4f_create(1,  2,  3,  4 ),
                                   simd4f_create(5,  6,  7,  8 ),
                                   simd4f_create(9,  10, 11, 12 ),
                                   simd4f_create(13, 14, 15, 16 ));
        
        simd4x4f x;
        simd4x4f_transpose(&in, &x);
        
        // octave simd4x4f: transpose([1,5,9,13 ; 2,6,10,14 ; 3,7,11,15 ; 4,8,12,16 ])
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(1.000000, 5.000000, 9.000000, 13.000000), simd4f_create(2.000000, 6.000000, 10.000000, 14.000000), simd4f_create(3.000000, 7.000000, 11.000000, 15.000000), simd4f_create(4.000000, 8.000000, 12.000000, 16.000000)), epsilon );
    }

    it("should have simd4x4f_matrix_vector_mul for matrix-vector multiply") {
        
        simd4x4f a = simd4x4f_create(simd4f_create( 1,    9,   17,   25 ),
                                     simd4f_create( 3,   11,   19,   27 ),
                                     simd4f_create( 5,   13,   21,   29 ),
                                     simd4f_create( 7,   15,   23,   31 ));

        simd4f b = simd4f_create( 26,  -28,   30,  -32 );
        
        simd4f x;
        simd4x4f_matrix_vector_mul(&a, &b, &x);
        
        // octave simd4f: [1,3,5,7;9,11,13,15;17,19,21,23;25,27,29,31] * [26;-28;30;-32]
        should_be_equal_simd4f(x, simd4f_create(-132.000000, -164.000000, -196.000000, -228.000000), epsilon );
    }


    it("should have simd4x4f_matrix_mul for matrix multiply") {
        
        simd4x4f a = simd4x4f_create(simd4f_create( 1,    9,   17,   25 ),
                                     simd4f_create( 3,   11,   19,   27 ),
                                     simd4f_create( 5,   13,   21,   29 ),
                                     simd4f_create( 7,   15,   23,   31 ));

        simd4x4f b = simd4x4f_create(simd4f_create(  2 , -10,   18 , -26 ),
                                     simd4f_create( -4,   12,  -20,   28 ),
                                     simd4f_create(  6,  -14,   22,  -30 ),
                                     simd4f_create( -8,   16,  -24,   32 ));
        
        simd4x4f x;
        simd4x4f_matrix_mul(&a, &b, &x);
        
        // octave simd4x4f: [1,3,5,7;9,11,13,15;17,19,21,23;25,27,29,31] * [2,-4,6,-8;-10,12,-14,16;18,-20,22,-24;-26,28,-30,32]
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(-120.000000, -248.000000, -376.000000, -504.000000), simd4f_create(128.000000, 256.000000, 384.000000, 512.000000), simd4f_create(-136.000000, -264.000000, -392.000000, -520.000000), simd4f_create(144.000000, 272.000000, 400.000000, 528.000000)), epsilon );
    }
    
    
}

