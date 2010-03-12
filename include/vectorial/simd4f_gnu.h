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



static simd4f simd4f_create(float x, float y, float z, float w) {
    simd4f s = { x, y, z, w };
    return s;
}

static simd4f simd4f_splat(float v) { 
    simd4f s = { v, v, v, v }; 
    return s;
}

static simd4f simd4f_add(simd4f lhs, simd4f rhs) {
    simd4f ret = lhs + rhs;
    return ret;
}


static float simd4f_getX(simd4f s) { _simd4f_union u={s}; return u.f[0]; }
static float simd4f_getY(simd4f s) { _simd4f_union u={s}; return u.f[1]; }
static float simd4f_getZ(simd4f s) { _simd4f_union u={s}; return u.f[2]; }
static float simd4f_getW(simd4f s) { _simd4f_union u={s}; return u.f[3]; }


#ifdef __cplusplus
}
#endif


#endif

