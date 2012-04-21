
#include "spec_helper.h"

const int epsilon = 1;

describe(simd4f, "sanity") {
    it("VECTORIAL_SIMD_TYPE should be defined to a string") {
        std::cout << "Simd type: " << VECTORIAL_SIMD_TYPE << std::endl;
    }
}

describe(simd4f, "creating") {
    
    it("should be possible to create with simd4f_create") {
        
        simd4f x = simd4f_create(1, 2, 3, 4);

        should_be_close_to( simd4f_get_x(x), 1, epsilon);
        should_be_close_to( simd4f_get_y(x), 2, epsilon);
        should_be_close_to( simd4f_get_z(x), 3, epsilon);
        should_be_close_to( simd4f_get_w(x), 4, epsilon);

        // octave simd4f: [1,2,3,4]
        should_be_equal_simd4f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 3.000000000000000f, 4.000000000000000f), epsilon );
        
    }

    it("should have simd4f_zero for zero vector") {

        simd4f x = simd4f_zero();

        // octave simd4f: [0,0,0,0]
        should_be_equal_simd4f(x, simd4f_create(0.000000000000000f, 0.000000000000000f, 0.000000000000000f, 0.000000000000000f), epsilon );
    }
    
    
}
#ifdef _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#endif

#define unaligned_mem(n) ((float*)((unsigned char*)alloca(sizeof(float)*n+4)+4))

