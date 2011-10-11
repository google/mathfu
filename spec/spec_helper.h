#ifndef VECTORIAL_SPEC_HELPER_H
#define VECTORIAL_SPEC_HELPER_H

#define VECTORIAL_OSTREAM

#include "spec.h"

#include "vectorial/vectorial.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

#define should_be_close_to(a,b,tolerance) should_be_close_to_(this, a,b,tolerance,__FILE__,__LINE__)
#define should_be_equal_simd4f( a, b, tolerance) should_be_equal_simd4f_(this, a,b,tolerance,__FILE__,__LINE__)
#define should_be_equal_vec4f( a, b, tolerance) should_be_equal_vec4f_(this, a,b,tolerance,__FILE__,__LINE__)
#define should_be_equal_vec3f( a, b, tolerance) should_be_equal_vec3f_(this, a,b,tolerance,__FILE__,__LINE__)
#define should_be_equal_vec2f( a, b, tolerance) should_be_equal_vec2f_(this, a,b,tolerance,__FILE__,__LINE__)

#define should_be_equal_simd4x4f( a, b, tolerance) should_be_equal_simd4x4f_(this, a,b,tolerance,__FILE__,__LINE__)

#define should_be_equal_mat4f( a, b, tolerance) should_be_equal_mat4f_(this, a,b,tolerance,__FILE__,__LINE__)

// Based on:
// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
// 
static inline bool compare_floats(float A, float B, int maxUlps)
{
    // Make sure maxUlps is non-negative and small enough that the
    // default NAN won't compare as equal to anything.
    // assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
    union {
        float f;
        int i;
    } f2iA, f2iB;
    f2iA.f = A;
    f2iB.f = B;
    
    int aInt = f2iA.i;
//    int aInt = *(int*)&A;
    // Make aInt lexicographically ordered as a twos-complement int
    if (aInt < 0)
        aInt = 0x80000000 - aInt;
    // Make bInt lexicographically ordered as a twos-complement int
    int bInt = f2iB.i;
//    int bInt = *(int*)&B;
    if (bInt < 0)
        bInt = 0x80000000 - bInt;
    int intDiff = abs(aInt - bInt);
    if (intDiff <= maxUlps)
        return true;
    return false;
}









static void should_be_close_to_(specific::SpecBase *spec, float a, float b, int tolerance, const char *file, int line) {
    
    bool equal=true;
    if( !compare_floats(a,b,tolerance) ) equal = false;

    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}


static void should_be_equal_simd4f_(specific::SpecBase *spec, const simd4f& a, const simd4f& b, int tolerance, const char *file, int line) {
    
    bool equal=true;
    if( !compare_floats( simd4f_get_x(a), simd4f_get_x(b), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_y(a), simd4f_get_y(b), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_z(a), simd4f_get_z(b), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_w(a), simd4f_get_w(b), tolerance) ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}

static void should_be_equal_vec4f_(specific::SpecBase *spec, const vectorial::vec4f& a, const vectorial::vec4f& b, int tolerance, const char *file, int line) {
    
    bool equal=true;
    if( !compare_floats( a.x(), b.x(), tolerance) ) equal = false;
    if( !compare_floats( a.y(), b.y(), tolerance) ) equal = false;
    if( !compare_floats( a.z(), b.z(), tolerance) ) equal = false;
    if( !compare_floats( a.w(), b.w(), tolerance) ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}

static void should_be_equal_vec3f_(specific::SpecBase *spec, const vectorial::vec3f& a, const vectorial::vec3f& b, int tolerance, const char *file, int line) {
    
    bool equal=true;
    if( !compare_floats( a.x(), b.x(), tolerance) ) equal = false;
    if( !compare_floats( a.y(), b.y(), tolerance) ) equal = false;
    if( !compare_floats( a.z(), b.z(), tolerance) ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}

static void should_be_equal_vec2f_(specific::SpecBase *spec, const vectorial::vec2f& a, const vectorial::vec2f& b, int tolerance, const char *file, int line) {
    
    bool equal=true;
    if( !compare_floats( a.x(), b.x(), tolerance) ) equal = false;
    if( !compare_floats( a.y(), b.y(), tolerance) ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}



static void should_be_equal_simd4x4f_(specific::SpecBase *spec, const simd4x4f& a, const simd4x4f& b, int tolerance, const char *file, int line) {
    
    bool equal=true;
    if( !compare_floats( simd4f_get_x(a.x), simd4f_get_x(b.x), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_y(a.x), simd4f_get_y(b.x), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_z(a.x), simd4f_get_z(b.x), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_w(a.x), simd4f_get_w(b.x), tolerance) ) equal = false;

    if( !compare_floats( simd4f_get_x(a.y), simd4f_get_x(b.y), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_y(a.y), simd4f_get_y(b.y), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_z(a.y), simd4f_get_z(b.y), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_w(a.y), simd4f_get_w(b.y), tolerance) ) equal = false;

    if( !compare_floats( simd4f_get_x(a.z), simd4f_get_x(b.z), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_y(a.z), simd4f_get_y(b.z), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_z(a.z), simd4f_get_z(b.z), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_w(a.z), simd4f_get_w(b.z), tolerance) ) equal = false;

    if( !compare_floats( simd4f_get_x(a.w), simd4f_get_x(b.w), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_y(a.w), simd4f_get_y(b.w), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_z(a.w), simd4f_get_z(b.w), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_w(a.w), simd4f_get_w(b.w), tolerance) ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << ")";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}

static void should_be_equal_mat4f_(specific::SpecBase *spec, const vectorial::mat4f& a, const vectorial::mat4f& b, int tolerance, const char *file, int line) {
                                                                        
    bool equal=true;                                                    
    if( !compare_floats( simd4f_get_x(a.value.x), simd4f_get_x(b.value.x), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_y(a.value.x), simd4f_get_y(b.value.x), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_z(a.value.x), simd4f_get_z(b.value.x), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_w(a.value.x), simd4f_get_w(b.value.x), tolerance) ) equal = false;

    if( !compare_floats( simd4f_get_x(a.value.y), simd4f_get_x(b.value.y), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_y(a.value.y), simd4f_get_y(b.value.y), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_z(a.value.y), simd4f_get_z(b.value.y), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_w(a.value.y), simd4f_get_w(b.value.y), tolerance) ) equal = false;

    if( !compare_floats( simd4f_get_x(a.value.z), simd4f_get_x(b.value.z), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_y(a.value.z), simd4f_get_y(b.value.z), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_z(a.value.z), simd4f_get_z(b.value.z), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_w(a.value.z), simd4f_get_w(b.value.z), tolerance) ) equal = false;

    if( !compare_floats( simd4f_get_x(a.value.w), simd4f_get_x(b.value.w), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_y(a.value.w), simd4f_get_y(b.value.w), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_z(a.value.w), simd4f_get_z(b.value.w), tolerance) ) equal = false;
    if( !compare_floats( simd4f_get_w(a.value.w), simd4f_get_w(b.value.w), tolerance) ) equal = false;
    
    std::stringstream ss;
    ss << a << " == " << b << " (with tolerance of " << tolerance << " ulps)";
    spec->should_test(equal, ss.str().c_str(), file, line);
    
    
}



#endif
