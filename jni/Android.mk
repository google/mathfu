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

LOCAL_PATH:=$(call my-dir)/..

source_directories:=include

# Conditionally include libstlport (so include path is added to CFLAGS) if
# it's not being built using the NDK build process.
define add-stlport-includes
$(eval \
  ifeq ($(NDK_PROJECT_PATH),)
  include external/stlport/libstlport.mk
  endif)
endef

# Configure common local variables to build box2d adding $(1) to the end of the
# build target's name.
define mathfu-module
$(eval \
  LOCAL_MODULE:=libmathfu$(1)
  LOCAL_MODULE_TAGS:=optional
  LOCAL_COPY_HEADERS_TO:=mathfu$(1))
endef

# Configure local variables to build box2d adding $(1) to the end of the
# build target's name.
define mathfu-build
$(eval \
  $$(call mathfu-module,$(1))
  LOCAL_SRC_FILES:=
  LOCAL_COPY_HEADERS:=\
    $(subst $(LOCAL_PATH)/,,\
      $(foreach source_dir,$(source_directories),\
        $(wildcard $(LOCAL_PATH)/$(source_dir)/*.h)))
  LOCAL_CFLAGS:=$(if $(APP_DEBUG),-DDEBUG=1,-DDEBUG=0)
  LOCAL_EXPORT_C_INCLUDES:=$(LOCAL_PATH)
  LOCAL_ARM_MODE:=arm
  $$(call add-stlport-includes))
endef

# --- libmathfu ---
# Build shared library.
include $(CLEAR_VARS)
$(call mathfu-build,)
include $(BUILD_SHARED_LIBRARY)

# --- libmathfu_static ---
# Build static library.
include $(CLEAR_VARS)
$(call mathfu-build,_static)
include $(BUILD_STATIC_LIBRARY)
