#ifndef VECTORIAL_SIMD4X4F_SCALAR_H
#define VECTORIAL_SIMD4X4F_SCALAR_H


vectorial_inline simd4x4f simd4x4f_transpose(simd4x4f s) {
    simd4x4f d;
    d.x.x = s.x.x;
    d.x.y = s.y.x;
    d.x.z = s.z.x;
    d.x.w = s.w.x;

    d.y.x = s.x.y;
    d.y.y = s.y.y;
    d.y.z = s.z.y;
    d.y.w = s.w.y;

    d.z.x = s.x.z;
    d.z.y = s.y.z;
    d.z.z = s.z.z;
    d.z.w = s.w.z;

    d.w.x = s.x.w;
    d.w.y = s.y.w;
    d.w.z = s.z.w;
    d.w.w = s.w.w;
    return d;
}




#endif
