#ifndef VECTORIAL_SIMD_H
#define VECTORIAL_SIMD_H


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
  #include "simd_scalar.h"
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

    
    


}


#endif
#endif
