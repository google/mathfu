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

// todo: or is simd4f_splat(simd4f_getX(v))  better?

vectorial_inline simd4f simd4f_splat_x(simd4f v) {
    float32x2_t low = vget_low_f32(v);
    simd4f ret = vdupq_lane_f32(low, 0);
    return ret;
}

vectorial_inline simd4f simd4f_splat_y(simd4f v) { 
    float32x2_t low = vget_low_f32(v);
    simd4f ret = vdupq_lane_f32(low, 1);
    return ret;
}

vectorial_inline simd4f simd4f_splat_z(simd4f v) { 
    float32x2_t high = vget_high_f32(v);
    simd4f ret = vdupq_lane_f32(high, 0);
    return ret;
}

vectorial_inline simd4f simd4f_splat_w(simd4f v) { 
    float32x2_t high = vget_high_f32(v);
    simd4f ret = vdupq_lane_f32(high, 1);
    return ret;
}

vectorial_inline simd4f simd4f_reciprocal(simd4f v) { 
//    return vrecpsq_f32(..,v);
    return vrecpeq_f32(v);
}

vectorial_inline simd4f simd4f_sqrt(simd4f v) { 
    return simd4f_reciprocal(vrsqrteq_f32(v));
}

vectorial_inline simd4f simd4f_rsqrt(simd4f v) { 
    return vrsqrteq_f32(v);
}



// arithmetics

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



vectorial_inline float simd4f_getX(simd4f s) { return vgetq_lane_f32(s, 0); }
vectorial_inline float simd4f_getY(simd4f s) { return vgetq_lane_f32(s, 1); }
vectorial_inline float simd4f_getZ(simd4f s) { return vgetq_lane_f32(s, 2); }
vectorial_inline float simd4f_getW(simd4f s) { return vgetq_lane_f32(s, 3); }


#ifdef __cplusplus
}
#endif


#endif

