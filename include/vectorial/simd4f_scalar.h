#ifndef VECTORIAL_SIMD4F_SCALAR_H
#define VECTORIAL_SIMD4F_SCALAR_H



#ifdef __cplusplus
extern "C" {
#endif


struct simd4f { 
    float x;
    float y; 
    float z; 
    float w;
};



static simd4f simd4f_create(float x, float y, float z, float w) {
    simd4f s = { x, y, z, w };
    return s;
}

static simd4f simd4f_splat(float v) { 
    simd4f s = { v, v, v, v }; 
    return s;
}


static float simd4f_getX(simd4f s) { return s.x; }
static float simd4f_getY(simd4f s) { return s.y; }
static float simd4f_getZ(simd4f s) { return s.z; }
static float simd4f_getW(simd4f s) { return s.w; }


#ifdef __cplusplus
}
#endif


#endif

