#ifndef VECTORIAL_SIMD4X4F_SSE_H
#define VECTORIAL_SIMD4X4F_SSE_H



vectorial_inline simd4x4f simd4x4f_transpose(simd4x4f s) {
    simd4x4f d = s;
    _MM_TRANSPOSE4_PS(d.x, d.y, d.z, d.w);
    return d;
}




#endif
