#ifndef VECTORIAL_SIMD4X4_H
#define VECTORIAL_SIMD4X4_H


#include "simd4f.h"

/*
  Note, x,y,z,w are conceptually columns with matrix math.
*/

typedef struct {
    simd4f x,y,z,w;
} simd4x4f;



vectorial_inline simd4x4f simd4x4f_create(simd4f x, simd4f y, simd4f z, SIMD_PARAM(simd4f, w)) {
    simd4x4f s = { x, y, z, w };
    return s;
}


vectorial_inline void simd4x4f_identity(simd4x4f* m) {
    *m = simd4x4f_create( simd4f_create(1.0f, 0.0f, 0.0f, 0.0f),
                          simd4f_create(0.0f, 1.0f, 0.0f, 0.0f),
                          simd4f_create(0.0f, 0.0f, 1.0f, 0.0f),
                          simd4f_create(0.0f, 0.0f, 0.0f, 1.0f));
}



vectorial_inline void simd4x4f_uload(simd4x4f* m, float *f) {

    m->x = simd4f_uload4(f + 0);
    m->y = simd4f_uload4(f + 4);
    m->z = simd4f_uload4(f + 8);
    m->w = simd4f_uload4(f + 12);

}





#ifdef VECTORIAL_SCALAR
    #include "simd4x4f_scalar.h"
#elif defined(VECTORIAL_SSE)
    #include "simd4x4f_sse.h"
#elif defined(VECTORIAL_GNU)
    #include "simd4x4f_gnu.h"
#elif defined(VECTORIAL_NEON)
    #include "simd4x4f_neon.h"
#else
    #error No implementation defined
#endif

vectorial_inline void simd4x4f_sum(simd4x4f* a, simd4f* out) {
    simd4f t;
    t = simd4f_add(a->x, a->y);
    t = simd4f_add(t, a->z);
    t = simd4f_add(t, a->w);
    *out = t;
}

vectorial_inline void simd4x4f_matrix_vector_mul(simd4x4f* a, simd4f * b, simd4f* out) {

    simd4x4f bbbb = simd4x4f_create( simd4f_splat_x(*b),
                                     simd4f_splat_y(*b),
                                     simd4f_splat_z(*b),
                                     simd4f_splat_w(*b) );

    simd4x4f ab = simd4x4f_create( simd4f_mul(a->x, bbbb.x),
                                   simd4f_mul(a->y, bbbb.y),
                                   simd4f_mul(a->z, bbbb.z),
                                   simd4f_mul(a->w, bbbb.w) );

    simd4x4f_sum(&ab, out);

}


vectorial_inline void simd4x4f_matrix_mul(simd4x4f* a, simd4x4f* b, simd4x4f* out) {

    simd4x4f_matrix_vector_mul(a, &b->x, &out->x);
    simd4x4f_matrix_vector_mul(a, &b->y, &out->y);
    simd4x4f_matrix_vector_mul(a, &b->z, &out->z);
    simd4x4f_matrix_vector_mul(a, &b->w, &out->w);

}



#ifdef __cplusplus

    #ifdef VECTORIAL_OSTREAM
        #include <ostream>

        static std::ostream& operator<<(std::ostream& os, const simd4x4f& v) {
            os << "simd4x4f(simd4f(" << simd4f_get_x(v.x) << ", "
                       << simd4f_get_x(v.y) << ", "
                       << simd4f_get_x(v.z) << ", "
                       << simd4f_get_x(v.w) << "),\n"
                       << "         simd4f(" << simd4f_get_y(v.x) << ", "
                       << simd4f_get_y(v.y) << ", "
                       << simd4f_get_y(v.z) << ", "
                       << simd4f_get_y(v.w) << "),\n"
                       << "         simd4f(" << simd4f_get_z(v.x) << ", "
                       << simd4f_get_z(v.y) << ", "
                       << simd4f_get_z(v.z) << ", "
                       << simd4f_get_z(v.w) << "),\n"
                       << "         simd4f(" << simd4f_get_w(v.x) << ", "
                       << simd4f_get_w(v.y) << ", "
                       << simd4f_get_w(v.z) << ", "
                       << simd4f_get_w(v.w) << "))";
            return os;
        }
    #endif

#endif





#endif 
