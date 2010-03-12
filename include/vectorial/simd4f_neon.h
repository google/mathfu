#ifndef VECTORIAL_SIMD4F_NEON_H
#define VECTORIAL_SIMD4F_NEON_H

#include <arm_neon.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef float32x4_t simd4f;

union _simd4f_union {
    simd4f s ;
    float f[4];
};



vectorial_inline simd4f simd4f_create(float x, float y, float z, float w) {
    simd4f s = { x, y, z, w };
    return s;
}

vectorial_inline simd4f simd4f_splat(float v) { 
    simd4f s = vdupq_n_f32(v);
    return s;
}

vectorial_inline simd4f simd4f_add(simd4f lhs, simd4f rhs) {
    simd4f ret = vaddq_f32(lhs, rhs);
    return ret;
}

vectorial_inline simd4f simd4f_sub(simd4f lhs, simd4f rhs) {
    simd4f ret = vsubq_f32(lhs, rhs);
    return ret;
}

vectorial_inline simd4f simd4f_mul(simd4f lhs, simd4f rhs) {
    simd4f ret = vmulq_f32(lhs, rhs);
    return ret;
}

vectorial_inline simd4f simd4f_div(simd4f lhs, simd4f rhs) {
    simd4f recip = vrecpsq_f32( simd4f_create(1.0f, 1.0f, 1.0f, 1.0f), rhs );
    simd4f ret = vmulq_f32(lhs, recip);
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

