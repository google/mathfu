
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
    
    
}

describe(simd4f, "utilities") {

    it("should have simd4f_splat that expands a single scalar to all elements") {
        simd4f x = simd4f_splat(42);
        // octave simd4f: [42,42,42,42]
        should_be_equal_simd4f(x, simd4f_create(42.000000, 42.000000, 42.000000, 42.000000), epsilon );
    }

    it("should have simd4f_splat_x,y,z,w splatting of an element") {
        simd4f a = simd4f_create(1,2,3,4);

        simd4f x;
        
        x = simd4f_splat_x(a);
        // octave simd4f: [1,1,1,1]
        should_be_equal_simd4f(x, simd4f_create(1.000000, 1.000000, 1.000000, 1.000000), epsilon );

        x = simd4f_splat_y(a);
        // octave simd4f: [2,2,2,2]
        should_be_equal_simd4f(x, simd4f_create(2.000000, 2.000000, 2.000000, 2.000000), epsilon );

        x = simd4f_splat_z(a);
        // octave simd4f: [3,3,3,3]
        should_be_equal_simd4f(x, simd4f_create(3.000000, 3.000000, 3.000000, 3.000000), epsilon );

        x = simd4f_splat_w(a);
        // octave simd4f: [4,4,4,4]
        should_be_equal_simd4f(x, simd4f_create(4.000000, 4.000000, 4.000000, 4.000000), epsilon );
    }
    
    it("should have simd4f_sum that adds elements") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f x = simd4f_sum(a);
        // octave simd4f: [sum([1,2,3,4]), sum([1,2,3,4]), sum([1,2,3,4]), sum([1,2,3,4])]
        should_be_equal_simd4f(x, simd4f_create(10.000000, 10.000000, 10.000000, 10.000000), epsilon );
        
    }

}

describe(simd4f, "arithmetic with another simd4f") {

    it("should have simd4f_add for component-wise addition") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(10,20,30,40);
        
        simd4f x = simd4f_add(a,b);
        // octave simd4f: [1,2,3,4] + [10,20,30,40]
        should_be_equal_simd4f(x, simd4f_create(11.000000, 22.000000, 33.000000, 44.000000), epsilon );
    }

    it("should have simd4f_sub for component-wise subtraction") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(10,20,30,40);
        
        simd4f x = simd4f_sub(b,a);
        // octave simd4f: [10,20,30,40] - [1,2,3,4] 
        should_be_equal_simd4f(x, simd4f_create(9.000000, 18.000000, 27.000000, 36.000000), epsilon );
    }

    it("should have simd4f_mul for component-wise multiply") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(10,20,30,40);
        
        simd4f x = simd4f_mul(a,b);
        // octave simd4f: [1,2,3,4] .* [10,20,30,40]
        should_be_equal_simd4f(x, simd4f_create(10.000000, 40.000000, 90.000000, 160.000000), epsilon );
    }

    it("should have simd4f_div for component-wise division") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(10,20,30,40);
        
        simd4f x = simd4f_div(b,a);
        // octave simd4f: [10,20,30,40] ./ [1,2,3,4] 
        should_be_equal_simd4f(x, simd4f_create(10.000000, 10.000000, 10.000000, 10.000000), epsilon );
    }

}


describe(simd4f, "vector math") {
    
    it("should have simd4f_dot4 for four component dot product") {
        simd4f a = simd4f_create(1,2,3,4);
        simd4f b = simd4f_create(10,20,30,40);
        
        simd4f x = simd4f_dot4(a,b);
        // octave simd4f: [dot([1, 2, 3, 4], [10, 20, 30, 40]),dot([1, 2, 3, 4], [10, 20, 30, 40]),dot([1, 2, 3, 4], [10, 20, 30, 40]),dot([1, 2, 3, 4], [10, 20, 30, 40])]
        should_be_equal_simd4f(x, simd4f_create(300.000000, 300.000000, 300.000000, 300.000000), epsilon );
    }

    it("should have simd4f_dot3 for three component dot product") {
        simd4f a = simd4f_create(1,2,3,9999);
        simd4f b = simd4f_create(10,20,30,-9990);
        
        simd4f x = simd4f_dot3(a,b);
        // octave simd4f: [dot([1, 2, 3, 0], [10, 20, 30, 0]),dot([1, 2, 3, 0], [10, 20, 30, 0]),dot([1, 2, 3, 0], [10, 20, 30, 0]),dot([1, 2, 3, 0], [10, 20, 30, 0])]
        should_be_equal_simd4f(x, simd4f_create(140.000000, 140.000000, 140.000000, 140.000000), epsilon );
    }
    
}


