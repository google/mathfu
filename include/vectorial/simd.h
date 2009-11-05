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


namespace vectorial {
    
    class simd4f {
    public:

        #ifdef VECTORIAL_GNU
            typedef float v4f __attribute__ ((vector_size (16)));
        #endif
        
        #ifdef VECTORIAL_SSE
            typedef __m128 v4f;
        #endif

        #ifdef VECTORIAL_NEON
            typedef float32x4_t v4f;
        #endif
        
    
        union simd_value {
            v4f v;
            float f[4];
        };
    
        simd_value value;
    
        vectorial_inline simd4f() {}
        vectorial_inline explicit simd4f(const v4f& v) { value.v=v; }
        vectorial_inline simd4f(const simd4f& v) { value.v=v.value.v; }
    
        vectorial_inline simd4f(float x, float y, float z, float w) { v4f v = {x,y,z,w}; value.v=v; }

        vectorial_inline simd4f operator+(const simd4f& other) const { 
            return simd4f(value.v + other.value.v); 
        }
        
        
        vectorial_inline const float& operator[](size_t i) const { return value.f[i]; }
        vectorial_inline float& operator[](size_t i) { return value.f[i]; }
    
    };

}


#endif
#endif
