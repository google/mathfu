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
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#ifdef __ANDROID__
#include <android/log.h>
#endif
#include <unistd.h>
#include <vector>
#include <matrices/Matrix_4x4.h>
#include <TestUtils/TestUtils.h>

#ifdef __ANDROID__
#define LOG_PRINT(...) \
  __android_log_print(ANDROID_LOG_VERBOSE, "VectorTest", __VA_ARGS__)
#else
#define LOG_PRINT(...) printf(__VA_ARGS__)
#endif

// Generate a random value of type T
template<class T> inline T getRand() {
  return (T)rand() / RAND_MAX;
}

// Number of elements to iterate over
static const size_t kMatrixSize = 1000;
using goomath::Matrix;

// This test creates a number of matrices and performs some mathematical
// operations on them in order to measure expected performance of matrix
// operations.
int main(int argc, char** argv) {
  typedef float T;
  struct timeval start, end, diff;
  int iterations = 100;
  // Create a array of matrices
  std::vector<Matrix<T, 4> > matrices;
  T final_sum = 0;
  Matrix<T, 4> mul = Matrix<T, 4>::Identity();
  for (int i = 0; i < kMatrixSize; i++) {
    Matrix<T, 4> mat(
      getRand<T>(), getRand<T>(), getRand<T>(), getRand<T>(),
      getRand<T>(), getRand<T>(), getRand<T>(), getRand<T>(),
      getRand<T>(), getRand<T>(), getRand<T>(), getRand<T>(),
      getRand<T>(), getRand<T>(), getRand<T>(), getRand<T>());
    matrices.push_back(mat);
  }
  // Start matrix performance code. Run a number of loops for more accurate
  // numbers.
  gettimeofday(&start, NULL);
  PERFTEST_2D_VECTOR_LOOP(iterations, kMatrixSize) mul += matrices[j];
  PERFTEST_2D_VECTOR_LOOP(iterations, kMatrixSize) mul *= matrices[j];
  PERFTEST_2D_VECTOR_LOOP(iterations, kMatrixSize) {
    goomath::Vector<T, 4> tmp =
      matrices[j] * goomath::Vector<T, 4>(matrices[i](0, 0), matrices[i](1, 0),
                                          matrices[i](2, 0), matrices[i](3, 0));
    mul -= Matrix<T, 4>::OuterProduct(tmp, tmp);
  }
  PERFTEST_2D_VECTOR_LOOP(iterations, kMatrixSize) {
    mul += (matrices[j] * getRand<T>());
  }
  for (int i = 0; i < 16; ++i) final_sum += mul[i];
  // End matrix performance code
  gettimeofday(&end, NULL);
  timersub(&end, &start, &diff);
  float time_passed = diff.tv_sec + diff.tv_usec / 1000000.f;
  LOG_PRINT("Sum %f", final_sum);
  LOG_PRINT("Time %f", time_passed);
  return 0;
}
