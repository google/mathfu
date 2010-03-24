#ifndef VECTORIAL_SIMD4F_GNU_H
#define VECTORIAL_SIMD4F_GNU_H

#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef float simd4f __attribute__ ((vector_size (16)));

union _simd4f_union {
    simd4f s ;
    float f[4];
};

vectorial_inline float simd4f_getX(simd4f s) { _simd4f_union u={s}; return u.f[0]; }
vectorial_inline float simd4f_getY(simd4f s) { _simd4f_union u={s}; return u.f[1]; }
vectorial_inline float simd4f_getZ(simd4f s) { _simd4f_union u={s}; return u.f[2]; }
vectorial_inline float simd4f_getW(simd4f s) { _simd4f_union u={s}; return u.f[3]; }


vectorial_inline simd4f simd4f_create(float x, float y, float z, float w) {
    simd4f s = { x, y, z, w };
    return s;
}

vectorial_inline simd4f simd4f_uload4(const float *ary) {
    simd4f s = { ary[0], ary[1], ary[2], ary[3] };
    return s;
}

vectorial_inline simd4f simd4f_uload3(const float *ary) {
    simd4f s = { ary[0], ary[1], ary[2], 0 };
    return s;
}

vectorial_inline simd4f simd4f_uload2(const float *ary) {
    simd4f s = { ary[0], ary[1], 0, 0 };
    return s;
}



vectorial_inline simd4f simd4f_splat(float v) { 
    simd4f s = { v, v, v, v }; 
    return s;
}

vectorial_inline simd4f simd4f_splat_x(simd4f v) { 
    float s = simd4f_getX(v);
    simd4f ret = { s, s, s, s }; 
    return ret;
}

vectorial_inline simd4f simd4f_splat_y(simd4f v) { 
    float s = simd4f_getY(v);
    simd4f ret = { s, s, s, s }; 
    return ret;
}

vectorial_inline simd4f simd4f_splat_z(simd4f v) { 
    float s = simd4f_getZ(v);
    simd4f ret = { s, s, s, s }; 
    return ret;
}

vectorial_inline simd4f simd4f_splat_w(simd4f v) { 
    float s = simd4f_getW(v);
    simd4f ret = { s, s, s, s }; 
    return ret;
}

vectorial_inline simd4f simd4f_reciprocal(simd4f v) { 
    return simd4f_splat(1.0f) / v;
}

vectorial_inline simd4f simd4f_sqrt(simd4f v) { 
    simd4f ret = { sqrtf(simd4f_getX(v)), sqrtf(simd4f_getY(v)), sqrtf(simd4f_getZ(v)), sqrtf(simd4f_getW(v)) };
    return ret;
}

vectorial_inline simd4f simd4f_rsqrt(simd4f v) { 
    return simd4f_splat(1.0f) / simd4f_sqrt(v);
}



vectorial_inline simd4f simd4f_add(simd4f lhs, simd4f rhs) {
    simd4f ret = lhs + rhs;
    return ret;
}

vectorial_inline simd4f simd4f_sub(simd4f lhs, simd4f rhs) {
    simd4f ret = lhs - rhs;
    return ret;
}

vectorial_inline simd4f simd4f_mul(simd4f lhs, simd4f rhs) {
    simd4f ret = lhs * rhs;
    return ret;
}

vectorial_inline simd4f simd4f_div(simd4f lhs, simd4f rhs) {
    simd4f ret = lhs / rhs;
    return ret;
}


vectorial_inline simd4f simd4f_cross3(simd4f l, simd4f r) {
    _simd4f_union lhs = {l};
    _simd4f_union rhs = {r};
    
    return simd4f_create( lhs.f[1] * rhs.f[2] - lhs.f[2] * rhs.f[1],
                          lhs.f[2] * rhs.f[0] - lhs.f[0] * rhs.f[2],
                          lhs.f[0] * rhs.f[1] - lhs.f[1] * rhs.f[0], 0);
}



#ifdef __cplusplus
}
#endif


#endif

