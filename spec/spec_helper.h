#ifndef VECTORIAL_SPEC_HELPER_H
#define VECTORIAL_SPEC_HELPER_H

#define VECTORIAL_OSTREAM

#include "spec.h"
#include "vectorial/simd4f.h"
#include <cmath>

#include <iostream>
/*
#include "vectorial/vec4f.h"
#include "vectorial/vec3f.h"
*/

#define should_be_close_to(a,b,tolerance) should_be_close_to_(this, a,b,tolerance,__FILE__,__LINE__)
#define should_be_equal_simd4f( a, b, tolerance) should_be_equal_simd4f_(this, a,b,tolerance,__FILE__,__LINE__)





static void should_be_close_to_(specific::SpecBase *spec, float a, float b, float tolerance, const char *file, int line) {
    
    bool equal=true;
    if( fabs(b-a) > tolerance ) equal = false;

    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}


template<class A, class B> static  void should_be_equal_simd4f_(specific::SpecBase *spec, const A& a, const B& b, float tolerance, const char *file, int line) {
    
    bool equal=true;
    if( fabs( simd4f_getX(b) - simd4f_getX(a)) > tolerance ) equal = false;
    if( fabs( simd4f_getY(b) - simd4f_getY(a)) > tolerance ) equal = false;
    if( fabs( simd4f_getZ(b) - simd4f_getZ(a)) > tolerance ) equal = false;
//    if(A::elements > 3 && B::elements > 3)
        if( fabs( simd4f_getW(b) - simd4f_getW(a)) > tolerance ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}




#endif
