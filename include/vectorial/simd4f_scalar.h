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

    static simd4f operator/(const simd4f& lhs, const simd4f& rhs) { 
        return simd4f(lhs.f[0]/rhs.f[0], lhs.f[1]/rhs.f[1], lhs.f[2]/rhs.f[2], lhs.f[3]/rhs.f[3]); 
    }


    static simd4f operator+(const simd4f& lhs, float rhs) { 
        return simd4f(lhs.f[0]+rhs, lhs.f[1]+rhs, lhs.f[2]+rhs, lhs.f[3]+rhs); 
    }

    static simd4f operator*(const simd4f& lhs, float rhs) { 
        return simd4f(lhs.f[0]*rhs, lhs.f[1]*rhs, lhs.f[2]*rhs, lhs.f[3]*rhs); 
    }

    static simd4f operator/(const simd4f& lhs, float rhs) { 
        return simd4f(lhs.f[0]/rhs, lhs.f[1]/rhs, lhs.f[2]/rhs, lhs.f[3]/rhs); 
    }


    static simd4f operator+(float lhs, const simd4f& rhs) { 
        return simd4f(lhs+rhs.f[0], lhs+rhs.f[1], lhs+rhs.f[2], lhs+rhs.f[3]); 
    }

    static simd4f operator*(float lhs, const simd4f& rhs) { 
        return simd4f(lhs*rhs.f[0], lhs*rhs.f[1], lhs*rhs.f[2], lhs*rhs.f[3]); 
    }

    static simd4f operator/(float lhs, const simd4f& rhs) { 
        return simd4f(lhs/rhs.f[0], lhs/rhs.f[1], lhs/rhs.f[2], lhs/rhs.f[3]); 
    }




    static simd4f sin(const simd4f& v) {
        return simd4f(sinf(v.x()), sinf(v.y()), sinf(v.z()), sinf(v.w()));
    }

    static simd4f cos(const simd4f& v) {
        return simd4f(cosf(v.x()), cosf(v.y()), cosf(v.z()), cosf(v.w()));
    }
    
    static simd4f tan(const simd4f& v) {
        return simd4f(tanf(v.x()), tanf(v.y()), tanf(v.z()), tanf(v.w()));
    }


    static simd4f asin(const simd4f& v) {
        return simd4f(asinf(v.x()), asinf(v.y()), asinf(v.z()), asinf(v.w()));
    }

    static simd4f acos(const simd4f& v) {
        return simd4f(acosf(v.x()), acosf(v.y()), acosf(v.z()), acosf(v.w()));
    }
    
    static simd4f atan(const simd4f& v) {
        return simd4f(atanf(v.x()), atanf(v.y()), atanf(v.z()), atanf(v.w()));
    }

    static simd4f atan2(const simd4f& v, const simd4f& u) {
        return simd4f(atan2f(v.x(), u.x()), atan2f(v.y(), u.y()), atan2f(v.z(), u.z()), atan2f(v.w(), u.w()));
    }

    static simd4f sqrt(const simd4f& v) {
        return simd4f(sqrtf(v.x()), sqrtf(v.y()), sqrtf(v.z()), sqrtf(v.w()));
    }


}

#endif

