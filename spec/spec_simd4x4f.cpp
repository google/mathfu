#include "spec_helper.h"

const int epsilon = 1;

#ifndef M_PI
#define M_PI 3.141592f
#endif

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
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(1.000000000000000f, 2.000000000000000f, 3.000000000000000f, 4.000000000000000f), simd4f_create(5.000000000000000f, 6.000000000000000f, 7.000000000000000f, 8.000000000000000f), simd4f_create(9.000000000000000f, 10.000000000000000f, 11.000000000000000f, 12.000000000000000f), simd4f_create(13.000000000000000f, 14.000000000000000f, 15.000000000000000f, 16.000000000000000f)), epsilon );
        
    }
    

    it("should be possible to set to identity") {
        simd4x4f x;
        simd4x4f_identity(&x);
        
        // octave simd4x4f: [1,0,0,0; 0,1,0,0; 0,0,1,0; 0,0,0,1]
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(1.000000000000000f, 0.000000000000000f, 0.000000000000000f, 0.000000000000000f), simd4f_create(0.000000000000000f, 1.000000000000000f, 0.000000000000000f, 0.000000000000000f), simd4f_create(0.000000000000000f, 0.000000000000000f, 1.000000000000000f, 0.000000000000000f), simd4f_create(0.000000000000000f, 0.000000000000000f, 0.000000000000000f, 1.000000000000000f)), epsilon );
    }
}


