
#include "bench.h"
#include <stdlib.h>

#include <iostream>
#include "vectorial/simd4x4f.h"

#define NUM (819200)
#define ITER 100
//using namespace vectorial;

namespace {
    simd4x4f* alloc_vec4x4f(size_t n) {
        void *ptr = memalign(n*sizeof(simd4x4f), 16);
        return static_cast<simd4x4f*>(ptr);
    }    
}



static simd4x4f * a;
static simd4x4f * b;
static simd4x4f * c;




void matrix_func() {
    
    simd4x4f* vectorial_restrict aa = a;
    simd4x4f* vectorial_restrict bb = b;
    simd4x4f* vectorial_restrict cc = c;
    
    for(size_t i = 0; i < NUM; ++i)
    {
        simd4x4f_matrix_mul(&aa[i], &bb[i], &bb[i]);
    }    
}

void matrix_bench() {

    a = alloc_vec4x4f(NUM);
    b = alloc_vec4x4f(NUM);
    c = alloc_vec4x4f(NUM);


    for(size_t i = 0; i < NUM; ++i)
    {
        simd4f v = simd4f_create(i,i,i,i);
        simd4f vi = simd4f_create(NUM-i,NUM-i,NUM-i,NUM-i);
        a[i]=simd4x4f_create(v,v,v,v);
        b[i]=simd4x4f_create(vi,vi,vi,vi);
    }
        
    profile("matrix mul", matrix_func, ITER, NUM);

    memfree(a);
    memfree(b);
    memfree(c);


}
