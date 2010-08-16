#ifndef VECTORIAL_MAT4F_H

#ifndef VECTORIAL_SIMD4X4F_H
  #include "vectorial/simd4x4f.h"
#endif

#ifndef VECTORIAL_VEC4F_H
  #include "vectorial/vec4f.h"
#endif


namespace vectorial {
    

    class mat4f {
    public:

        simd4x4f value;
    
        inline mat4f() {}
        inline mat4f(const simd4x4f& v) : value(v) {}
        inline mat4f(vec4f v0, vec4f v1, vec4f v2, vec4f v3) : value(simd4x4f_create(v0.value, v1.value, v2.value, v3.value)) {}


        static mat4f identity() { mat4f m; simd4x4f_identity(&m.value); return m; }

    };


}



#ifdef VECTORIAL_OSTREAM
//#include <ostream>

static std::ostream& operator<<(std::ostream& os, const vectorial::mat4f& v) {

    os << "[ ";
    os << simd4f_get_x(v.value.x) << ", ";
    os << simd4f_get_x(v.value.y) << ", ";
    os << simd4f_get_x(v.value.z) << ", ";
    os << simd4f_get_x(v.value.w) << " ; ";

    os << simd4f_get_y(v.value.x) << ", ";
    os << simd4f_get_y(v.value.y) << ", ";
    os << simd4f_get_y(v.value.z) << ", ";
    os << simd4f_get_y(v.value.w) << " ; ";

    os << simd4f_get_z(v.value.x) << ", ";
    os << simd4f_get_z(v.value.y) << ", ";
    os << simd4f_get_z(v.value.z) << ", ";
    os << simd4f_get_z(v.value.w) << " ; ";

    os << simd4f_get_w(v.value.x) << ", ";
    os << simd4f_get_w(v.value.y) << ", ";
    os << simd4f_get_w(v.value.z) << ", ";
    os << simd4f_get_w(v.value.w) << " ]";

    return os;
}
#endif




#endif
