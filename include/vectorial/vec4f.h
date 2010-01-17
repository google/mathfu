#ifndef VECTORIAL_VEC_H

#ifndef VECTORIAL_SIMD4F_H
  #include "vectorial/simd4f.h"
#endif



namespace vectorial {
    

    class vec4f : public simd4f {
    public:
    
        inline vec4f() : simd4f(0.0f, 0.0f, 0.0f, 0.0f) {}
    //    vec4(const vec4& v) : simd4f(v) {}
        inline vec4f(const simd4f& v) : simd4f(v) {}
        inline vec4f(float x, float y, float z, float w) : simd4f(x,y,z,w) {}
    
    
    };


    static float dot(const vec4f& lhs, const vec4f& rhs) {
        const simd4f m = lhs * rhs;
        return m.x() + m.y() + m.z() + m.w(); 
    }


    static float length_squared(const vec4f& v) {
        return dot(v,v); 
    }
    
    static float length(const vec4f& v) {
        return sqrtf(length_squared(v));
    }


    static vec4f normalize(const vec4f& v) {
        float invlen = 1.0f / length(v);
        return v * invlen;
    }



}



#ifdef VECTORIAL_OSTREAM
#include <ostream>

static std::ostream& operator<<(std::ostream& os, const vectorial::vec4f& v) {
    os << "[ " << v.x() << ", "
               << v.y() << ", "
               << v.z() << ", "
               << v.w() << " ]";
    return os;
}
#endif




#endif
