# Copyright 2014 Google Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/benchmarks)

# Generate a rule to build a performance test executable
# ${benchmark_executable_name} from ${benchmark_source} using compile flags
# ${cxx_flags}
function(benchmark_executable benchmark_executable_name benchmark_source
         cxx_flags)
  add_executable(${benchmark_executable_name} ${benchmark_source})
  set_target_properties(${benchmark_executable_name}
    PROPERTIES COMPILE_FLAGS "${cxx_flags}")
  if(UNIX AND NOT APPLE)
    target_link_libraries(${benchmark_executable_name} rt)
  endif(UNIX AND NOT APPLE)
endfunction()

# Generates a rule to build performance test executables.  This only builds
# ${benchmark_name}_benchmarks if SIMD is disabled (see ${mathfu_enable_simd})
# or ${benchmark_name}_benchmarks and ${benchmark_name}_no_simd_benchmarks if
# SIMD is enabled where the no_simd_benchmarks binary has SIMD disabled.
function(benchmark_executables benchmark_name benchmark_source)
  benchmark_executable(${benchmark_name}_benchmarks ${benchmark_source}
    "${CMAKE_CXX_FLAGS}")
  if(mathfu_enable_simd)
    benchmark_executable(${benchmark_name}_no_simd_benchmarks
      ${benchmark_source} "${CMAKE_CXX_FLAGS} ${mathfu_disable_simd_defines}")
  endif()
endfunction()

benchmark_executables(matrix matrix_benchmark/main.cpp)
benchmark_executables(vector vector_benchmark/main.cpp)
