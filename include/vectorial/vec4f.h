#ifndef VECTORIAL_VEC_H

#ifndef VECTORIAL_SIMD4F_H
  #include "vectorial/simd4f.h"
#endif



namespace vectorial {
    

    class vec4f {
    public:

        simd4f value;
    
        inline vec4f() {}
    //    vec4(const vec4& v) : simd4f(v) {}
        inline vec4f(const simd4f& v) : value(v) {}
        inline vec4f(float x, float y, float z, float w) : value( simd4f_create(x,y,z,w) ) {}
    
        inline float x() const { return simd4f_getX(value); }
        inline float y() const { return simd4f_getY(value); }
        inline float z() const { return simd4f_getZ(value); }
        inline float w() const { return simd4f_getW(value); }

    
        enum { elements = 4 };


    };


    vectorial_inline vec4f operator+(const vec4f& lhs, const vec4f& rhs) {
        return vec4f( simd4f_add(lhs.value, rhs.value) );
    }

    vectorial_inline vec4f operator-(const vec4f& lhs, const vec4f& rhs) {
        return vec4f( simd4f_sub(lhs.value, rhs.value) );
    }

    vectorial_inline vec4f operator*(const vec4f& lhs, const vec4f& rhs) {
        return vec4f( simd4f_mul(lhs.value, rhs.value) );
    }

    vectorial_inline vec4f operator/(const vec4f& lhs, const vec4f& rhs) {
        return vec4f( simd4f_div(lhs.value, rhs.value) );
    }



    vectorial_inline vec4f operator+(const vec4f& lhs, float rhs) {
        return vec4f( simd4f_add(lhs.value, simd4f_splat(rhs)) );
    }

    vectorial_inline vec4f operator-(const vec4f& lhs, float rhs) {
        return vec4f( simd4f_sub(lhs.value, simd4f_splat(rhs)) );
    }

    vectorial_inline vec4f operator*(const vec4f& lhs, float rhs) {
        return vec4f( simd4f_mul(lhs.value, simd4f_splat(rhs)) );
    }

    vectorial_inline vec4f operator/(const vec4f& lhs, float rhs) {
        return vec4f( simd4f_div(lhs.value, simd4f_splat(rhs)) );
    }

    vectorial_inline vec4f operator+(float lhs, const vec4f& rhs) {
        return vec4f( simd4f_add(simd4f_splat(lhs), rhs.value) );
    }

    vectorial_inline vec4f operator-(float lhs, const vec4f& rhs) {
        return vec4f( simd4f_sub(simd4f_splat(lhs), rhs.value) );
    }

    vectorial_inline vec4f operator*(float lhs, const vec4f& rhs) {
        return vec4f( simd4f_mul(simd4f_splat(lhs), rhs.value) );
    }

    vectorial_inline vec4f operator/(float lhs, const vec4f& rhs) {
        return vec4f( simd4f_div(simd4f_splat(lhs), rhs.value) );
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
