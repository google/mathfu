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
include $(CLEAR_VARS)

# Configure the locations of MathFu's dependencies.
MATHFU_DIR:=$(abspath $(LOCAL_PATH)/../../)
include $(MATHFU_DIR)/android_config.mk

namespace:=$(if $(NDK_PROJECT_PATH),,_mathfu)
LOCAL_MODULE:=$(LOCAL_TEST_NAME)${namespace}
LOCAL_MODULE_TAGS:=optional
LOCAL_SRC_FILES:=\
	$(LOCAL_PATH)/$(LOCAL_TEST_NAME).cpp
LOCAL_C_INCLUDES:=\
	$(LOCAL_PATH)/ \
	$(LOCAL_PATH)/../ \
	$(MATHFU_DIR)/include \
	$(DEPENDENCIES_VECTORIAL_DIR)/include
LOCAL_LDLIBS:=-llog -landroid -lm_hard
LOCAL_WHOLE_STATIC_LIBRARIES:=\
	libfplutil_main \
	libfplutil_print
LOCAL_STATIC_LIBRARIES:=\
	android_native_app_glue \
	libgtest
LOCAL_CFLAGS:=-mhard-float -mfloat-abi=hard -Wno-narrowing -mfpu=neon
include $(BUILD_SHARED_LIBRARY)

$(call import-add-path,$(abspath $(DEPENDENCIES_FPLUTIL_DIR)))
$(call import-add-path,$(abspath $(DEPENDENCIES_GTEST_DIR)/..))

# Import googletest and native_app_glue libraries.
$(call import-module,libfplutil/jni)
$(call import-module,googletest)
$(call import-module,android/native_app_glue)

LOCAL_TEST_NAME:=
