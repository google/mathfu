#ifndef VECTORIAL_SPEC_HELPER_H
#define VECTORIAL_SPEC_HELPER_H

#define VECTORIAL_OSTREAM
#include "spec.h"
#include "vectorial/vec4f.h"
#include <cmath>


#define should_be_close_to(a,b,tolerance) should_be_close_to_(this, a,b,tolerance,__FILE__,__LINE__)
#define should_be_equal_simd4f( a, b, tolerance) should_be_equal_simd4f_(this, a,b,tolerance,__FILE__,__LINE__)

static void should_be_close_to_(specific::SpecBase *spec, float a, float b, float tolerance, const char *file, int line) {
    
    bool equal=true;
    if( fabs(b-a) > tolerance ) equal = false;

    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}


static void should_be_equal_simd4f_(specific::SpecBase *spec, const vectorial::simd4f& a, const vectorial::simd4f& b, float tolerance, const char *file, int line) {
    
    bool equal=true;
    if( fabs(b.x()-a.x()) > tolerance ) equal = false;
    if( fabs(b.y()-a.y()) > tolerance ) equal = false;
    if( fabs(b.z()-a.z()) > tolerance ) equal = false;
    if( fabs(b.w()-a.w()) > tolerance ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}





#endif
