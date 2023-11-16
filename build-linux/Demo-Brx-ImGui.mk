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
SOURCE_DIR := $(LOCAL_PATH)/../source
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
C_FLAGS += -Dbrx_init_unknown_device=brx_init_vk_device
C_FLAGS += -Dbrx_destroy_unknown_device=brx_destroy_vk_device
C_FLAGS += -Dbrx_shader_byte_code=spirv
C_FLAGS += -I$(LOCAL_PATH)/../shaders/spirv
C_FLAGS += -DPAL_STDCPP_COMPAT=1
C_FLAGS += -I$(THIRD_PARTY_DIR)/CoreRT/src/Native/inc/unix
C_FLAGS += -I$(THIRD_PARTY_DIR)/DirectXMath/Inc

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
	$(BIN_DIR)/Demo-Brx-ImGui

# Link
ifeq (true, $(APP_DEBUG))
$(BIN_DIR)/Demo-Brx-ImGui: \
	$(OBJ_DIR)/Demo-Brx-ImGui-main.o \
	$(OBJ_DIR)/Demo-Brx-ImGui-renderer.o \
	$(OBJ_DIR)/Demo-Brx-ImGui-scene_renderer.o \
	$(OBJ_DIR)/Demo-Brx-ImGui-thirdparty-DXUT-Optional-DXUTcamera.o \
	$(BIN_DIR)/libBRX.so \
	$(BIN_DIR)/libBRX-ImGui.so \
	$(BIN_DIR)/libVkLayer_khronos_validation.so \
	$(BIN_DIR)/VkLayer_khronos_validation.json \
	$(LOCAL_PATH)/Demo-Brx-ImGui.map
else
$(BIN_DIR)/Demo-Brx-ImGui: \
	$(OBJ_DIR)/Demo-Brx-ImGui-main.o \
	$(OBJ_DIR)/Demo-Brx-ImGui-renderer.o \
	$(OBJ_DIR)/Demo-Brx-ImGui-scene_renderer.o \
	$(OBJ_DIR)/Demo-Brx-ImGui-thirdparty-DXUT-Optional-DXUTcamera.o \
	$(BIN_DIR)/libBRX.so \
	$(BIN_DIR)/libBRX-ImGui.so \
	$(LOCAL_PATH)/Demo-Brx-ImGui.map
endif
	$(HIDE) mkdir -p $(BIN_DIR)
	$(HIDE) $(CC) -pie $(LD_FLAGS) \
		-Wl,--version-script=$(LOCAL_PATH)/Demo-Brx-ImGui.map \
		$(OBJ_DIR)/Demo-Brx-ImGui-main.o \
		$(OBJ_DIR)/Demo-Brx-ImGui-renderer.o \
		$(OBJ_DIR)/Demo-Brx-ImGui-scene_renderer.o \
		$(OBJ_DIR)/Demo-Brx-ImGui-thirdparty-DXUT-Optional-DXUTcamera.o \
		-L$(BIN_DIR) -lBRX -lBRX-ImGui\
		-lxcb \
		-o $(BIN_DIR)/Demo-Brx-ImGui

# Compile
$(OBJ_DIR)/Demo-Brx-ImGui-main.o $(OBJ_DIR)/Demo-Brx-ImGui-main.d: $(SOURCE_DIR)/main.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/main.cpp -MD -MF $(OBJ_DIR)/Demo-Brx-ImGui-main.d -o $(OBJ_DIR)/Demo-Brx-ImGui-main.o

$(OBJ_DIR)/Demo-Brx-ImGui-renderer.o $(OBJ_DIR)/Demo-Brx-ImGui-renderer.d: $(SOURCE_DIR)/renderer.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/renderer.cpp -MD -MF $(OBJ_DIR)/Demo-Brx-ImGui-renderer.d -o $(OBJ_DIR)/Demo-Brx-ImGui-renderer.o

$(OBJ_DIR)/Demo-Brx-ImGui-scene_renderer.o $(OBJ_DIR)/Demo-Brx-ImGui-scene_renderer.d: $(SOURCE_DIR)/scene_renderer.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/scene_renderer.cpp -MD -MF $(OBJ_DIR)/Demo-Brx-ImGui-scene_renderer.d -o $(OBJ_DIR)/Demo-Brx-ImGui-scene_renderer.o

$(OBJ_DIR)/Demo-Brx-ImGui-thirdparty-DXUT-Optional-DXUTcamera.o $(OBJ_DIR)/Demo-Brx-ImGui-thirdparty-DXUT-Optional-DXUTcamera.d : $(SOURCE_DIR)/../thirdparty/DXUT/Optional/DXUTcamera.cpp
	$(HIDE) mkdir -p $(OBJ_DIR)
	$(HIDE) $(CC) -c $(C_FLAGS) $(SOURCE_DIR)/../thirdparty/DXUT/Optional/DXUTcamera.cpp -MD -MF $(OBJ_DIR)/Demo-Brx-ImGui-thirdparty-DXUT-Optional-DXUTcamera.d -o $(OBJ_DIR)/Demo-Brx-ImGui-thirdparty-DXUT-Optional-DXUTcamera.o

