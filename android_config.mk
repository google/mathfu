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

# Location of the vectorial library.
DEPENDENCIES_VECTORIAL_DIR?=$(MATHFU_DIR)/../../../../external/vectorial
# Location of the googletest library.
DEPENDENCIES_GTEST_DIR?=$(MATHFU_DIR)/../googletest
# Location of the fplutil library.
DEPENDENCIES_FPLUTIL_DIR?=$(MATHFU_DIR)/../fplutil

# Whether to disable SIMD.
MATHFU_DISABLE_SIMD?=0
# Whether to force padding of data structures with SIMD enabled.
# -1 = default, 0 = no padding, 1 = padding
MATHFU_FORCE_PADDING?=-1
