#ifndef VECTORIAL_SPEC_HELPER_H
#define VECTORIAL_SPEC_HELPER_H

#define VECTORIAL_OSTREAM

#include "spec.h"
#include "vectorial/simd4f.h"
//#include <cmath>

//#include <iostream>
/*#include "vectorial/vec4f.h"
#include "vectorial/vec3f.h"
#include "vectorial/vec2f.h"

#include "vectorial/simd4x4f.h"
*/
#define should_be_close_to(a,b,tolerance) should_be_close_to_(this, a,b,tolerance,__FILE__,__LINE__)
#define should_be_equal_simd4f( a, b, tolerance) should_be_equal_simd4f_(this, a,b,tolerance,__FILE__,__LINE__)
#define should_be_equal_vec4f( a, b, tolerance) should_be_equal_vec4f_(this, a,b,tolerance,__FILE__,__LINE__)
#define should_be_equal_vec3f( a, b, tolerance) should_be_equal_vec3f_(this, a,b,tolerance,__FILE__,__LINE__)
#define should_be_equal_vec2f( a, b, tolerance) should_be_equal_vec2f_(this, a,b,tolerance,__FILE__,__LINE__)

#define should_be_equal_simd4x4f( a, b, tolerance) should_be_equal_simd4x4f_(this, a,b,tolerance,__FILE__,__LINE__)





static void should_be_close_to_(specific::SpecBase *spec, float a, float b, float tolerance, const char *file, int line) {
    
    bool equal=true;
    if( fabs(b-a) > tolerance ) equal = false;

    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}


static void should_be_equal_simd4f_(specific::SpecBase *spec, const simd4f& a, const simd4f& b, float tolerance, const char *file, int line) {
    
    bool equal=true;
    if( fabs( simd4f_get_x(b) - simd4f_get_x(a)) > tolerance ) equal = false;
    if( fabs( simd4f_get_y(b) - simd4f_get_y(a)) > tolerance ) equal = false;
    if( fabs( simd4f_get_z(b) - simd4f_get_z(a)) > tolerance ) equal = false;
//    if(A::elements > 3 && B::elements > 3)
        if( fabs( simd4f_get_w(b) - simd4f_get_w(a)) > tolerance ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}

static void should_be_equal_vec4f_(specific::SpecBase *spec, const vectorial::vec4f& a, const vectorial::vec4f& b, float tolerance, const char *file, int line) {
    
    bool equal=true;
    if( fabs( b.x() - a.x()) > tolerance ) equal = false;
    if( fabs( b.y() - a.y()) > tolerance ) equal = false;
    if( fabs( b.z() - a.z()) > tolerance ) equal = false;
//    if(A::elements > 3 && B::elements > 3)
        if( fabs( b.w() - a.w()) > tolerance ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}

static void should_be_equal_vec3f_(specific::SpecBase *spec, const vectorial::vec3f& a, const vectorial::vec3f& b, float tolerance, const char *file, int line) {
    
    bool equal=true;
    if( fabs( b.x() - a.x()) > tolerance ) equal = false;
    if( fabs( b.y() - a.y()) > tolerance ) equal = false;
    if( fabs( b.z() - a.z()) > tolerance ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}

static void should_be_equal_vec2f_(specific::SpecBase *spec, const vectorial::vec2f& a, const vectorial::vec2f& b, float tolerance, const char *file, int line) {
    
    bool equal=true;
    if( fabs( b.x() - a.x()) > tolerance ) equal = false;
    if( fabs( b.y() - a.y()) > tolerance ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}



static void should_be_equal_simd4x4f_(specific::SpecBase *spec, const simd4x4f& a, const simd4x4f& b, float tolerance, const char *file, int line) {
    
    bool equal=true;
    if( fabs( simd4f_get_x(b.x) - simd4f_get_x(a.x)) > tolerance ) equal = false;
    if( fabs( simd4f_get_y(b.x) - simd4f_get_y(a.x)) > tolerance ) equal = false;
    if( fabs( simd4f_get_z(b.x) - simd4f_get_z(a.x)) > tolerance ) equal = false;
    if( fabs( simd4f_get_w(b.x) - simd4f_get_w(a.x)) > tolerance ) equal = false;

    if( fabs( simd4f_get_x(b.y) - simd4f_get_x(a.y)) > tolerance ) equal = false;
    if( fabs( simd4f_get_y(b.y) - simd4f_get_y(a.y)) > tolerance ) equal = false;
    if( fabs( simd4f_get_z(b.y) - simd4f_get_z(a.y)) > tolerance ) equal = false;
    if( fabs( simd4f_get_w(b.y) - simd4f_get_w(a.y)) > tolerance ) equal = false;

    if( fabs( simd4f_get_x(b.z) - simd4f_get_x(a.z)) > tolerance ) equal = false;
    if( fabs( simd4f_get_y(b.z) - simd4f_get_y(a.z)) > tolerance ) equal = false;
    if( fabs( simd4f_get_z(b.z) - simd4f_get_z(a.z)) > tolerance ) equal = false;
    if( fabs( simd4f_get_w(b.z) - simd4f_get_w(a.z)) > tolerance ) equal = false;

    if( fabs( simd4f_get_x(b.w) - simd4f_get_x(a.w)) > tolerance ) equal = false;
    if( fabs( simd4f_get_y(b.w) - simd4f_get_y(a.w)) > tolerance ) equal = false;
    if( fabs( simd4f_get_z(b.w) - simd4f_get_z(a.w)) > tolerance ) equal = false;
    if( fabs( simd4f_get_w(b.w) - simd4f_get_w(a.w)) > tolerance ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}



#endif
