#
# Copyright (C) YuqiaoZhang(HanetakaChou)
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# https://developer.android.com/ndk/guides/android_mk

LOCAL_PATH := $(realpath $(call my-dir))

# Android Application Library

include $(CLEAR_VARS)

ifeq (arm, $(TARGET_ARCH))
LOCAL_ARM_MODE := arm
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := NativeActivity

LOCAL_SRC_FILES := \
	$(LOCAL_PATH)/../source/main.cpp \
	$(LOCAL_PATH)/../source/renderer.cpp \
	$(LOCAL_PATH)/../source/scene_renderer.cpp \
	$(LOCAL_PATH)/../thirdparty/DXUT/Optional/DXUTcamera.cpp

LOCAL_CFLAGS :=
LOCAL_CFLAGS += -finput-charset=UTF-8 
LOCAL_CFLAGS += -fexec-charset=UTF-8
LOCAL_CFLAGS += -Wall 
LOCAL_CFLAGS += -Werror=return-type
LOCAL_CFLAGS += -Dbrx_init_unknown_device=brx_init_vk_device
LOCAL_CFLAGS += -Dbrx_destroy_unknown_device=brx_destroy_vk_device
LOCAL_CFLAGS += -Dbrx_shader_byte_code=spirv
LOCAL_CFLAGS += -DPAL_STDCPP_COMPAT=1

LOCAL_CPPFLAGS :=

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../thirdparty/CoreRT/src/Native/inc/unix
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../thirdparty/DirectXMath/Inc

LOCAL_LDFLAGS :=
LOCAL_LDFLAGS += -Wl,--enable-new-dtags
LOCAL_LDFLAGS += -Wl,-rpath,\$$ORIGIN
LOCAL_LDFLAGS += -Wl,--version-script,$(LOCAL_PATH)/libNativeActivity.map

LOCAL_LDLIBS :=
LOCAL_LDLIBS += -landroid

LOCAL_SHARED_LIBRARIES :=
LOCAL_SHARED_LIBRARIES += BRX
LOCAL_SHARED_LIBRARIES += BRX-ImGui

include $(BUILD_SHARED_LIBRARY)

# BRX

include $(CLEAR_VARS)

LOCAL_MODULE := BRX

LOCAL_SRC_FILES := $(LOCAL_PATH)/../thirdparty/Brioche/build-android/debug_$(APP_DEBUG)/lib/$(TARGET_ARCH_ABI)/libBRX$(TARGET_SONAME_EXTENSION)

include $(PREBUILT_SHARED_LIBRARY)

# BRX-ImGui

include $(CLEAR_VARS)

LOCAL_MODULE := BRX-ImGui

LOCAL_SRC_FILES := $(LOCAL_PATH)/../thirdparty/Brioche-ImGui/build-android/debug_$(APP_DEBUG)/lib/$(TARGET_ARCH_ABI)/libBRX-ImGui$(TARGET_SONAME_EXTENSION)

include $(PREBUILT_SHARED_LIBRARY)

# VkLayer_khronos_validation

ifeq (true, $(APP_DEBUG))

include $(CLEAR_VARS)

LOCAL_MODULE := VkLayer_khronos_validation

LOCAL_SRC_FILES := $(LOCAL_PATH)/../thirdparty/Vulkan-ValidationLayers/bin/android/$(TARGET_ARCH_ABI)/libVkLayer_khronos_validation$(TARGET_SONAME_EXTENSION)

include $(PREBUILT_SHARED_LIBRARY)

endif
