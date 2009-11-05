#ifndef VECTORIAL_CONFIG_H
#define VECTORIAL_CONFIG_H


#ifndef VECTORIAL_FORCED
    #if defined(__SSE__)

        #define VECTORIAL_SSE

    #elif defined(__ARM_NEON__) 

        #define VECTORIAL_NEON

    #elif defined(__GNUC__)

        #define VECTORIAL_GNU

    #else

        #define VECTORIAL_SCALAR

    #endif
#endif



#ifdef VECTORIAL_SCALAR
    #define VECTORIAL_SIMD_TYPE "scalar"
#endif

#ifdef VECTORIAL_SSE
    #define VECTORIAL_SIMD_TYPE "sse"
#endif

#ifdef VECTORIAL_NEON
    #define VECTORIAL_SIMD_TYPE "neon"
#endif

#ifdef VECTORIAL_GNU
    #define VECTORIAL_SIMD_TYPE "gnu"
#endif



#if defined(VECTORIAL_FORCED) && !defined(VECTORIAL_SIMD_TYPE)
  #error VECTORIAL_FORCED set but no simd-type found, try f.ex. VECTORIAL_SCALAR
#endif


#define vectorial_inline    inline
#define vectorial_restrict  restrict


#endif
