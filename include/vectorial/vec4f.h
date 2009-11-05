#ifndef VECTORIAL_VEC_H

#ifndef VECTORIAL_SIMD_H
  #include "vectorial/simd.h"
#endif



namespace vectorial {
    

    class vec4f : public simd4f {
    public:
    
        inline vec4f() : simd4f(0.0f, 0.0f, 0.0f, 0.0f) {}
    //    vec4(const vec4& v) : simd4f(v) {}
        inline vec4f(const simd4f& v) : simd4f(v) {}
        inline vec4f(float x, float y, float z, float w) : simd4f(x,y,z,w) {}
    
    
    };


}



#ifdef VECTORIAL_OSTREAM
#include <ostream>

std::ostream& operator<<(std::ostream& os, const vectorial::vec4f& v) {
    os << "[ " << v[0] << ", "
               << v[1] << ", "
               << v[2] << ", "
               << v[3] << " ]";
    return os;
}
#endif




#endif
