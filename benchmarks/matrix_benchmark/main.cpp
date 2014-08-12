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
#include "mathfu/matrix_4x4.h"
#include "mathfu/utilities.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>

#if defined(__ANDROID__)
#include "AndroidUtil/AndroidLogPrint.h"
#include "AndroidUtil/AndroidMainWrapper.h"
#endif  // defined(__ANDROID__)

#include "benchmark_common.h"

// Number of elements to iterate over
static const size_t kMatrixSize = 1000;
// Number of iterations of each operation.
static const size_t kIterations = 50;

using mathfu::Matrix;
using mathfu::Random;

// This test creates a number of matrices and performs some mathematical
// operations on them in order to measure expected performance of matrix
// operations.
int main(int argc, char** argv) {
  typedef float T;
  (void)argc;
  (void)argv;
  // Create an array of matrices containing random values.
  std::vector<Matrix<T, 4> > matrices;
  Matrix<T, 4> mul = Matrix<T, 4>::Identity();
  for (size_t i = 0; i < kMatrixSize; i++) {
    Matrix<T, 4> mat(Random<T>(), Random<T>(), Random<T>(), Random<T>(),
                     Random<T>(), Random<T>(), Random<T>(), Random<T>(),
                     Random<T>(), Random<T>(), Random<T>(), Random<T>(),
                     Random<T>(), Random<T>(), Random<T>(), Random<T>());
    matrices.push_back(mat);
  }
  // Start matrix benchmark, running a number of loops for more accurate
  // numbers.
  printf("Running matrix benchmark...\n");
  Timer timer;
  PERFTEST_2D_VECTOR_LOOP(kIterations, kMatrixSize) mul += matrices[j];
  PERFTEST_2D_VECTOR_LOOP(kIterations, kMatrixSize) mul *= matrices[j];
  PERFTEST_2D_VECTOR_LOOP(kIterations, kMatrixSize) {
    mathfu::Vector<T, 4> tmp =
      matrices[j] * mathfu::Vector<T, 4>(matrices[i](0, 0), matrices[i](1, 0),
                                         matrices[i](2, 0), matrices[i](3, 0));
    mul -= Matrix<T, 4>::OuterProduct(tmp, tmp);
  }
  PERFTEST_2D_VECTOR_LOOP(kIterations, kMatrixSize) {
    mul += matrices[j] * Random<T>();
  }
  // End matrix performance code
  double elapsed = timer.GetElapsedSeconds();
  printf("Took %f seconds\n", elapsed);
  return 0;
}
