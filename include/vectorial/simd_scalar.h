#ifndef VECTORIAL_SIMD_SCALAR_H
#define VECTORIAL_SIMD_SCALAR_H


namespace vectorial {
    
    class simd4f {
    public:

        float x,y,z,w;
    
        inline simd4f() {}
        inline simd4f(const simd4f& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }    
        inline simd4f(float x_, float y_, float z_, float w_) { x=x_; y=y_; z=z_; w=w_; }

        inline simd4f operator+(const simd4f& other) const { 
            return simd4f(x+other.x, y+other.y, z+other.z, w+other.w); 
        }
    
    };

}

#endif

