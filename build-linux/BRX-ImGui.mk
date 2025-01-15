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

HIDE := @

LOCAL_PATH := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ifeq (true, $(APP_DEBUG))
	BIN_DIR := $(LOCAL_PATH)/bin/debug
	OBJ_DIR := $(LOCAL_PATH)/obj/debug
else
	BIN_DIR := $(LOCAL_PATH)/bin/release
	OBJ_DIR := $(LOCAL_PATH)/obj/release
endif
SOURCE_DIR := $(LOCAL_PATH)/..
THIRD_PARTY_DIR := $(LOCAL_PATH)/../thirdparty

CC := clang++

C_FLAGS := 
C_FLAGS += -finput-charset=UTF-8 
C_FLAGS += -fexec-charset=UTF-8
C_FLAGS += -Wall 
C_FLAGS += -Werror=return-type
C_FLAGS += -fPIC
C_FLAGS += -pthread
ifeq (true, $(APP_DEBUG))
	C_FLAGS += -g -O0 -UNDEBUG
else
	C_FLAGS += -O2 -DNDEBUG
endif
C_FLAGS += -fvisibility=hidden
C_FLAGS += -Dbrx_shader_byte_code=spirv
C_FLAGS += -DIMGUI_API=__attribute__\(\(visibility\(\"default\"\)\)\)
C_FLAGS += -DIMGUI_IMPL_API=__attribute__\(\(visibility\(\"default\"\)\)\)
C_FLAGS += -DPAL_STDCPP_COMPAT=1
C_FLAGS += -I$(LOCAL_PATH)/../../CoreRT/src/Native/inc/unix
C_FLAGS += -I$(LOCAL_PATH)/../../DirectXMath/Inc
C_FLAGS += -I$(LOCAL_PATH)/../thirdparty/freetype/include

LD_FLAGS := 
LD_FLAGS += -pthread
LD_FLAGS += -Wl,--no-undefined
LD_FLAGS += -Wl,--enable-new-dtags 
LD_FLAGS += -Wl,-rpath,\$$ORIGIN
LD_FLAGS += -z now
LD_FLAGS += -z relro
ifneq (true, $(APP_DEBUG))
	LD_FLAGS += -s
endif

all :  \
	$(BIN_DIR)/libBRX-ImGui.so

# Link
$(BIN_DIR)/libBRX-ImGui.so: \
	$(OBJ_DIR)/BRX-ImGui-imgui.o \
	$(OBJ_DIR)/BRX-ImGui-imgui_demo.o \
	$(OBJ_DIR)/BRX-ImGui-imgui_draw.o \
	$(OBJ_DIR)/BRX-ImGui-imgui_tables.o \
	$(OBJ_DIR)/BRX-ImGui-imgui_widgets.o \
	$(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_glut.o \
	$(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_brx.o \
	$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_ttf_compressed.o \
	$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_japanese_ttf_compressed.o \
	$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_simplified_chinese_ttf_compressed.o \
	$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_traditional_chinese_ttf_compressed.o \
	$(OBJ_DIR)/BRX-ImGui-misc-freetype-imgui_freetype.o \
	$(OBJ_DIR)/libfreetype.a
	$(HIDE) mkdir -p $(BIN_DIR)
	$(HIDE) $(CC) -shared $(LD_FLAGS) \
		$(OBJ_DIR)/BRX-ImGui-imgui.o \
		$(OBJ_DIR)/BRX-ImGui-imgui_demo.o \
		$(OBJ_DIR)/BRX-ImGui-imgui_draw.o \
		$(OBJ_DIR)/BRX-ImGui-imgui_tables.o \
		$(OBJ_DIR)/BRX-ImGui-imgui_widgets.o \
		$(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_glut.o \
		$(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_brx.o \
		$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_ttf_compressed.o \
		$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_japanese_ttf_compressed.o \
		$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_simplified_chinese_ttf_compressed.o \
		$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_traditional_chinese_ttf_compressed.o \
		$(OBJ_DIR)/BRX-ImGui-misc-freetype-imgui_freetype.o \
		$(OBJ_DIR)/libfreetype.a \
		-o $(BIN_DIR)/libBRX-ImGui.so

# Compile
$(OBJ_DIR)/BRX-ImGui-imgui.o: $(SOURCE_DIR)/imgui.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/imgui.cpp -MD -MF $(OBJ_DIR)/BRX-ImGui-imgui.d -o $(OBJ_DIR)/BRX-ImGui-imgui.o

$(OBJ_DIR)/BRX-ImGui-imgui_demo.o: $(SOURCE_DIR)/imgui_demo.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/imgui_demo.cpp -MD -MF $(OBJ_DIR)/BRX-ImGui-imgui_demo.d -o $(OBJ_DIR)/BRX-ImGui-imgui_demo.o

$(OBJ_DIR)/BRX-ImGui-imgui_draw.o: $(SOURCE_DIR)/imgui_draw.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/imgui_draw.cpp -MD -MF $(OBJ_DIR)/BRX-ImGui-imgui_draw.d -o $(OBJ_DIR)/BRX-ImGui-imgui_draw.o

$(OBJ_DIR)/BRX-ImGui-imgui_tables.o: $(SOURCE_DIR)/imgui_tables.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/imgui_tables.cpp -MD -MF $(OBJ_DIR)/BRX-ImGui-imgui_tables.d -o $(OBJ_DIR)/BRX-ImGui-imgui_tables.o

$(OBJ_DIR)/BRX-ImGui-imgui_widgets.o: $(SOURCE_DIR)/imgui_widgets.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/imgui_widgets.cpp -MD -MF $(OBJ_DIR)/BRX-ImGui-imgui_widgets.d -o $(OBJ_DIR)/BRX-ImGui-imgui_widgets.o

$(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_glut.o: $(SOURCE_DIR)/backends/imgui_impl_glut.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/backends/imgui_impl_glut.cpp -MD -MF $(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_glut.d -o $(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_glut.o

$(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_brx.o: $(SOURCE_DIR)/backends/imgui_impl_brx.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/backends/imgui_impl_brx.cpp -MD -MF $(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_brx.d -o $(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_brx.o

$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_ttf_compressed.o: $(SOURCE_DIR)/misc/fonts/noto_sans_ttf_compressed.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/misc/fonts/noto_sans_ttf_compressed.cpp -MD -MF $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_ttf_compressed.d -o $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_ttf_compressed.o

$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_japanese_ttf_compressed.o: $(SOURCE_DIR)/misc/fonts/noto_sans_japanese_ttf_compressed.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/misc/fonts/noto_sans_japanese_ttf_compressed.cpp -MD -MF $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_japanese_ttf_compressed.d -o $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_japanese_ttf_compressed.o

$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_simplified_chinese_ttf_compressed.o: $(SOURCE_DIR)/misc/fonts/noto_sans_simplified_chinese_ttf_compressed.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/misc/fonts/noto_sans_simplified_chinese_ttf_compressed.cpp -MD -MF $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_simplified_chinese_ttf_compressed.d -o $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_simplified_chinese_ttf_compressed.o

$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_traditional_chinese_ttf_compressed.o: $(SOURCE_DIR)/misc/fonts/noto_sans_traditional_chinese_ttf_compressed.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/misc/fonts/noto_sans_traditional_chinese_ttf_compressed.cpp -MD -MF $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_traditional_chinese_ttf_compressed.d -o $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_traditional_chinese_ttf_compressed.o

$(OBJ_DIR)/BRX-ImGui-misc-freetype-imgui_freetype.o: $(SOURCE_DIR)/misc/freetype/imgui_freetype.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/misc/freetype/imgui_freetype.cpp -MD -MF $(OBJ_DIR)/BRX-ImGui-misc-freetype-imgui_freetype.d -o $(OBJ_DIR)/BRX-ImGui-misc-freetype-imgui_freetype.o

# Copy
ifeq (true, $(APP_DEBUG))
CONFIG_NAME := debug

$(OBJ_DIR)/libfreetype.a: $(THIRD_PARTY_DIR)/freetype/build-linux/bin/$(CONFIG_NAME)/libfreetype.a
	$(HIDE) mkdir -p $(BIN_DIR)
	$(HIDE) cp -f $(THIRD_PARTY_DIR)/freetype/build-linux/bin/$(CONFIG_NAME)/libfreetype.a $(OBJ_DIR)/libfreetype.a

else
CONFIG_NAME := release

$(OBJ_DIR)/libfreetype.a: $(THIRD_PARTY_DIR)/freetype/build-linux/bin/$(CONFIG_NAME)/libfreetype.a
	$(HIDE) mkdir -p $(BIN_DIR)
	$(HIDE) cp -f $(THIRD_PARTY_DIR)/freetype/build-linux/bin/$(CONFIG_NAME)/libfreetype.a $(OBJ_DIR)/libfreetype.a
endif

-include \
	$(OBJ_DIR)/BRX-ImGui-imgui.d \
	$(OBJ_DIR)/BRX-ImGui-imgui_demo.d \
	$(OBJ_DIR)/BRX-ImGui-imgui_draw.d \
	$(OBJ_DIR)/BRX-ImGui-imgui_tables.d \
	$(OBJ_DIR)/BRX-ImGui-imgui_widgets.d \
	$(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_glut.d \
	$(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_brx.d \
	$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_ttf_compressed.d \
	$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_japanese_ttf_compressed.d \
	$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_simplified_chinese_ttf_compressed.d \
	$(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_traditional_chinese_ttf_compressed.d \
	$(OBJ_DIR)/BRX-ImGui-misc-freetype-imgui_freetype.d \

clean:
	$(HIDE) rm -f $(BIN_DIR)/libBRX-ImGui.so
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-imgui.o
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-imgui_demo.o
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-imgui_draw.o
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-imgui_tables.o
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-imgui_widgets.o
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_glut.o
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_brx.o
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_ttf_compressed.o
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_japanese_ttf_compressed.o
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_simplified_chinese_ttf_compressed.o
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_traditional_chinese_ttf_compressed.o
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-misc-freetype-imgui_freetype.o
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-imgui.d
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-imgui_demo.d
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-imgui_draw.d
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-imgui_tables.d
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-imgui_widgets.d
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_glut.d
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-backends-imgui_impl_brx.d
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_ttf_compressed.d
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_japanese_ttf_compressed.d
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_simplified_chinese_ttf_compressed.d
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-misc-fonts-noto_sans_traditional_chinese_ttf_compressed.d
	$(HIDE) rm -f $(OBJ_DIR)/BRX-ImGui-misc-freetype-imgui_freetype.d
	$(HIDE) rm -f $(OBJ_DIR)/libfreetype.a

.PHONY : \
	all \
	clean