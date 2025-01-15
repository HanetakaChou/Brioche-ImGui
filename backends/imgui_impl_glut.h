// dear imgui: Platform Backend for GLUT/FreeGLUT
// This needs to be used along with a Renderer (e.g. OpenGL2)

// !!! GLUT/FreeGLUT IS OBSOLETE PREHISTORIC SOFTWARE. Using GLUT is not recommended unless you really miss the 90's. !!!
// !!! If someone or something is teaching you GLUT today, you are being abused. Please show some resistance. !!!
// !!! Nowadays, prefer using GLFW or SDL instead!

// Implemented features:
//  [X] Platform: Partial keyboard support. Since 1.87 we are using the io.AddKeyEvent() function. Pass ImGuiKey values to all key functions e.g. ImGui::IsKeyPressed(ImGuiKey_Space). [Legacy GLUT values are obsolete since 1.87 and not supported since 1.91.5]
// Missing features or Issues:
//  [ ] Platform: GLUT is unable to distinguish e.g. Backspace from CTRL+H or TAB from CTRL+I
//  [ ] Platform: Missing horizontal mouse wheel support.
//  [ ] Platform: Missing mouse cursor shape/visibility support.
//  [ ] Platform: Missing clipboard support (not supported by Glut).
//  [ ] Platform: Missing gamepad support.

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#pragma once
#ifndef IMGUI_DISABLE
#include "../imgui.h" // IMGUI_IMPL_API

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
extern "C" IMGUI_IMPL_API bool ImGui_ImplGLUT_Init();
extern "C" IMGUI_IMPL_API void ImGui_ImplGLUT_Shutdown();
extern "C" IMGUI_IMPL_API void ImGui_ImplGLUT_NewFrame(float delta_time_in_seconds);

// We are using the same weird names as GLUT for consistency.
//------------------------------------ GLUT name ---------------------------------------------- Decent Name ---------
extern "C" IMGUI_IMPL_API void ImGui_ImplGLUT_ReshapeFunc(int w, int h);                            // ~ ResizeFunc
extern "C" IMGUI_IMPL_API void ImGui_ImplGLUT_MotionFunc(int x, int y);                             // ~ MouseMoveFunc
extern "C" IMGUI_IMPL_API void ImGui_ImplGLUT_MouseFunc(int mouse_button, bool down, int x, int y); // ~ MouseButtonFunc
extern "C" IMGUI_IMPL_API void ImGui_ImplGLUT_MouseWheelFunc(bool dir, int x, int y);               // ~ MouseWheelFunc
extern "C" IMGUI_IMPL_API void ImGui_ImplGLUT_KeyboardFunc(unsigned char c);                        // ~ CharPressedFunc
extern "C" IMGUI_IMPL_API void ImGui_ImplGLUT_KeyboardUpFunc(unsigned char c);                      // ~ CharReleasedFunc
extern "C" IMGUI_IMPL_API void ImGui_ImplGLUT_SpecialFunc(int key);                                 // ~ KeyPressedFunc
extern "C" IMGUI_IMPL_API void ImGui_ImplGLUT_SpecialUpFunc(int key);                               // ~ KeyReleasedFunc

#endif // #ifndef IMGUI_DISABLE
