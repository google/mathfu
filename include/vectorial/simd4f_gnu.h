#ifndef VECTORIAL_SIMD4F_SCALAR_H
#define VECTORIAL_SIMD4F_SCALAR_H


#ifdef __cplusplus
extern "C" {
#endif


union simd4f { 
    float v __attribute__ ((vector_size (16)));
    float c[4];
};

static simd4f simd4f_create(float x, float y, float z, float w) {
    simd4f s = {{ x, y, z, w }};
    return s;
}

static float simd4f_getX(simd4f s) { return s.c[0]; }
static float simd4f_getY(simd4f s) { return s.c[1]; }
static float simd4f_getZ(simd4f s) { return s.c[2]; }
static float simd4f_getW(simd4f s) { return s.c[3]; }

/*

simd4f simd4f_add(simd4f, simd4f);


simd4f simd4f_dot4(simd4f, simd4f);
simd4f simd4f_dot3(simd4f, simd4f);

float simd4f_dot4_float(simd4f, simd4f);
float simd4f_dot3_float(simd4f, simd4f);
*/

#ifdef __cplusplus
}
#endif


#endif