describe(simd4f, "utilities") {

    it("should have simd4f_uload4 for loading four float values from an unaligned float array into simd4f") {
        float *f = unaligned_mem(4);
        f[0] = 1;
        f[1] = 2;
        f[2] = 3;
        f[3] = 4;
        simd4f x = simd4f_uload4(f);
        // octave simd4f: [1,2,3,4]
        should_be_equal_simd4f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 3.000000000000000f, 4.000000000000000f), epsilon );
    }

    it("should have simd4f_uload3 for loading three float values from an unaligned float array into simd4f") {
        float *f = unaligned_mem(3);
        f[0] = 1;
        f[1] = 2;
        f[2] = 3;
        simd4f x = simd4f_uload3(f);
        // octave simd4f: [1,2,3]
        should_be_equal_simd4f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 3.000000000000000f, 0.0f), epsilon );
    }

    it("should have simd4f_uload2 for loading two float values from float an unaligned array into simd4f") {
        float *f = unaligned_mem(2);
        f[0] = 1;
        f[1] = 2;
        simd4f x = simd4f_uload2(f);
        // octave simd4f: [1,2]
        should_be_equal_simd4f(x, simd4f_create(1.000000000000000f, 2.000000000000000f, 0.0f, 0.0f), epsilon );
    }


    it("should have simd4f_ustore4 for storing four float values from simd4f to an unaligned array") {
        float *f = unaligned_mem(4);
        f[0] = -1;
        f[1] = -1;
        f[2] = -1;
        f[3] = -1;
        simd4f a = simd4f_create(1,2,3,4);
        simd4f_ustore4(a, f);
        should_be_close_to(f[0], 1, epsilon);
        should_be_close_to(f[1], 2, epsilon);
        should_be_close_to(f[2], 3, epsilon);
        should_be_close_to(f[3], 4, epsilon);
    }

    it("should have simd4f_ustore3 for storing three float values from simd4f to an unaligned array") {
        float *f = unaligned_mem(3);
        f[0] = -1;
        f[1] = -1;
        f[2] = -1;
        simd4f a = simd4f_create(1,2,3,4);
        simd4f_ustore3(a, f);
        should_be_close_to(f[0], 1, epsilon);
        should_be_close_to(f[1], 2, epsilon);
        should_be_close_to(f[2], 3, epsilon);
    }

    it("should have simd4f_ustore2 for storing two float values from simd4f to an unaligned array") {
        float *f = unaligned_mem(2);
        f[0] = -1;
        f[1] = -1;
        simd4f a = simd4f_create(1,2,3,4);
        simd4f_ustore2(a, f);
        should_be_close_to(f[0], 1, epsilon);
        should_be_close_to(f[1], 2, epsilon);
    }




    it("should have simd4f_splat that expands a single scalar to all elements") {
        simd4f x = simd4f_splat(42);
        // octave simd4f: [42,42,42,42]
        should_be_equal_simd4f(x, simd4f_create(42.000000000000000f, 42.000000000000000f, 42.000000000000000f, 42.000000000000000f), epsilon );
    }

    it("should have simd4f_splat_x,y,z,w splatting of an element") {
        simd4f a = simd4f_create(1,2,3,4);

        simd4f x;
        
        x = simd4f_splat_x(a);
        // octave simd4f: [1,1,1,1]
        should_be_equal_simd4f(x, simd4f_create(1.000000000000000f, 1.000000000000000f, 1.000000000000000f, 1.000000000000000f), epsilon );

        x = simd4f_splat_y(a);
        // octave simd4f: [2,2,2,2]
        should_be_equal_simd4f(x, simd4f_create(2.000000000000000f, 2.000000000000000f, 2.000000000000000f, 2.000000000000000f), epsilon );

        x = simd4f_splat_z(a);
        // octave simd4f: [3,3,3,3]
        should_be_equal_simd4f(x, simd4f_create(3.000000000000000f, 3.000000000000000f, 3.000000000000000f, 3.000000000000000f), epsilon );

        x = simd4f_splat_w(a);
        // octave simd4f: [4,4,4,4]
        should_be_equal_simd4f(x, simd4f_create(4.000000000000000f, 4.000000000000000f, 4.000000000000000f, 4.000000000000000f), epsilon );
    }
    
    it("should have simd4f_sum that adds elements") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f x = simd4f_sum(a);
        // octave simd4f: [sum([1,2,3,4]), sum([1,2,3,4]), sum([1,2,3,4]), sum([1,2,3,4])]
        should_be_equal_simd4f(x, simd4f_create(10.000000000000000f, 10.000000000000000f, 10.000000000000000f, 10.000000000000000f), epsilon );
        
    }
        
    it("should have simd4f_reciprocal") {
        simd4f a = simd4f_create(0.00001f, 2.00001f, 3.0f, 99999999.0f);
        simd4f x = simd4f_reciprocal(a);
        // octave simd4f: 1 ./ [0.00001, 2.00001, 3.0, 99999999.0]
        should_be_equal_simd4f(x, simd4f_create(99999.999999999985448f, 0.499997500012500f, 0.333333333333333f, 0.000000010000000f), epsilon );
    }

    it("should have simd4f_sqrt") {
        simd4f a = simd4f_create(0.00001f, 2.00001f, 3.0f, 99999999.0f);
        simd4f x = simd4f_sqrt(a);
        // octave simd4f:  sqrt([0.00001, 2.00001, 3.0, 99999999.0])
        should_be_equal_simd4f(x, simd4f_create(0.003162277660168f, 1.414217097902582f, 1.732050807568877f, 9999.999949999999444f), epsilon );
    }

    it("should have simd4f_rsqrt for reciprocal of square-root") {
        simd4f a = simd4f_create(0.00001f, 2.00001f, 3.0f, 99999999.0f);
        simd4f x = simd4f_rsqrt(a);
        const int epsilon = 4; // Grant larger error
        // octave simd4f:  1 ./ sqrt([0.00001, 2.00001, 3.0, 99999999.0])
        should_be_equal_simd4f(x, simd4f_create(316.227766016837904f, 0.707105013426224f, 0.577350269189626f, 0.000100000000500f), epsilon );
    }

}

