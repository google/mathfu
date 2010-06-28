#include "bench.h"
#include <sstream>
#include <iostream>
#include "vectorial/config.h"


namespace profiler {

    #ifdef BENCH_MACH
    mach_timebase_info_data_t info;
    void init() {
        mach_timebase_info(&info);
    }
    #endif
    
    #ifdef BENCH_GTOD
    void init() {
    }
    #endif

    #ifdef BENCH_QPC
    double frequency;
    void init() {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        frequency = (double)freq.QuadPart;
    }
    #endif


    time_t now() {

        #ifdef BENCH_MACH
        return mach_absolute_time();
        #endif

        #ifdef BENCH_GTOD
        time_t v;
        gettimeofday(&v, NULL);
        return v;
        #endif
        
        #ifdef BENCH_QPC
        LARGE_INTEGER v;
        QueryPerformanceCounter(&v);
        return v;
        #endif

    }
    
    
    double diffTime(time_t start, time_t end) {
        
        #ifdef BENCH_GTOD
        return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
        #endif
        
        #ifdef BENCH_MACH        
        return ((end-start) * info.numer / info.denom) / 1000000000.0;
        #endif

        #ifdef BENCH_QPC
        return (end.QuadPart - start.QuadPart) / frequency;
        #endif
    }
    
}


std::string formatTime(double d, double relative ) {
    const double sec   = 1.0;
    const double milli = 0.001;
    const double micro = 0.000001;
    const double nano  = 0.000000001;
    std::stringstream ss;
    if( relative < 0.0) relative=d;
    if( relative >= sec ) ss << d << "s";
    else if( relative >= milli ) ss << d/milli << "ms";
    else if( relative >= micro ) ss << d/micro <<"us";
    else ss << d/nano << "ns";
    return ss.str();
}

void profile(const char* name, void (*func)(), int iterations, int elements) {
    
    profiler::init();
    profiler::time_t start = profiler::now();
    for(int i = 0; i < iterations; ++i)
    {
        func();
    }
    profiler::time_t end = profiler::now();
    
    std::cout << "Using simd: " << VECTORIAL_SIMD_TYPE << std::endl;
    std::cout << "Testing: " << name << std::endl;
    std::cout << "Duration " << formatTime(profiler::diffTime(start,end)) << std::endl;
    std::cout << "Per iter " << formatTime(profiler::diffTime(start,end) / iterations) << std::endl;
    std::cout << "Per item " << formatTime(profiler::diffTime(start,end) / iterations / elements) << std::endl;

    
}

void add_bench();
void dot_bench();
void quad_bench();
void matrix_bench();

int main() {
    
//    add_bench();
//    dot_bench();
//    quad_bench();
    matrix_bench();

    return 0;
}

