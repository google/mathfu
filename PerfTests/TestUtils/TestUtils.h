#ifndef MATHFU_PERFTESTS_TESTUTILS_H_
#define MATHFU_PERFTESTS_TESTUTILS_H_

// This macro stands in for a double loop over a vector with size elements. This
// double loop is repeated iterations times to create more accurate performance
// test
#define PERFTEST_2D_VECTOR_LOOP(iterations, size) \
  for (unsigned int k = 0; k < iterations; k++) \
  for (unsigned int i = 0; i < size; i++) \
  for (unsigned int j = 0; j < size; j++)

#ifdef WIN32
struct timeval {
  long int tv_sec;
  long int tv_usec;
}

void gettimeofday(timeval* time, void*) {
  LARGE_INTEGER largetime;
  QueryPerformanceCounter(&largetime);
  time->tv_sec = 0;
  time->tv_usec = largetime.QuadPart;
}

void timersub(timeval* end, timeval* start, timeval* result) {
  result->tv_sec = 0;
  result->tv_usec = end->tv_usec - start->tv_usec;
}
#endif  // WIN32

#endif  // MATHFU_PERFTESTS_TESTUTILS_H_
