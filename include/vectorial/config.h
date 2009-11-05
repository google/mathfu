#ifndef VECTORIAL_CONFIG_H
#define VECTORIAL_CONFIG_H

#if defined(__SSE__)

  #define VECTORIAL_SSE
  #define VECTORIAL_SIMD_TYPE "sse"

#elif defined(__ARM_NEON__) 

  #define VECTORIAL_NEON
  #define VECTORIAL_SIMD_TYPE "neon"

#elif defined(__GNUC__)

  #define VECTORIAL_GCC
  #define VECTORIAL_SIMD_TYPE "gcc"

#else

  #define VECTORIAL_SCALAR
  #define VECTORIAL_SIMD_TYPE "scalar"
  #define VECTORIAL_SIMD_INC "vectorial/simd_scalar.h"

#endif

#define vectorial_inline    inline
#define vectorial_restrict  restrict


#endif
