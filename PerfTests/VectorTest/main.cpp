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
#ifdef __ANDROID__
#include <android/log.h>
#endif
#include <vector>
#include <vectors/Vector_3D.h>
#include <TestUtils/TestUtils.h>

// Number of elements to iterate over
static const size_t kVectorSize = 1000;
using mathfu::Vector;

// This test creates a number of vectors and performs some mathematical
// operations on them in order to measure expected performance of vector
// operations.
int main(int argc, char** argv) {
  typedef float T;
  size_t iterations = 100;
  (void)argc;
  (void)argv;
  // Create a array of vectors
  std::vector<Vector<T, 3> > vectors;
  T final_sum = 0;
  Vector<T, 3> sum(0.0f);
  for (size_t i = 0; i < kVectorSize; i++) {
    Vector<T, 3> vec(
      (T)rand() / RAND_MAX, (T)rand() / RAND_MAX, (T)rand() / RAND_MAX);
    vectors.push_back(vec);
  }
  // Start vector performance code. Run a number of loops for more accurate
  // numbers.
  Timer timer;
  PERFTEST_2D_VECTOR_LOOP(iterations, kVectorSize) sum += vectors[j];
  PERFTEST_2D_VECTOR_LOOP(iterations, kVectorSize) sum -= vectors[j];
  PERFTEST_2D_VECTOR_LOOP(iterations, kVectorSize) sum *= 0.1f;
  PERFTEST_2D_VECTOR_LOOP(iterations, kVectorSize) {
    sum += Vector<T, 3>::CrossProduct(vectors[i], vectors[j]);
  }
  PERFTEST_2D_VECTOR_LOOP(iterations, kVectorSize) {
    final_sum += Vector<T, 3>::DotProduct(vectors[j], vectors[i]);
  }
  PERFTEST_2D_VECTOR_LOOP(iterations, kVectorSize) {
      final_sum -= vectors[i].Length();
  }
  PERFTEST_2D_VECTOR_LOOP(iterations, kVectorSize) {
    final_sum += vectors[i].Normalize();
  }
  final_sum += sum[0] + sum[1] + sum[2];
  // End vector performance code
  double elapsed = timer.GetElapsedSeconds();
  LOG_PRINT("Sum %f\n", final_sum);
  LOG_PRINT("Time %f\n", elapsed);
  return 0;
}