describe(simd4x4f, "loading and storing") {

    it("should be possible to load from array of 16 floats with simd4x4f_uload") {
        
        simd4x4f x;
        float f[16] = {1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16 };
        simd4x4f_uload(&x, f);

        should_be_equal_simd4x4f(x, simd4x4f_create( simd4f_create(1,2,3,4),
                                                     simd4f_create(5,6,7,8),
                                                     simd4f_create(9,10,11,12),
                                                     simd4f_create(13,14,15,16) ), epsilon);
        
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
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(1.000000000000000f, 5.000000000000000f, 9.000000000000000f, 13.000000000000000f), simd4f_create(2.000000000000000f, 6.000000000000000f, 10.000000000000000f, 14.000000000000000f), simd4f_create(3.000000000000000f, 7.000000000000000f, 11.000000000000000f, 15.000000000000000f), simd4f_create(4.000000000000000f, 8.000000000000000f, 12.000000000000000f, 16.000000000000000f)), epsilon );
    }

    it("should have simd4x4f_transpose for transpose") {
        
        simd4x4f in = simd4x4f_create(simd4f_create(1,  2,  3,  4 ),
                                   simd4f_create(5,  6,  7,  8 ),
                                   simd4f_create(9,  10, 11, 12 ),
                                   simd4f_create(13, 14, 15, 16 ));
        
        simd4x4f x;
        simd4x4f_transpose(&in, &x);
        
        // octave simd4x4f: transpose([1,5,9,13 ; 2,6,10,14 ; 3,7,11,15 ; 4,8,12,16 ])
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(1.000000000000000f, 5.000000000000000f, 9.000000000000000f, 13.000000000000000f), simd4f_create(2.000000000000000f, 6.000000000000000f, 10.000000000000000f, 14.000000000000000f), simd4f_create(3.000000000000000f, 7.000000000000000f, 11.000000000000000f, 15.000000000000000f), simd4f_create(4.000000000000000f, 8.000000000000000f, 12.000000000000000f, 16.000000000000000f)), epsilon );
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
        should_be_equal_simd4f(x, simd4f_create(-132.000000000000000f, -164.000000000000000f, -196.000000000000000f, -228.000000000000000f), epsilon );
    }

    it("should have simd4x4f_matrix_vector3_mul for matrix-vector3 multiply") {
        
        simd4x4f a = simd4x4f_create(simd4f_create( 1,    9,   17,   25 ),
                                     simd4f_create( 3,   11,   19,   27 ),
                                     simd4f_create( 5,   13,   21,   29 ),
                                     simd4f_create( 7,   15,   23,   31 ));

        simd4f b = simd4f_create( 26,  -28,   30,  -32 );
        
        simd4f x;
        simd4x4f_matrix_vector3_mul(&a, &b, &x);
        
        // TODO octave simd4f: 
        
    }

    it("should have simd4x4f_matrix_vector3_mul for matrix-vector3 multiply") {
        
        simd4x4f a = simd4x4f_create(simd4f_create( 1,    9,   17,   25 ),
                                     simd4f_create( 3,   11,   19,   27 ),
                                     simd4f_create( 5,   13,   21,   29 ),
                                     simd4f_create( 7,   15,   23,   31 ));

        simd4f b = simd4f_create( 26,  -28,   30,  -32 );
        
        simd4f x;
        simd4x4f_matrix_vector3_mul(&a, &b, &x);
        
        // TODO octave simd4f: 
        
    }

    it("should have simd4x4f_matrix_point3_mul") { /* TODO */ }

    it("should have simd4x4f_inv_ortho_matrix_point3_mul for transforming point with inverse of a orhtonormal matrix") {

        simd4x4f a = simd4x4f_create(simd4f_create( 0,  -1,   0,   0 ),
                                     simd4f_create( 1,   0,   0,   0 ),
                                     simd4f_create( 0,   0,   1,   0 ),
                                     simd4f_create( 1,   2,   3,   1 ));

        simd4f b = simd4f_create(5,6,7,0);

        simd4f x;
        simd4x4f_inv_ortho_matrix_point3_mul(&a, &b, &x);

        // octave simd4f: inverse([0,1,0,1; -1,0,0,2; 0,0,1,3; 0,0,0,1]) * [5;6;7;1] .* [1;1;1;0]
        should_be_equal_simd4f(x, simd4f_create(-4.000000000000000f, 4.000000000000000f, 4.000000000000000f, 0.000000000000000f), epsilon );
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
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(-120.000000000000000f, -248.000000000000000f, -376.000000000000000f, -504.000000000000000f), simd4f_create(128.000000000000000f, 256.000000000000000f, 384.000000000000000f, 512.000000000000000f), simd4f_create(-136.000000000000000f, -264.000000000000000f, -392.000000000000000f, -520.000000000000000f), simd4f_create(144.000000000000000f, 272.000000000000000f, 400.000000000000000f, 528.000000000000000f)), epsilon );
    }
    
    
    
    
    it("should have simd4x4f_inverse for calculating inverse matrix") {
        
        simd4x4f a = simd4x4f_create(simd4f_create(7,  2,  87,  5 ),
                                   simd4f_create(5,  24,  6,  3 ),
                                   simd4f_create(4,  6, 5, 6 ),
                                   simd4f_create(5, 7, 4, 6 ));
        
        simd4x4f x;
        simd4x4f_inverse(&a, &x);
        
        // octave simd4x4f: inverse( [7,5,4,5 ; 2,24,6,7 ; 87,6,5,4 ; 5,3,6,6] )
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(0.015309310560300f, -0.049885440533222f, -1.081337221412206f, 1.093522182878568f), simd4f_create(-0.004061653822120f, 0.054051239325141f, 0.123620079150177f, -0.147260987294314f), simd4f_create(0.011247656738180f, 0.004165798791918f, 0.042282857737971f, -0.053738804415747f), simd4f_create(-0.015517600499896f, -0.024265777962924f, 0.728702353676318f, -0.536971464278276f)), epsilon );

        simd4x4f x2;
        simd4x4f_matrix_mul(&x, &a, &x2);
        simd4x4f identity;
        simd4x4f_identity(&identity);
        // Allow larger error for M * M' = I
        const int epsilon = 0x35100000; 
        should_be_equal_simd4x4f(x2, identity, epsilon);
        
    }
    
    
    
}


