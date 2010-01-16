#ifndef VECTORIAL_SIMD4F_SCALAR_H
#define VECTORIAL_SIMD4F_SCALAR_H

#include <cstdlib> // size_t .. 
#include <cmath>

namespace vectorial {
    
    class simd4f {
    public:
        float f[4];

    
        vectorial_inline simd4f() {}
        vectorial_inline simd4f(const simd4f& v) { f[0] = v.f[0]; f[1] = v.f[1]; f[2] = v.f[2]; f[3] = v.f[3]; }
        vectorial_inline simd4f(float x_, float y_, float z_, float w_) { f[0]=x_; f[1]=y_; f[2]=z_; f[3]=w_; }


        
        vectorial_inline const float& operator [] (size_t i) const { return f[i]; }
        vectorial_inline float& operator [] (size_t i) { return f[i]; }

        vectorial_inline const float& x() const { return f[0]; }
        vectorial_inline const float& y() const { return f[1]; }
        vectorial_inline const float& z() const { return f[2]; }
        vectorial_inline const float& w() const { return f[3]; }
    
    };


    static simd4f operator+(const simd4f& lhs, const simd4f& rhs) { 
        return simd4f(lhs.f[0]+rhs.f[0], lhs.f[1]+rhs.f[1], lhs.f[2]+rhs.f[2], lhs.f[3]+rhs.f[3]); 
    }

    static simd4f operator*(const simd4f& lhs, const simd4f& rhs) { 
        return simd4f(lhs.f[0]*rhs.f[0], lhs.f[1]*rhs.f[1], lhs.f[2]*rhs.f[2], lhs.f[3]*rhs.f[3]); 
    }

    static simd4f operator+(const simd4f& lhs, float rhs) { 
        return simd4f(lhs.f[0]+rhs, lhs.f[1]+rhs, lhs.f[2]+rhs, lhs.f[3]+rhs); 
    }

    static simd4f operator*(const simd4f& lhs, float rhs) { 
        return simd4f(lhs.f[0]*rhs, lhs.f[1]*rhs, lhs.f[2]*rhs, lhs.f[3]*rhs); 
    }


    static simd4f sin(const simd4f& v) {
        return simd4f(sinf(v.x()), sinf(v.y()), sinf(v.z()), sinf(v.w()));
    }


}

#endif