# Copy
ifeq (true, $(APP_DEBUG))
CONFIG_NAME := debug

$(BIN_DIR)/libBRX.so: $(THIRD_PARTY_DIR)/Brioche/build-linux/bin/$(CONFIG_NAME)/libBRX.so
	$(HIDE) mkdir -p $(BIN_DIR)
	$(HIDE) cp -f $(THIRD_PARTY_DIR)/Brioche/build-linux/bin/$(CONFIG_NAME)/libBRX.so $(BIN_DIR)/libBRX.so

$(BIN_DIR)/libBRX-ImGui.so: $(THIRD_PARTY_DIR)/Brioche-ImGui/build-linux/bin/$(CONFIG_NAME)/libBRX-ImGui.so
	$(HIDE) mkdir -p $(BIN_DIR)
	$(HIDE) cp -f $(THIRD_PARTY_DIR)/Brioche-ImGui/build-linux/bin/$(CONFIG_NAME)/libBRX-ImGui.so $(BIN_DIR)/libBRX-ImGui.so

$(BIN_DIR)/libVkLayer_khronos_validation.so: $(THIRD_PARTY_DIR)/Vulkan-ValidationLayers/bin/linux/x64/libVkLayer_khronos_validation.so
	$(HIDE) mkdir -p $(BIN_DIR)
	$(HIDE) cp -f $(THIRD_PARTY_DIR)/Vulkan-ValidationLayers/bin/linux/x64/libVkLayer_khronos_validation.so $(BIN_DIR)/libVkLayer_khronos_validation.so

$(BIN_DIR)/VkLayer_khronos_validation.json: $(THIRD_PARTY_DIR)/Vulkan-ValidationLayers/bin/linux/x64/VkLayer_khronos_validation.json
	$(HIDE) mkdir -p $(BIN_DIR)
	$(HIDE) cp -f $(THIRD_PARTY_DIR)/Vulkan-ValidationLayers/bin/linux/x64/VkLayer_khronos_validation.json $(BIN_DIR)/VkLayer_khronos_validation.json
else
CONFIG_NAME := release

$(BIN_DIR)/libBRX.so: $(THIRD_PARTY_DIR)/Brioche/build-linux/bin/$(CONFIG_NAME)/libBRX.so
	$(HIDE) mkdir -p $(BIN_DIR)
	$(HIDE) cp -f $(THIRD_PARTY_DIR)/Brioche/build-linux/bin/$(CONFIG_NAME)/libBRX.so $(BIN_DIR)/libBRX.so

$(BIN_DIR)/libBRX-ImGui.so: $(THIRD_PARTY_DIR)/Brioche-ImGui/build-linux/bin/$(CONFIG_NAME)/libBRX-ImGui.so
	$(HIDE) mkdir -p $(BIN_DIR)
	$(HIDE) cp -f $(THIRD_PARTY_DIR)/Brioche-ImGui/build-linux/bin/$(CONFIG_NAME)/libBRX-ImGui.so $(BIN_DIR)/libBRX-ImGui.so
endif

-include \
	$(OBJ_DIR)/Demo-Brx-ImGui-main.d \
	$(OBJ_DIR)/Demo-Brx-ImGui-renderer.d \
	$(OBJ_DIR)/Demo-Brx-ImGui-scene_renderer.d \
	$(OBJ_DIR)/Demo-Brx-ImGui-thirdparty-DXUT-Optional-DXUTcamera.d

clean:
	$(HIDE) rm -f $(BIN_DIR)/Demo-Brx-ImGui
	$(HIDE) rm -f $(OBJ_DIR)/Demo-Brx-ImGui-main.o
	$(HIDE) rm -f $(OBJ_DIR)/Demo-Brx-ImGui-renderer.o
	$(HIDE) rm -f $(OBJ_DIR)/Demo-Brx-ImGui-scene_renderer.o
	$(HIDE) rm -f $(OBJ_DIR)/Demo-Brx-ImGui-thirdparty-DXUT-Optional-DXUTcamera.o
	$(HIDE) rm -f $(OBJ_DIR)/Demo-Brx-ImGui-main.d
	$(HIDE) rm -f $(OBJ_DIR)/Demo-Brx-ImGui-renderer.d
	$(HIDE) rm -f $(OBJ_DIR)/Demo-Brx-ImGui-scene_renderer.d
	$(HIDE) rm -f $(OBJ_DIR)/Demo-Brx-ImGui-thirdparty-DXUT-Optional-DXUTcamera.d
	$(HIDE) rm -f $(BIN_DIR)/libBRX.so
	$(HIDE) rm -f $(BIN_DIR)/libBRX-ImGui.so
ifeq (true, $(APP_DEBUG))
	$(HIDE) rm -f $(BIN_DIR)/libVkLayer_khronos_validation.so
	$(HIDE) rm -f $(BIN_DIR)/VkLayer_khronos_validation.json
endif

.PHONY : \
	all \
	clean