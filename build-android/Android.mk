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

LOCAL_PATH := $(call my-dir)

# BRX-ImGui

include $(CLEAR_VARS)

ifeq (armeabi-v7a, $(TARGET_ARCH_ABI))
LOCAL_ARM_MODE := arm
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := BRX-ImGui

LOCAL_SRC_FILES := \
	$(LOCAL_PATH)/../imgui.cpp \
	$(LOCAL_PATH)/../imgui_demo.cpp \
	$(LOCAL_PATH)/../imgui_draw.cpp \
	$(LOCAL_PATH)/../imgui_tables.cpp \
	$(LOCAL_PATH)/../imgui_widgets.cpp \
	$(LOCAL_PATH)/../backends/imgui_impl_android.cpp \
	$(LOCAL_PATH)/../backends/imgui_impl_brx.cpp \
	$(LOCAL_PATH)/../misc/fonts/noto_sans_ttf_compressed.cpp \
	$(LOCAL_PATH)/../misc/fonts/noto_sans_japanese_ttf_compressed.cpp \
	$(LOCAL_PATH)/../misc/fonts/noto_sans_simplified_chinese_ttf_compressed.cpp \
	$(LOCAL_PATH)/../misc/fonts/noto_sans_traditional_chinese_ttf_compressed.cpp \
	$(LOCAL_PATH)/../misc/freetype/imgui_freetype.cpp

LOCAL_CFLAGS :=
LOCAL_CFLAGS += -finput-charset=UTF-8 
LOCAL_CFLAGS += -fexec-charset=UTF-8
LOCAL_CFLAGS += -Wall
LOCAL_CFLAGS += -Werror=return-type
LOCAL_CFLAGS += -fvisibility=hidden
LOCAL_CFLAGS += -Dbrx_shader_byte_code=spirv
LOCAL_CFLAGS += -DIMGUI_API=__attribute__\(\(visibility\(\"default\"\)\)\)
LOCAL_CFLAGS += -DIMGUI_IMPL_API=__attribute__\(\(visibility\(\"default\"\)\)\)
LOCAL_CFLAGS += -DPAL_STDCPP_COMPAT=1

LOCAL_CPPFLAGS :=

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../CoreRT/src/Native/inc/unix
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../DirectXMath/Inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../thirdparty/freetype/include

LOCAL_LDFLAGS :=
LOCAL_LDFLAGS += -Wl,--enable-new-dtags
LOCAL_LDFLAGS += -Wl,-rpath,\$$ORIGIN

LOCAL_LDLIBS :=
LOCAL_LDLIBS := -landroid

LOCAL_STATIC_LIBRARIES :=
LOCAL_STATIC_LIBRARIES += freetype

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := freetype

LOCAL_SRC_FILES := $(LOCAL_PATH)/../thirdparty/freetype/build-android/debug_$(APP_DEBUG)/obj/local/$(TARGET_ARCH_ABI)/libfreetype$(TARGET_LIB_EXTENSION)

include $(PREBUILT_STATIC_LIBRARY)