/*
* Copyright 2014 Google Inc. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#ifndef MATHFU_VECTORS_GLSL_MAPPINGS
#define MATHFU_VECTORS_GLSL_MAPPINGS

namespace mathfu {

typedef Vector<float, 2> vec2;
typedef Vector<float, 3> vec3;
typedef Vector<float, 4> vec4;

typedef Vector<int, 2> vec2i;
typedef Vector<int, 3> vec3i;
typedef Vector<int, 4> vec4i;

typedef Matrix<float, 2, 2> mat2;
typedef Matrix<float, 3, 3> mat3;
typedef Matrix<float, 4, 4> mat4;

template<class T>
inline Vector<T, 3> cross(const Vector<T, 3>& v1, const Vector<T, 3>& v2) {
  return Vector<T, 3>::CrossProduct(v1,v2);
}

template<class TV>
inline typename TV::Scalar dot(const TV& v1, const TV& v2) {
  return TV::DotProduct(v1,v2);
}

template<class TV>
inline TV normalize(const TV& v1) {
  return v1.Normalized();
}

}  // namespace mathfu

#endif  // MATHFU_VECTORS_GLSL_MAPPINGS