describe(simd4f, "arithmetic with another simd4f") {

    it("should have simd4f_add for component-wise addition") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(10,20,30,40);
        
        simd4f x = simd4f_add(a,b);
        // octave simd4f: [1,2,3,4] + [10,20,30,40]
        should_be_equal_simd4f(x, simd4f_create(11.000000000000000f, 22.000000000000000f, 33.000000000000000f, 44.000000000000000f), epsilon );
    }

    it("should have simd4f_sub for component-wise subtraction") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(10,20,30,40);
        
        simd4f x = simd4f_sub(b,a);
        // octave simd4f: [10,20,30,40] - [1,2,3,4] 
        should_be_equal_simd4f(x, simd4f_create(9.000000000000000f, 18.000000000000000f, 27.000000000000000f, 36.000000000000000f), epsilon );
    }

    it("should have simd4f_mul for component-wise multiply") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(10,20,30,40);
        
        simd4f x = simd4f_mul(a,b);
        // octave simd4f: [1,2,3,4] .* [10,20,30,40]
        should_be_equal_simd4f(x, simd4f_create(10.000000000000000f, 40.000000000000000f, 90.000000000000000f, 160.000000000000000f), epsilon );
    }

    it("should have simd4f_div for component-wise division") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(10,20,30,40);
        
        simd4f x = simd4f_div(b,a);
        // octave simd4f: [10,20,30,40] ./ [1,2,3,4] 
        should_be_equal_simd4f(x, simd4f_create(10.000000000000000f, 10.000000000000000f, 10.000000000000000f, 10.000000000000000f), epsilon );
    }

    it("should have simd4f_madd for multiply-add") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(100,100,100,100);
        simd4f c = simd4f_create(6,7,8,9);

        simd4f x = simd4f_madd(a,b,c);
        // octave simd4f: [1,2,3,4] .* [100,100,100,100] .+ [6,7,8,9]
        should_be_equal_simd4f(x, simd4f_create(106.000000000000000f, 207.000000000000000f, 308.000000000000000f, 409.000000000000000f), epsilon );

    }

}


