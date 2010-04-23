
#include "bench.h"
#include <stdlib.h>

#include <iostream>
#include "vectorial/simd4x4f.h"

#define NUM (81920)
#define ITER 100
//using namespace vectorial;

namespace {
    simd4x4f* alloc_simd4x4f(size_t n) {
        void *ptr = memalign(n*sizeof(simd4x4f), 16);
        return static_cast<simd4x4f*>(ptr);
    }    
}



static simd4x4f * a;
static simd4x4f * b;
static simd4x4f * c;



static simd4x4f add_4x4(SIMD_PARAM(simd4x4f, a), SIMD_PARAM(simd4x4f, b)) {
    return simd4x4f_create(
        simd4f_add(a.x, b.x),
        simd4f_add(a.y, b.y),
        simd4f_add(a.z, b.z),
        simd4f_add(a.w, b.w)
        );
}

static simd4x4f add_4x4_rp(simd4x4f *a, simd4x4f *b) {
    return simd4x4f_create(
        simd4f_add(a->x, b->x),
        simd4f_add(a->y, b->y),
        simd4f_add(a->z, b->z),
        simd4f_add(a->w, b->w)
        );
}


static void add_4x4_p(simd4x4f *a, simd4x4f *b, simd4x4f *out) {
    out->x = simd4f_add(a->x, b->x);
    out->y = simd4f_add(a->y, b->y);
    out->z = simd4f_add(a->z, b->z);
    out->w = simd4f_add(a->w, b->w);
}




void quad_return_func() {
    
    
    simd4x4f* aa = a;
    simd4x4f* bb = b;
    simd4x4f* cc = c;
    
    for(size_t i = 0; i < NUM; ++i)
    {
        bb[i] = add_4x4(aa[i], bb[i]);
    }    
}


void quad_pointer_func() {
    
    simd4x4f* aa = a;
    simd4x4f* bb = b;
    simd4x4f* cc = c;
    
    for(size_t i = 0; i < NUM; ++i)
    {
        add_4x4_p(&aa[i], &bb[i], &bb[i]);
    }
    

}

void quad_pointer_return_func() {
    
    simd4x4f* aa = a;
    simd4x4f* bb = b;
    simd4x4f* cc = c;
    
    for(size_t i = 0; i < NUM; ++i)
    {
        bb[i] = add_4x4_rp(&aa[i], &bb[i]);
    }    
    

}


void quad_bench() {

    a = alloc_simd4x4f(NUM);
    b = alloc_simd4x4f(NUM);
    c = alloc_simd4x4f(NUM);


    for(size_t i = 0; i < NUM; ++i)
    {
        simd4f t = simd4f_create(i,i,i,i); 
        simd4f t2 = simd4f_create(NUM-i,NUM-i,NUM-i,NUM-i); 
        a[i]=simd4x4f_create(t,t,t,t);
        b[i]=simd4x4f_create(t2,t2,t2,t2);
    }
        
    profile("quad return-value", quad_return_func, ITER, NUM);
    profile("quad pass-by-pointer", quad_pointer_func, ITER, NUM);
    profile("quad pass-by-pointer return-value", quad_pointer_return_func, ITER, NUM);

    memfree(a);
    memfree(b);
    memfree(c);


}
