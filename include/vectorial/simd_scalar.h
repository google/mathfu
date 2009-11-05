#ifndef VECTORIAL_SIMD_SCALAR_H
#define VECTORIAL_SIMD_SCALAR_H


namespace vectorial {
    
    class simd4f {

    private:
        float f[4];

    public:

    
        inline simd4f() {}
        inline simd4f(const simd4f& v) { f[0] = v[0]; f[1] = v[1]; f[2] = v[2]; f[3] = v[3]; }
        inline simd4f(float x_, float y_, float z_, float w_) { f[0]=x_; f[1]=y_; f[2]=z_; f[3]=w_; }

        inline simd4f operator+(const simd4f& other) const { 
            return simd4f(f[0]+other[0], f[1]+other[1], f[2]+other[2], f[3]+other[3]); 
        }
        
        vectorial_inline const float& operator[](size_t i) const { return f[i]; }
        vectorial_inline float& operator[](size_t i) { return f[i]; }
    
    };

}

#endif

