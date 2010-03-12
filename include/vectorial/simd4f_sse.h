#ifndef VECTORIAL_SIMD4F_SSE_H
#define VECTORIAL_SIMD4F_SSE_H

#include <xmmintrin.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef __m128 simd4f; 

union _simd4f_union {
    simd4f s ;
    float f[4];
};



vectorial_inline simd4f simd4f_create(float x, float y, float z, float w) {
    simd4f s = { x, y, z, w };
    return s;
}

vectorial_inline simd4f simd4f_splat(float v) { 
    simd4f s = { v, v, v, v }; 
    return s;
}

vectorial_inline simd4f simd4f_add(simd4f lhs, simd4f rhs) {
    simd4f ret = _mm_add_ps(lhs, rhs);
    return ret;
}


vectorial_inline float simd4f_getX(simd4f s) { _simd4f_union u={s}; return u.f[0]; }
vectorial_inline float simd4f_getY(simd4f s) { _simd4f_union u={s}; return u.f[1]; }
vectorial_inline float simd4f_getZ(simd4f s) { _simd4f_union u={s}; return u.f[2]; }
vectorial_inline float simd4f_getW(simd4f s) { _simd4f_union u={s}; return u.f[3]; }


#ifdef __cplusplus
}
#endif


#endif

