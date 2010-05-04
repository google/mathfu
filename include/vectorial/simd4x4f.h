#ifndef VECTORIAL_SIMD4X4_H
#define VECTORIAL_SIMD4X4_H


#include "simd4f.h"


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






#ifdef __cplusplus

    #ifdef VECTORIAL_OSTREAM
        #include <ostream>

        static std::ostream& operator<<(std::ostream& os, const simd4x4f& v) {
            os << "simd4x4f(simd4f(" << simd4f_get_x(v.x) << ", "
                       << simd4f_get_y(v.x) << ", "
                       << simd4f_get_z(v.x) << ", "
                       << simd4f_get_w(v.x) << "),\n"
                       << "         simd4f(" << simd4f_get_x(v.y) << ", "
                       << simd4f_get_y(v.y) << ", "
                       << simd4f_get_z(v.y) << ", "
                       << simd4f_get_w(v.y) << "),\n"
                       << "         simd4f(" << simd4f_get_x(v.z) << ", "
                       << simd4f_get_y(v.z) << ", "
                       << simd4f_get_z(v.z) << ", "
                       << simd4f_get_w(v.z) << "),\n"
                       << "         simd4f(" << simd4f_get_x(v.w) << ", "
                       << simd4f_get_y(v.w) << ", "
                       << simd4f_get_z(v.w) << ", "
                       << simd4f_get_w(v.w) << "))";
            return os;
        }
    #endif

#endif





#endif 