describe(simd4x4f, "math on elements") {
    
    it("should have simd4x4f_add for element-wise addition") {
        
        simd4x4f a = simd4x4f_create(simd4f_create(1,  2,  3,  4 ),
                                     simd4f_create(5,  6,  7,  8 ),
                                     simd4f_create(9,  10, 11, 12 ),
                                     simd4f_create(13, 14, 15, 16 ));
        
        simd4x4f b = simd4x4f_create(simd4f_create(  2 , -10,   18 , -26 ),
                                     simd4f_create( -4,   12,  -20,   28 ),
                                     simd4f_create(  6,  -14,   22,  -30 ),
                                     simd4f_create( -8,   16,  -24,   32 ));

        simd4x4f x;
        
        simd4x4f_add(&a, &b, &x);
                                 

        // octave simd4x4f: [1,5,9,13 ; 2,6,10,14 ; 3,7,11,15 ; 4,8,12,16 ] + [2,-4,6,-8;-10,12,-14,16;18,-20,22,-24;-26,28,-30,32]
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(3.000000000000000f, -8.000000000000000f, 21.000000000000000f, -22.000000000000000f), simd4f_create(1.000000000000000f, 18.000000000000000f, -13.000000000000000f, 36.000000000000000f), simd4f_create(15.000000000000000f, -4.000000000000000f, 33.000000000000000f, -18.000000000000000f), simd4f_create(5.000000000000000f, 30.000000000000000f, -9.000000000000000f, 48.000000000000000f)), epsilon );
        
    }

    it("should have simd4x4f_sub for element-wise substraction") {
        
        simd4x4f a = simd4x4f_create(simd4f_create(1,  2,  3,  4 ),
                                     simd4f_create(5,  6,  7,  8 ),
                                     simd4f_create(9,  10, 11, 12 ),
                                     simd4f_create(13, 14, 15, 16 ));
        
        simd4x4f b = simd4x4f_create(simd4f_create(  2 , -10,   18 , -26 ),
                                     simd4f_create( -4,   12,  -20,   28 ),
                                     simd4f_create(  6,  -14,   22,  -30 ),
                                     simd4f_create( -8,   16,  -24,   32 ));

        simd4x4f x;
        
        simd4x4f_sub(&a, &b, &x);
                                 

        // octave simd4x4f: [1,5,9,13 ; 2,6,10,14 ; 3,7,11,15 ; 4,8,12,16 ] - [2,-4,6,-8;-10,12,-14,16;18,-20,22,-24;-26,28,-30,32]
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(-1.000000000000000f, 12.000000000000000f, -15.000000000000000f, 30.000000000000000f), simd4f_create(9.000000000000000f, -6.000000000000000f, 27.000000000000000f, -20.000000000000000f), simd4f_create(3.000000000000000f, 24.000000000000000f, -11.000000000000000f, 42.000000000000000f), simd4f_create(21.000000000000000f, -2.000000000000000f, 39.000000000000000f, -16.000000000000000f)), epsilon );
        
    }

    it("should have simd4x4f_mul for element-wise multiplication") {
        
        simd4x4f a = simd4x4f_create(simd4f_create(1,  2,  3,  4 ),
                                     simd4f_create(5,  6,  7,  8 ),
                                     simd4f_create(9,  10, 11, 12 ),
                                     simd4f_create(13, 14, 15, 16 ));
        
        simd4x4f b = simd4x4f_create(simd4f_create(  2 , -10,   18 , -26 ),
                                     simd4f_create( -4,   12,  -20,   28 ),
                                     simd4f_create(  6,  -14,   22,  -30 ),
                                     simd4f_create( -8,   16,  -24,   32 ));

        simd4x4f x;
        
        simd4x4f_mul(&a, &b, &x);
                                 

        // octave simd4x4f: [1,5,9,13 ; 2,6,10,14 ; 3,7,11,15 ; 4,8,12,16 ] .* [2,-4,6,-8;-10,12,-14,16;18,-20,22,-24;-26,28,-30,32]
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(2.000000000000000f, -20.000000000000000f, 54.000000000000000f, -104.000000000000000f), simd4f_create(-20.000000000000000f, 72.000000000000000f, -140.000000000000000f, 224.000000000000000f), simd4f_create(54.000000000000000f, -140.000000000000000f, 242.000000000000000f, -360.000000000000000f), simd4f_create(-104.000000000000000f, 224.000000000000000f, -360.000000000000000f, 512.000000000000000f)), epsilon );
        
    }

    it("should have simd4x4f_div for element-wise division") {
        
        simd4x4f a = simd4x4f_create(simd4f_create(1,  2,  3,  4 ),
                                     simd4f_create(5,  6,  7,  8 ),
                                     simd4f_create(9,  10, 11, 12 ),
                                     simd4f_create(13, 14, 15, 16 ));
        
        simd4x4f b = simd4x4f_create(simd4f_create(  2 , -10,   18 , -26 ),
                                     simd4f_create( -4,   12,  -20,   28 ),
                                     simd4f_create(  6,  -14,   22,  -30 ),
                                     simd4f_create( -8,   16,  -24,   32 ));

        simd4x4f x;
        
        simd4x4f_div(&a, &b, &x);
                                 

        // octave simd4x4f: [1,5,9,13 ; 2,6,10,14 ; 3,7,11,15 ; 4,8,12,16 ] ./ [2,-4,6,-8;-10,12,-14,16;18,-20,22,-24;-26,28,-30,32]
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(0.500000000000000f, -0.200000000000000f, 0.166666666666667f, -0.153846153846154f), simd4f_create(-1.250000000000000f, 0.500000000000000f, -0.350000000000000f, 0.285714285714286f), simd4f_create(1.500000000000000f, -0.714285714285714f, 0.500000000000000f, -0.400000000000000f), simd4f_create(-1.625000000000000f, 0.875000000000000f, -0.625000000000000f, 0.500000000000000f)), epsilon );
        
    }
    
    
}