describe(simd4f, "vector math") {
    
    it("should have simd4f_dot4 for four component dot product") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(10,20,30,40);
        
        simd4f x = simd4f_dot4(a,b);
        // octave simd4f: [dot([1, 2, 3, 4], [10, 20, 30, 40]),dot([1, 2, 3, 4], [10, 20, 30, 40]),dot([1, 2, 3, 4], [10, 20, 30, 40]),dot([1, 2, 3, 4], [10, 20, 30, 40])]
        should_be_equal_simd4f(x, simd4f_create(300.000000000000000f, 300.000000000000000f, 300.000000000000000f, 300.000000000000000f), epsilon );
    }

    it("should have simd4f_dot3 for three component dot product") {
        simd4f a = simd4f_create(1,2,3,9999);
        simd4f b = simd4f_create(10,20,30,-9990);
        
        simd4f x = simd4f_dot3(a,b);
        // octave simd4f: [dot([1, 2, 3], [10, 20, 30]),dot([1, 2, 3], [10, 20, 30]),dot([1, 2, 3], [10, 20, 30]),dot([1, 2, 3], [10, 20, 30])]
        should_be_equal_simd4f(x, simd4f_create(140.000000000000000f, 140.000000000000000f, 140.000000000000000f, 140.000000000000000f), epsilon );
    }

    it("should have simd4f_dot2 for two component dot product") {
        simd4f a = simd4f_create(1,2,3,9999);
        simd4f b = simd4f_create(10,20,30,-9990);
        
        simd4f x = simd4f_dot2(a,b);
        // octave simd4f: [dot([1, 2], [10, 20]),dot([1, 2], [10, 20]),dot([1, 2], [10, 20]),dot([1, 2], [10, 20])]
        should_be_equal_simd4f(x, simd4f_create(50.000000000000000f, 50.000000000000000f, 50.000000000000000f, 50.000000000000000f), epsilon );
    }
    
    it("should have simd4f_length4 for four component vector length") {
        simd4f a = simd4f_create(1,2,-3,9999);
        simd4f x = simd4f_length4(a);
        // octave simd4f: [norm([1,2,-3,9999]), norm([1,2,-3,9999]), norm([1,2,-3,9999]), norm([1,2,-3,9999])]
        should_be_equal_simd4f(x, simd4f_create(9999.000700069982486f, 9999.000700069982486f, 9999.000700069982486f, 9999.000700069982486f), epsilon );

    }

    it("should have simd4f_length3 for three component vector length") {
        simd4f a = simd4f_create(1,2,-3,9999);
        simd4f x = simd4f_length3(a);
        // octave simd4f: [norm([1,2,-3]), norm([1,2,-3]), norm([1,2,-3]), norm([1,2,-3])]
        should_be_equal_simd4f(x, simd4f_create(3.741657386773941f, 3.741657386773941f, 3.741657386773941f, 3.741657386773941f), epsilon );

    }

    it("should have simd4f_length2 for two component vector length") {
        simd4f a = simd4f_create(1,2,-3,9999);
        simd4f x = simd4f_length2(a);
        // octave simd4f: [norm([1,2]),norm([1,2]),norm([1,2]),norm([1,2])]
        should_be_equal_simd4f(x, simd4f_create(2.236067977499790f, 2.236067977499790f, 2.236067977499790f, 2.236067977499790f), epsilon );

    }


    it("should have simd4f_length4_squared for four component squared vector length") {
        simd4f a = simd4f_create(1,2,-3,9999);
        simd4f x = simd4f_length4_squared(a);
        // octave simd4f: ([(dot([1,2,-3,9999], [1,2,-3,9999])), (dot([1,2,-3,9999], [1,2,-3,9999])), (dot([1,2,-3,9999], [1,2,-3,9999])), (dot([1,2,-3,9999], [1,2,-3,9999]))])
        should_be_equal_simd4f(x, simd4f_create(99980015.000000000000000f, 99980015.000000000000000f, 99980015.000000000000000f, 99980015.000000000000000f), epsilon );

    }

    it("should have simd4f_length3_squared for three component squared vector length") {
        simd4f a = simd4f_create(1,2,-3,9999);
        simd4f x = simd4f_length3_squared(a);
        // octave simd4f: ([dot([1,2,-3], [1,2,-3]), dot([1,2,-3], [1,2,-3]), dot([1,2,-3], [1,2,-3]), dot([1,2,-3], [1,2,-3])])
        should_be_equal_simd4f(x, simd4f_create(14.000000000000000f, 14.000000000000000f, 14.000000000000000f, 14.000000000000000f), epsilon );

    }

    it("should have simd4f_length2_squared for two component squared vector length") {
        simd4f a = simd4f_create(1,2,-3,9999);
        simd4f x = simd4f_length2_squared(a);
        // octave simd4f: ([dot([1,2], [1,2]), dot([1,2], [1,2]), dot([1,2], [1,2]), dot([1,2], [1,2])])
        should_be_equal_simd4f(x, simd4f_create(5.000000000000000f, 5.000000000000000f, 5.000000000000000f, 5.000000000000000f), epsilon );

    }
    
    
    
    it("should have simd4f_cross3 for cross product") {
        simd4f a = simd4f_create(1,12,3,-9999);
        simd4f b = simd4f_create(5,6,-17, 9999);

        simd4f x = simd4f_cross3(a,b);
        // octave simd4f: horzcat(  cross( [1,12,3], [5,6,-17] )  , [0] )
        should_be_equal_simd4f(x, simd4f_create(-222.000000000000000f, 32.000000000000000f, -54.000000000000000f, 0.000000000000000f), epsilon );

    }
    
    it("should have simd4f_normalize4 for normalizing four const vector to unit length") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f x = simd4f_normalize4(a);
        // octave simd4f: [1,2,3,4] / norm([1,2,3,4])
        should_be_equal_simd4f(x, simd4f_create(0.182574185835055f, 0.365148371670111f, 0.547722557505166f, 0.730296743340221f), epsilon );
    }

    it("should have simd4f_normalize3 for normalizing three component vector to unit length") {
        simd4f a = simd4f_create(1,2,3,0);
        simd4f x = simd4f_normalize4(a);
        // octave simd4f: [1,2,3,0] / norm([1,2,3])
        should_be_equal_simd4f(x, simd4f_create(0.267261241912424f, 0.534522483824849f, 0.801783725737273f, 0.000000000000000f), epsilon );
    }

    it("should have simd4f_normalize2 for normalizing two component vector to unit length") {
        simd4f a = simd4f_create(1,2,0,0);
        simd4f x = simd4f_normalize4(a);
        // octave simd4f: [1,2,0,0] / norm([1,2])
        should_be_equal_simd4f(x, simd4f_create(0.447213595499958f, 0.894427190999916f, 0.000000000000000f, 0.000000000000000f), epsilon );
    }

    
}

