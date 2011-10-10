#include "spec_helper.h"
#include <iostream>
using vectorial::vec4f;
using vectorial::mat4f;

const int epsilon = 1;

describe(mat4f, "constructing") {
    it("should have default constructor that does nothing..") {
        mat4f x;
    }

    it("should have constructor that constructs from four vec4") {
        mat4f x( vec4f(1,2,3,4), vec4f(5,6,7,8), vec4f(9,10,11,12), vec4f(13,14,15,16) );

        // octave mat4f: [1,5,9,13 ; 2,6,10,14 ; 3,7,11,15 ; 4,8,12,16 ]
        should_be_equal_mat4f(x, simd4x4f_create(simd4f_create(1.000000000000000f, 2.000000000000000f, 3.000000000000000f, 4.000000000000000f), simd4f_create(5.000000000000000f, 6.000000000000000f, 7.000000000000000f, 8.000000000000000f), simd4f_create(9.000000000000000f, 10.000000000000000f, 11.000000000000000f, 12.000000000000000f), simd4f_create(13.000000000000000f, 14.000000000000000f, 15.000000000000000f, 16.000000000000000f)), epsilon );
    }
    
    it("should have static function to create identity matrix") {
        
        mat4f x = mat4f::identity();
        
        // octave mat4f: [1,0,0,0;0,1,0,0;0,0,1,0;0,0,0,1]
        should_be_equal_mat4f(x, simd4x4f_create(simd4f_create(1.000000000000000f, 0.000000000000000f, 0.000000000000000f, 0.000000000000000f), simd4f_create(0.000000000000000f, 1.000000000000000f, 0.000000000000000f, 0.000000000000000f), simd4f_create(0.000000000000000f, 0.000000000000000f, 1.000000000000000f, 0.000000000000000f), simd4f_create(0.000000000000000f, 0.000000000000000f, 0.000000000000000f, 1.000000000000000f)), epsilon );
    }
    
}

