#ifndef VECTORIAL_SIMD4F_H
#define VECTORIAL_SIMD4F_H


#ifndef VECTORIAL_CONFIG_H
  #include "vectorial/config.h"
#endif




#ifdef VECTORIAL_SSE
  #include <xmmintrin.h>
#endif

#ifdef VECTORIAL_NEON
  #include <arm_neon.h>
#endif


#ifdef VECTORIAL_SCALAR
  #include "simd4f_scalar.h"
#else

#include <cstdlib> // size_t .. 
#include <cmath>

namespace vectorial {

    #ifdef VECTORIAL_GNU
        typedef float v4f __attribute__ ((vector_size (16)));
    #endif
    
    #ifdef VECTORIAL_SSE
        typedef __m128 v4f;
    #endif

    #ifdef VECTORIAL_NEON
        typedef float32x4_t v4f;
    #endif
    
    class simd4f {
    public:

        
    
        union simd_value {
            v4f v;
            float f[4];
        };
    
        simd_value value;
    
        vectorial_inline simd4f() {}
        /* vectorial_inline explicit simd4f(const v4f& v) { value.v=v; } Crashes on iPhone gcc (can't handle name mangling?) */ 
        vectorial_inline simd4f(const simd4f& v) { value.v=v.value.v; }
    
        vectorial_inline simd4f(float x, float y, float z, float w) { v4f v = {x,y,z,w}; value.v=v; }
        vectorial_inline simd4f(float fv[4]) { v4f v = {fv[0],fv[1],fv[2],fv[3]}; value.v=v; }

        
//        vectorial_inline const float& operator[](size_t i) const { return value.f[i]; }
//        vectorial_inline float& operator[](size_t i) { return value.f[i]; }


        vectorial_inline const float& x() const { return value.f[0]; }
        vectorial_inline const float& y() const { return value.f[1]; }
        vectorial_inline const float& z() const { return value.f[2]; }
        vectorial_inline const float& w() const { return value.f[3]; }

    
    };


    static simd4f operator+(const simd4f& lhs, const simd4f& rhs) { 
        simd4f ret; ret.value.v = lhs.value.v + rhs.value.v;
        return ret; 
    }

    static simd4f operator*(const simd4f& lhs, const simd4f& rhs) { 
        simd4f ret; ret.value.v=(lhs.value.v * rhs.value.v); 
        return ret;
    }

    static simd4f operator/(const simd4f& lhs, const simd4f& rhs) { 
        simd4f ret; ret.value.v=(lhs.value.v / rhs.value.v); 
        return ret;
    }


    static simd4f operator+(const simd4f& lhs, float rhs) { 
        v4f o = {rhs, rhs, rhs, rhs};
        simd4f ret; ret.value.v = (lhs.value.v + o);
        return ret;
    }

    static simd4f operator*(const simd4f& lhs, float rhs) { 
        v4f o = {rhs, rhs, rhs, rhs};
        simd4f ret; ret.value.v = (lhs.value.v * o); 
        return ret;
    }

    static simd4f operator/(const simd4f& lhs, float rhs) { 
        v4f o = {rhs, rhs, rhs, rhs};
        simd4f ret; ret.value.v = (lhs.value.v / o); 
        return ret;
    }

    static simd4f operator+(float lhs, const simd4f& rhs) { 
        v4f o = {lhs, lhs, lhs, lhs};
        simd4f ret; ret.value.v = (o + rhs.value.v);
        return ret;
    }

    static simd4f operator*(float lhs, const simd4f& rhs) { 
        v4f o = {lhs, lhs, lhs, lhs};
        simd4f ret; ret.value.v = (o * rhs.value.v); 
        return ret;
    }

    static simd4f operator/(float lhs, const simd4f& rhs) { 
        v4f o = {lhs, lhs, lhs, lhs};
        simd4f ret; ret.value.v = (o / rhs.value.v); 
        return ret;
    }


    static simd4f sin(const simd4f& v) {
        // TODO:
        return simd4f(sinf(v.x()), sinf(v.y()), sinf(v.z()), sinf(v.w()));
    }

    static simd4f cos(const simd4f& v) {
        // TODO:
        return simd4f(cosf(v.x()), cosf(v.y()), cosf(v.z()), cosf(v.w()));
    }
    
    static simd4f tan(const simd4f& v) {
        // TODO:
        return simd4f(tanf(v.x()), tanf(v.y()), tanf(v.z()), tanf(v.w()));
    }


    static simd4f asin(const simd4f& v) {
        // TODO:
        return simd4f(asinf(v.x()), asinf(v.y()), asinf(v.z()), asinf(v.w()));
    }

    static simd4f acos(const simd4f& v) {
        // TODO:
        return simd4f(acosf(v.x()), acosf(v.y()), acosf(v.z()), acosf(v.w()));
    }
    
    static simd4f atan(const simd4f& v) {
        // TODO:
        return simd4f(atanf(v.x()), atanf(v.y()), atanf(v.z()), atanf(v.w()));
    }

    static simd4f atan2(const simd4f& v, const simd4f& u) {
        // TODO:
        return simd4f(atan2f(v.x(), u.x()), atan2f(v.y(), u.y()), atan2f(v.z(), u.z()), atan2f(v.w(), u.w()));
    }
    
    
    static simd4f sqrt(const simd4f& v) {
        // TODO:
        return simd4f(sqrtf(v.x()), sqrtf(v.y()), sqrtf(v.z()), sqrtf(v.w()));
    }
    
    static simd4f inversesqrt(const simd4f& v) {
        // TODO:
        return 1.0f/sqrt(v);
    }
    
    static simd4f abs(const simd4f& v) {
        // TODO:
        return simd4f(fabsf(v.x()), fabsf(v.y()), fabsf(v.z()), fabsf(v.w()));        
    }

    namespace impl {
        static float sign(float f) {
            if(f < 0.0f) return -1.0f;
            if(f > 0.0f) return 1.0f;
            return 0.0f;
        }
    }

    static simd4f sign(const simd4f& v) {
        // TODO:
        return simd4f(impl::sign(v.x()),impl::sign(v.y()),impl::sign(v.z()),impl::sign(v.w()) );
    }

}


#endif
#endif
