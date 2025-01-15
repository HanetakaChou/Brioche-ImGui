//
// Copyright (C) YuqiaoZhang(HanetakaChou)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#ifndef _IMGUI_IMPL_BRX_H_
#define _IMGUI_IMPL_BRX_H_ 1

#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API
#endif

#include "../../Brioche/include/brx_device.h"

extern "C" IMGUI_IMPL_API void ImGui_ImplBrx_Init(brx_device *device, uint32_t frame_throttling_count);

extern "C" IMGUI_IMPL_API void ImGui_ImplBrx_Init_Pipeline(brx_device *device, brx_render_pass *imgui_render_pass);

extern "C" IMGUI_IMPL_API void ImGui_ImplBrx_RenderDrawData(brx_graphics_command_buffer *graphics_command_buffer, uint32_t frame_throttling_index);

extern "C" IMGUI_IMPL_API void ImGui_ImplBrx_Shutdown_Pipeline(brx_device *device);

extern "C" IMGUI_IMPL_API void ImGui_ImplBrx_Shutdown(brx_device *device);

#endif