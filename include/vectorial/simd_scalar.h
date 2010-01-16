#ifndef VECTORIAL_SIMD_SCALAR_H
#define VECTORIAL_SIMD_SCALAR_H

#include <cstdlib> // size_t .. 

namespace vectorial {
    
    class simd4f {

    private:
        float f[4];

    public:

    
        vectorial_inline simd4f() {}
        vectorial_inline simd4f(const simd4f& v) { f[0] = v.f[0]; f[1] = v.f[1]; f[2] = v.f[2]; f[3] = v.f[3]; }
        vectorial_inline simd4f(float x_, float y_, float z_, float w_) { f[0]=x_; f[1]=y_; f[2]=z_; f[3]=w_; }

        vectorial_inline simd4f operator+(const simd4f& other) const { 
            return simd4f(f[0]+other.f[0], f[1]+other.f[1], f[2]+other.f[2], f[3]+other.f[3]); 
        }

        vectorial_inline simd4f operator*(const simd4f& other) const { 
            return simd4f(f[0]*other.f[0], f[1]*other.f[1], f[2]*other.f[2], f[3]*other.f[3]); 
        }

        vectorial_inline simd4f operator+(float other) const { 
            return simd4f(f[0]+other, f[1]+other, f[2]+other, f[3]+other); 
        }

        vectorial_inline simd4f operator*(float other) const { 
            return simd4f(f[0]*other, f[1]*other, f[2]*other, f[3]*other); 
        }

        
        vectorial_inline const float& operator [] (size_t i) const { return f[i]; }
        vectorial_inline float& operator [] (size_t i) { return f[i]; }

        vectorial_inline const float& x() const { return f[0]; }
        vectorial_inline const float& y() const { return f[1]; }
        vectorial_inline const float& z() const { return f[2]; }
        vectorial_inline const float& w() const { return f[3]; }
    
    };

}

#endif

