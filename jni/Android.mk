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

LOCAL_PATH:=$(call my-dir)/..

source_directories:=\
	vectors \
	matrices

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
define gmath-module
$(eval \
  LOCAL_MODULE:=libgmath$(1)
  LOCAL_MODULE_TAGS:=optional
  LOCAL_COPY_HEADERS_TO:=gmath$(1))
endef

# Configure local variables to build box2d adding $(1) to the end of the
# build target's name.
define gmath-build
$(eval \
  $$(call gmath-module,$(1))
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

# --- libgmath ---
# Build shared library.
include $(CLEAR_VARS)
$(call gmath-build,)
include $(BUILD_SHARED_LIBRARY)

# --- libgmath_static ---
# Build static library.
include $(CLEAR_VARS)
$(call gmath-build,_static)
include $(BUILD_STATIC_LIBRARY)
