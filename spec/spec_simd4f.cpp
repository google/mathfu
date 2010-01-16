#include "spec_helper.h"
#include <iostream>
using vectorial::simd4f;

const float epsilon = 0.00001f;

describe(simd4f, "sanity") {
    it("has simd-type") {
        std::cout << "Using simd: " << VECTORIAL_SIMD_TYPE << std::endl;
    }
}

describe(simd4f, "arithmetics with other simd4f") {
    
    
    it("should component-wise add") {
        simd4f a(1,2,3,4);
        simd4f b(2,3,4,5);
        
        simd4f x = a + b;
        
        // octave: [1,2,3,4] + [2,3,4,5]
        should_be_equal_simd4f(x, simd4f(3.000000, 5.000000, 7.000000, 9.000000), epsilon );
        
    }
    
    it("should component-wise multiply") {
        simd4f a(1,2,3,4);
        simd4f b(2,3,4,5);
        
        simd4f x = a * b;
        
        // octave: [1,2,3,4] .* [2,3,4,5]
        should_be_equal_simd4f(x, simd4f(2.000000, 6.000000, 12.000000, 20.000000), epsilon );
    }

    it("should component-wise division") {
        simd4f a(1,2,3,4);
        simd4f b(2,3,4,5);
        
        simd4f x = a / b;
        
        // octave: [1,2,3,4] ./ [2,3,4,5]
        should_be_equal_simd4f(x, simd4f(0.500000, 0.666667, 0.750000, 0.800000), epsilon );
        
        x = b / a;
        // octave: [2,3,4,5] ./ [1,2,3,4] 
        should_be_equal_simd4f(x, simd4f(2.000000, 1.500000, 1.333333, 1.250000), epsilon );

    }

     
}

describe(simd4f, "arithmetics with other scalar") {

    it("should add") {
        simd4f a(1,2,3,4);
        float b = 2.2f;
        
        simd4f x = a + b;
        
        // octave: [1,2,3,4] + 2.2
        should_be_equal_simd4f(x, simd4f(3.200000, 4.200000, 5.200000, 6.200000), epsilon );
        x = b + a;
        // octave: 2.2 + [1,2,3,4]
        should_be_equal_simd4f(x, simd4f(3.200000, 4.200000, 5.200000, 6.200000), epsilon );
    }
    
    it("should multiply") {
        simd4f a(1,2,3,4);
        float b = 2.2f;
        
        simd4f x = a * b;
        
        // octave: [1,2,3,4] .* 2.2
        should_be_equal_simd4f(x, simd4f(2.200000, 4.400000, 6.600000, 8.800000), epsilon );
        x = b * a;
        // octave: 2.2 .* [1,2,3,4]
        should_be_equal_simd4f(x, simd4f(2.200000, 4.400000, 6.600000, 8.800000), epsilon );

    }

    it("should divide") {
        simd4f a(1,2,3,4);
        float b = 2.2f;
        
        simd4f x = a / b;
        
        // octave: [1,2,3,4] ./ 2.2
        should_be_equal_simd4f(x, simd4f(0.454545, 0.909091, 1.363636, 1.818182), epsilon );

        x = b / a;
        // octave:  2.2 ./ [1,2,3,4] 
        should_be_equal_simd4f(x, simd4f(2.200000, 1.100000, 0.733333, 0.550000), epsilon );

    }

}

describe(simd4f, "trig functions") {
    it("should have sin function") {
        simd4f a(1,2,3,4);
        simd4f x = vectorial::sin(a);
        // octave: sin([1,2,3,4])
        should_be_equal_simd4f(x, simd4f(0.841471, 0.909297, 0.141120, -0.756802), epsilon );
    }

    it("should have cos function") {
        simd4f a(1,2,3,4);
        simd4f x = vectorial::cos(a);
        // octave: cos([1,2,3,4])
        should_be_equal_simd4f(x, simd4f(0.540302, -0.416147, -0.989992, -0.653644), epsilon );
    }

    it("should have tan function") {
        simd4f a(1,2,3,4);
        simd4f x = vectorial::tan(a);
        // octave: tan([1,2,3,4])
        should_be_equal_simd4f(x, simd4f(1.557408, -2.185040, -0.142547, 1.157821), epsilon );
    }

    it("should have asin function") {
        simd4f a(0.1,0.2,0.3,0.4);
        simd4f x = vectorial::asin(a);
        // octave: asin([0.1,0.2,0.3,0.4])
        should_be_equal_simd4f(x, simd4f(0.100167, 0.201358, 0.304693, 0.411517), epsilon );
    }

    it("should have acos function") {
        simd4f a(0.1,0.2,0.3,0.4);
        simd4f x = vectorial::acos(a);
        // octave: acos([0.1,0.2,0.3,0.4])
        should_be_equal_simd4f(x, simd4f(1.470629, 1.369438, 1.266104, 1.159279), epsilon );
    }

    it("should have atan function") {
        simd4f a(0.1,0.2,0.3,0.4);
        simd4f x = vectorial::atan(a);
        // octave: atan([0.1,0.2,0.3,0.4])
        should_be_equal_simd4f(x, simd4f(0.099669, 0.197396, 0.291457, 0.380506), epsilon );
    }

    it("should have atan2 function") {
        simd4f a(0.1,0.2,0.3,0.4);
        simd4f b(0.6,0.7,0.8,0.9);
        simd4f x = vectorial::atan2(a,b);
        // octave: atan2([0.1,0.2,0.3,0.4], [0.6,0.7,0.8,0.9])
        should_be_equal_simd4f(x, simd4f(0.165149, 0.278300, 0.358771, 0.418224), epsilon );
    }


}

describe(simd4f, "common functions") {
    it("should have sqrt function") {
        simd4f a(1,2,3,4);
        simd4f x = vectorial::sqrt(a);
        // octave: sqrt([1,2,3,4])
        should_be_equal_simd4f(x, simd4f(1.000000, 1.414214, 1.732051, 2.000000), epsilon );
    }

    it("should have inversesqrt function") {
        simd4f a(1,2,3,4);
        simd4f x = 1.0f/vectorial::sqrt(a);
        // octave: 1.0./sqrt([1,2,3,4])
        should_be_equal_simd4f(x, simd4f(1.000000, 0.707107, 0.577350, 0.500000), epsilon );
    }
    
    it("should have abs function") {
        simd4f a(1,-2,3,-4);
        simd4f x = vectorial::abs(a);
        // octave: abs([1,-2,3,-4])
        should_be_equal_simd4f(x, simd4f(1.000000, 2.000000, 3.000000, 4.000000), epsilon );
    }

    it("should have sign function") {
        simd4f a(1,-2,3,-4);
        simd4f x = vectorial::sign(a);
        // octave: sign([1,-2,3,-4])
        should_be_equal_simd4f(x, simd4f(1.000000, -1.000000, 1.000000, -1.000000), epsilon );
    }

}

