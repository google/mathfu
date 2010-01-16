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


    #ifdef VECTORIAL_SCALAR

    static float dot(const vec4f& lhs, const vec4f& rhs) {
        return lhs.f[0]*rhs.f[0] + 
               lhs.f[1]*rhs.f[1] + 
               lhs.f[2]*rhs.f[2] + 
               lhs.f[3]*rhs.f[3]; 
    }


    static float length_squared(const vec4f& v) {
        return v.f[0]*v.f[0] + 
               v.f[1]*v.f[1] + 
               v.f[2]*v.f[2] + 
               v.f[3]*v.f[3]; 
    }
    
    static float length(const vec4f& v) {
        return sqrtf(length_squared(v));
    }
    
    #else

    static float length_squared(const simd4f& v) {
        // TODO:
        return v.value.f[0]*v.value.f[0] + 
               v.value.f[1]*v.value.f[1] + 
               v.value.f[2]*v.value.f[2] + 
               v.value.f[3]*v.value.f[3]; 
    }
    
    static float length(const simd4f& v) {
        // TODO:
        return sqrtf(length_squared(v));
    }
    
    static float dot(const simd4f& lhs, const simd4f& rhs) {
        return lhs.value.f[0]*rhs.value.f[0] + 
               lhs.value.f[1]*rhs.value.f[1] + 
               lhs.value.f[2]*rhs.value.f[2] + 
               lhs.value.f[3]*rhs.value.f[3]; 
    }
    
    #endif




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
