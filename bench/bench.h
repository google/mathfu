#ifndef BENCH_H
#define BENCH_H

#include <string>
#include <stdlib.h>

#ifdef __APPLE__
    #define BENCH_MACH
    #include <mach/mach_time.h>
    #include <stdint.h>
#elif defined(_WIN32)
    #define BENCH_QPC
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <malloc.h>
#else
    #define BENCH_GTOD
    #include <sys/time.h>
#endif


static void* memalign(size_t count, size_t align) {
    #ifdef _WIN32
    return _aligned_malloc(count,align);
    #else
    void *ptr;
    int e = posix_memalign(&ptr, align, count);
    //    if( e == EINVAL ) printf("EINVAL posix_memalign\n");
    //    if( e == ENOMEM ) printf("ENOMEM posix_memalign\n");
    return ptr;
    #endif
}

static void memfree(void* ptr) {
    #ifdef _WIN32
    _aligned_free(ptr);
    #else
    free(ptr);
    #endif
}

namespace profiler {

    #ifdef BENCH_GTOD
        typedef struct timeval time_t;
    #endif
    #ifdef BENCH_MACH
        typedef const uint64_t time_t;
    #endif
    #ifdef BENCH_QPC
        typedef LARGE_INTEGER time_t;
    #endif

    void init();
    time_t now();

    double diffTime(time_t start, time_t end);

}

std::string formatTime(double d, double relative=-1);
void profile(const char* name, void (*func)(), int iterations, int elements);


#endif
