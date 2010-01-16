#ifndef BENCH_H
#define BENCH_H

#include <stdint.h>
#include <string>

#ifdef __APPLE__
    #define BENCH_MACH
    #include <mach/mach_time.h>
#else
    #define BENCH_GTOD
    #include <sys/time.h>
#endif


namespace profiler {

    #ifdef BENCH_GTOD
        typedef struct timeval time_t;
    #endif
    #ifdef BENCH_MACH
        typedef const uint64_t time_t;
    #endif

    void init();
    time_t now();

    double diffTime(time_t start, time_t end);

}

std::string formatTime(double d, double relative=-1);
void profile(void (*func)(), int iterations, int elements);


#endif
