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

# Set some variables describing file locations.
#
# NOTE: This configuration assumes that gtest was built installed in the "inst"
# directory. Please see http://blah for instructions.
#
# Also keep in mind that this will need to be cleaned up were we ever to make
# this code public domain, we will wish to remove this relative path and
# replace it with something less google-specific.
set(GUNIT_INCDIR ${CMAKE_CURRENT_SOURCE_DIR}/../../googletest/include)
set(GTEST_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/../../googletest)

# Include helper functions and macros used by Google Test.
include(${GTEST_LIBDIR}/cmake/internal_utils.cmake)
config_compiler_and_linker()
string(REPLACE "-W4" "-W3" cxx_default "${cxx_default}")
string(REPLACE "-Wshadow" "" cxx_default "${cxx_default}")
string(REPLACE "-Wextra" "" cxx_default "${cxx_default}")

# This is the directory into which the executables are built.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${GUNIT_INCDIR} ${CMAKE_CURRENT_SOURCE_DIR}
                                    ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Common libraries for tests.
if(NOT MSVC)
  find_package(Threads)
endif()
set(COMMON_LIBS "gtest;${CMAKE_THREAD_LIBS_INIT}")

# PUT ADDITIONAL UNIT TEST BINARIES BELOW!

cxx_executable_with_flags(VectorTests "${cxx_default}" "${COMMON_LIBS}"
    Vectors/VectorTests.cpp ${SUPPORT_SRCS})
cxx_executable_with_flags(QuaternionTests "${cxx_default}" "${COMMON_LIBS}"
    Quaternions/QuaternionTests.cpp ${SUPPORT_SRCS})
cxx_executable_with_flags(MatrixTests "${cxx_default}" "${COMMON_LIBS}"
    Matrices/MatrixTests.cpp ${SUPPORT_SRCS})
