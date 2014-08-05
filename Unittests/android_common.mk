# Copyright (c) 2013 Google, Inc.
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
# 1. The origin of this software must not be misrepresented; you must not
# claim that you wrote the original software. If you use this software
# in a product, an acknowledgment in the product documentation would be
# appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
# misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

include $(CLEAR_VARS)
namespace:=$(if $(NDK_PROJECT_PATH),,_mathfu)
LOCAL_MODULE:=$(LOCAL_TEST_NAME)${namespace}
LOCAL_MODULE_TAGS:=optional
LOCAL_SRC_FILES:=\
	$(LOCAL_PATH)/$(LOCAL_TEST_NAME).cpp
LOCAL_C_INCLUDES:=\
	$(LOCAL_PATH)/ \
	$(LOCAL_PATH)/../ \
	$(LOCAL_PATH)/../../include \
	$(LOCAL_PATH)/../../../../../../external/vectorial/include
LOCAL_LDLIBS:=-llog -landroid -lm_hard
LOCAL_STATIC_LIBRARIES:=\
	android_native_app_glue libgtest libandroidutil_static
LOCAL_CFLAGS:=-mhard-float -mfloat-abi=hard -Wno-narrowing -mfpu=neon
# Override the default log tag in the AndroidUtil library.
ANDROIDUTIL_ADDITIONAL_CFLAGS:=-DANDROID_LOG_PRINT_TAG="$(LOCAL_MODULE)"
# Redirect gtest to AndroidUtil's buffered print functions.
GTEST_ADDITIONAL_CFLAGS:=\
    -include $(LOCAL_PATH)/../AndroidUtil/AndroidLogPrint.h \
	-DGTEST_ANDROID_LOG_PRINT=AndroidLogPrint \
	-DGTEST_ANDROID_LOG_VPRINT=AndroidLogVPrint \
    -DANDROID_LOG_OVERRIDE_PRINTF=0
include $(BUILD_SHARED_LIBRARY)
# Import googletest and native_app_glue libraries.
$(call import-module,AndroidUtil/jni)
$(call import-module,googletest)
$(call import-module,android/native_app_glue)

LOCAL_TEST_NAME:=
