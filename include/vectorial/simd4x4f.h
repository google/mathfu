#ifndef VECTORIAL_SIMD4X4_H
#define VECTORIAL_SIMD4X4_H


#include "simd4f.h"


typedef struct {
    simd4f x,y,z,w;
} simd4x4f;



vectorial_inline simd4x4f simd4x4f_create(simd4f x, simd4f y, simd4f z, simd4f w) {
    simd4x4f s = { x, y, z, w };
    return s;
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
            os << "simd4x4f(simd4f(" << simd4f_getX(v.x) << ", "
                       << simd4f_getY(v.x) << ", "
                       << simd4f_getZ(v.x) << ", "
                       << simd4f_getW(v.x) << "),\n"
                       << "         simd4f(" << simd4f_getX(v.y) << ", "
                       << simd4f_getY(v.y) << ", "
                       << simd4f_getZ(v.y) << ", "
                       << simd4f_getW(v.y) << "),\n"
                       << "         simd4f(" << simd4f_getX(v.z) << ", "
                       << simd4f_getY(v.z) << ", "
                       << simd4f_getZ(v.z) << ", "
                       << simd4f_getW(v.z) << "),\n"
                       << "         simd4f(" << simd4f_getX(v.w) << ", "
                       << simd4f_getY(v.w) << ", "
                       << simd4f_getZ(v.w) << ", "
                       << simd4f_getW(v.w) << "))";
            return os;
        }
    #endif

#endif





#endif 