describe(simd4x4f, "creating projection and view matrices") {

    it("should have simd4x4f_perspective for creating perspective projection matrix") {
        
        const float fov = 10.0f * M_PI / 180.0f;
        const float aspect = 1.6f;
        const float znear = 2.0f;
        const float zfar = 50.0f;

        const int epsilon = 50;
        
        simd4x4f x;
        simd4x4f_perspective(&x, fov, aspect, znear, zfar);
        
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(7.14378, 0, 0, 0),
                                                    simd4f_create(0, 11.4301, 0, 0),
                                                    simd4f_create(0, 0, -1.08333, -1),
                                                    simd4f_create(-0, -0, -4.16667, -0)), epsilon);
        
        
    }

    it("should have simd4x4f_ortho for creating orthogonal projection matrix") {
        

        simd4x4f x;
        simd4x4f_ortho(&x, -10, 20, -30, 40, -50, 60);
        const int epsilon = 20;        
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(0.0666667, 0, 0, 0),
                                                    simd4f_create(0, 0.0285714, 0, 0),
                                                    simd4f_create(-0, -0, -0.0181818, -0),
                                                    simd4f_create(-0.333333, -0.142857, -0.0909091, 1)), epsilon);
        
        
    }
    
    it("should have simd4x4f_lookat for creating look-at matrix") {
        
        simd4f eye = simd4f_create(1,2,3,0);
        simd4f center = simd4f_create(3,4,5,0);
        simd4f up = simd4f_create(0,1,0,0);

        simd4x4f x;
        simd4x4f_lookat(&x, eye, center, up);

        const int epsilon = 40;
        
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(-0.707107, -0.408248, -0.57735, 0),
                                                    simd4f_create(0, 0.816497, -0.57735, 0),
                                                    simd4f_create(0.707107, -0.408248, -0.57735, 0),
                                                    simd4f_create(-1.41421, 0, 3.4641, 1)), epsilon);

        
    }
    
    
    it("should have simd4x4f_translation for creating translation matrix") {
        
        simd4x4f x;
        simd4x4f_translation(&x, 1,2,3);

        // octave simd4x4f: [1,0,0,1; 0,1,0,2; 0,0,1,3; 0,0,0,1]
        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(1.000000000000000f, 0.000000000000000f, 0.000000000000000f, 0.000000000000000f), simd4f_create(0.000000000000000f, 1.000000000000000f, 0.000000000000000f, 0.000000000000000f), simd4f_create(0.000000000000000f, 0.000000000000000f, 1.000000000000000f, 0.000000000000000f), simd4f_create(1.000000000000000f, 2.000000000000000f, 3.000000000000000f, 1.000000000000000f)), epsilon );
    }

    it("should have simd4x4f_axis_rotation for creating a rotation matrix along a axis") {
        
        simd4x4f x;

        simd4x4f_axis_rotation(&x, 45 * M_PI / 180.0f, simd4f_create(1,2,3,0));

        const int epsilon = 20;

        should_be_equal_simd4x4f(x, simd4x4f_create(simd4f_create(0.728028, 0.608789, -0.315202, 0),
                                                   simd4f_create(-0.525105, 0.790791, 0.314508, 0),
                                                   simd4f_create(0.440727, -0.0634566, 0.895395, 0),
                                                   simd4f_create(0, 0, 0, 1)), epsilon);


    }
    
    
}


