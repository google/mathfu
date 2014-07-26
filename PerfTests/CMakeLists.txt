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

include_directories(${CMAKE_SOURCE_DIR}/PerfTests)

# Generates a rule to build a performance test executable.
function(perf_executable test_name)
  set(target Perf${test_name})
  add_executable(${target} ${test_name}/main.cpp)
  if(UNIX AND NOT APPLE)
    target_link_libraries(${target} rt)
  endif(UNIX AND NOT APPLE)
endfunction()

perf_executable(MatrixTest)
perf_executable(VectorTest)
