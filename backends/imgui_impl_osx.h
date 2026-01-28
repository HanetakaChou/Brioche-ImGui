// dear imgui: Platform Backend for OSX / Cocoa
// This needs to be used along with a Renderer (e.g. OpenGL2, OpenGL3, Vulkan, Metal..)
// - Not well tested. If you want a portable application, prefer using the GLFW or SDL platform Backends on Mac.
// - Requires linking with the GameController framework ("-framework GameController").

// Implemented features:
//  [X] Platform: Clipboard support is part of core Dear ImGui (no specific code in this backend).
//  [X] Platform: Mouse support. Can discriminate Mouse/Pen.
//  [X] Platform: Keyboard support. Since 1.87 we are using the io.AddKeyEvent() function. Pass ImGuiKey values to all key functions e.g. ImGui::IsKeyPressed(ImGuiKey_Space). [Legacy kVK_* values are obsolete since 1.87 and not supported since 1.91.5]
//  [X] Platform: Gamepad support.
//  [X] Platform: Mouse cursor shape and visibility (ImGuiBackendFlags_HasMouseCursors). Disable with 'io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange'.
//  [X] Platform: IME support.

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#pragma once
#include "../imgui.h" // IMGUI_IMPL_API
#ifndef IMGUI_DISABLE

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
extern "C" IMGUI_IMPL_API bool ImGui_ImplOSX_Init(void (*Platform_SetImeDataFn)(ImGuiContext *ctx, ImGuiViewport *viewport, ImGuiPlatformImeData *data));
extern "C" IMGUI_IMPL_API void ImGui_ImplOSX_Shutdown();
extern "C" IMGUI_IMPL_API void ImGui_ImplOSX_NewFrame(float delta_time_in_seconds);

extern "C" IMGUI_IMPL_API void ImGui_ImplOSX_ReshapeFunc(float width, float height, float width_scale, float height_scale);
extern "C" IMGUI_IMPL_API void ImGui_ImplOSX_HandleEvent(void *event, void *view);

#endif // #ifndef IMGUI_DISABLE