describe(simd4f, "shuffles and merges") {
    
    it("should have simd4f_shuffle_wxyz") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f x = simd4f_shuffle_wxyz(a);
        should_be_equal_simd4f(x, simd4f_create(4,1,2,3), epsilon );
    }

    it("should have simd4f_shuffle_zwxy") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f x = simd4f_shuffle_zwxy(a);
        should_be_equal_simd4f(x, simd4f_create(3,4,1,2), epsilon );
    }

    it("should have simd4f_shuffle_yzwx") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f x = simd4f_shuffle_yzwx(a);
        should_be_equal_simd4f(x, simd4f_create(2,3,4,1), epsilon );
    }

    it("should have simd4f_merge_high") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(5,6,7,8);
        simd4f x = simd4f_merge_high(a,b);
        should_be_equal_simd4f(x, simd4f_create(3,4,7,8), epsilon );
    }
    
}

describe(simd4f, "signs") {

    it("should have simd4f_flip_sign_0101 for flipping even elements sign") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f x = simd4f_flip_sign_0101(a);
        should_be_equal_simd4f(x, simd4f_create(1,-2,3,-4), epsilon );
    }

    it("should have simd4f_flip_sign_1010 for flipping even elements sign") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f x = simd4f_flip_sign_1010(a);
        should_be_equal_simd4f(x, simd4f_create(-1,2,-3,4), epsilon );
    }

}

describe(simd4f, "min-max") {
    
    it("should have simd4f_min for choosing minimum elements") {
        simd4f a = simd4f_create(1.0f,  2.0f, -300000000.0f, -0.000002f);
        simd4f b = simd4f_create(2.0f, -2.0f,  300000000.0f,  0.000001f);

        simd4f x = simd4f_min(a,b);
        should_be_equal_simd4f(x, simd4f_create(1.0f, -2.0f, -300000000.0f, -0.000002f), epsilon);
        
    }

    it("should have simd4f_max for choosing maximum elements") {
        simd4f a = simd4f_create(1.0f,  2.0f, -300000000.0f, -0.000002f);
        simd4f b = simd4f_create(2.0f, -2.0f,  300000000.0f,  0.000001f);

        simd4f x = simd4f_max(a,b);
        should_be_equal_simd4f(x, simd4f_create(2.0f, 2.0f, 300000000.0f, 0.000001f), epsilon);
        
    }
    
    
    
}


describe(simd4f, "zeroing")
{

    it("should have simd4f_zero_w that zeros the last element")
    {
        simd4f a = simd4f_create(1.0f, 2.0f, 3.0f, 4.0f);
        simd4f b = simd4f_create(1.0f, 2.0f, 3.0f, NAN);
        simd4f x = simd4f_zero_w(a);
        should_be_equal_simd4f(x, simd4f_create(1.0f, 2.0f, 3.0f, 0.0f), epsilon);
        x = simd4f_zero_w(b);
        should_be_equal_simd4f(x, simd4f_create(1.0f, 2.0f, 3.0f, 0.0f), epsilon);
    }

    it("should have simd4f_zero_zw that zeros the last element")
    {
        simd4f a = simd4f_create(1.0f, 2.0f, 3.0f, 4.0f);
        simd4f b = simd4f_create(1.0f, 2.0f, NAN, NAN);
        simd4f x = simd4f_zero_zw(a);
        should_be_equal_simd4f(x, simd4f_create(1.0f, 2.0f, 0.0f, 0.0f), epsilon);
        x = simd4f_zero_zw(b);
        should_be_equal_simd4f(x, simd4f_create(1.0f, 2.0f, 0.0f, 0.0f), epsilon);
    }

}





