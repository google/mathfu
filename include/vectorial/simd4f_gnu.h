#ifndef VECTORIAL_SIMD4F_GNU_H
#define VECTORIAL_SIMD4F_GNU_H



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





#ifdef __cplusplus
}
#endif


#endif

