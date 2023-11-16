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

#include <stddef.h>
#include <stdint.h>
#include "renderer.h"
#include "user_input_model.h"
#include "../thirdparty/McRT-Malloc/include/mcrt_allocator.h"
#include "../thirdparty/McRT-Malloc/include/mcrt_tick_count.h"
#include "../thirdparty/DXUT/Optional/DXUTcamera.h"
#include "../thirdparty/Brioche-Window-System-Integration/include/brx_wsi.h"

struct ui_controller_t
{
    bool m_show_demo_window;
    bool m_show_another_window;
};

struct user_camera_controller_t
{
    CDXUTFirstPersonCamera m_first_person_camera;
};

struct wsi_state_t
{
    bool m_running;
    renderer *m_renderer;
    int32_t m_window_width;
    int32_t m_window_height;
    float m_window_width_scale;
    float m_window_height_scale;
    double m_tick_count_resolution;
    uint64_t m_tick_count_previous_frame;
    bool m_ui_view;
    ui_controller_t m_ui_controller;
    ui_model_t m_ui_model;
    user_camera_controller_t m_user_camera_controller;
    user_camera_model_t m_user_camera_model;
};

static void *internal_imgui_malloc_wrapper(size_t size, void *user_data);
static void internal_imgui_free_wrapper(void *ptr, void *user_data);

static void internal_key_press_handler(void *handler_context, int key, bool shift_key, bool caps_key, bool ctrl_key, bool alt_key);
static void internal_key_release_handler(void *handler_context, int key, bool shift_key, bool caps_key, bool ctrl_key, bool alt_key);
static void internal_button_press_handler(void *handler_context, int button, int x, int y);
static void internal_button_release_handler(void *handler_context, int button, int x, int y);
static void internal_scroll_up_handler(void *handler_context, int x, int y);
static void internal_scroll_down_handler(void *handler_context, int x, int y);
static void internal_motion_handler(void *handler_context, int x, int y, bool left_button, bool middle_button, bool right_button);
static void internal_resize_handler(void *handler_context, int width, int height, float width_scale, float height_scale);

static void inline ui_controller_init(ui_controller_t *ui_controller);

static void inline ui_simulate(void *platform_context, ui_model_t *ui_model, ui_controller_t *ui_controller);

static void inline user_camera_controller_init(user_camera_model_t const *user_camera_model, user_camera_controller_t *user_camera_controller);

static void inline user_camera_simulate(float interval_time, user_camera_model_t *user_camera_model, user_camera_controller_t *user_camera_controller);

#if defined(__GNUC__)

#if defined(__linux__)

#include <unistd.h>
#include <errno.h>

#include "../thirdparty/Brioche-ImGui/imgui.h"
#include "../thirdparty/Brioche-ImGui/backends/imgui_impl_glut.h"

int main(int argc, char *argv[])

#elif defined(__MACH__)

#include "../thirdparty/Brioche-ImGui/imgui.h"
#include "../thirdparty/Brioche-ImGui/backends/imgui_impl_osx.h"

extern "C" void ImGui_ImplOSX_Platform_SetImeData(ImGuiContext *, ImGuiViewport *viewport, ImGuiPlatformImeData *data);

int main(int argc, char *argv[])

#else
#error Unknown Platform
#endif

#elif defined(_MSC_VER)

#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN 1
#include <sdkddkver.h>
#include <Windows.h>

#include "../thirdparty/Brioche-ImGui/imgui.h"
#include "../thirdparty/Brioche-ImGui/backends/imgui_impl_win32.h"

#ifndef NDEBUG
int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
#else
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
#endif

#else
#error Unknown Compiler
#endif
{
    // Vulkan Validation Layer
#if defined(__GNUC__)
#if defined(__linux__)
#ifndef NDEBUG
    {
        char dir_name[4096] = {};
        {
            ssize_t res_read_link = readlink("/proc/self/exe", dir_name, (sizeof(dir_name) / sizeof(dir_name[0])));
            assert(-1 != res_read_link);

            assert(res_read_link < (sizeof(dir_name) / sizeof(dir_name[0])));

            for (ssize_t index_plus_one = res_read_link; index_plus_one > 0; --index_plus_one)
            {
                ssize_t index = index_plus_one - 1;

                if ('/' != dir_name[index])
                {
                    dir_name[index] = '\0';
                }
                else
                {
                    break;
                }
            }
        }

        // We assume that the "VkLayer_khronos_validation.json" is at the same directory of the executable file
        int res_set_env_vk_layer_path = setenv("VK_LAYER_PATH", dir_name, 1);
        assert(0 == res_set_env_vk_layer_path);
    }
#endif
#elif defined(__MACH__)
    // Molten VK
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)
#ifndef NDEBUG
    {
        // We assume that the "VkLayer_khronos_validation.json" is at the same directory of the executable file
        WCHAR file_name[4096];
        DWORD const res_get_file_name = GetModuleFileNameW(NULL, file_name, sizeof(file_name) / sizeof(file_name[0]));
        assert(0U != res_get_file_name);

        for (int i = (res_get_file_name - 1); i > 0; --i)
        {
            if (L'\\' == file_name[i])
            {
                file_name[i] = L'\0';
                break;
            }
        }

        BOOL const res_set_environment_variable = SetEnvironmentVariableW(L"VK_LAYER_PATH", file_name);
        assert(FALSE != res_set_environment_variable);
    }
#endif

#else
#error Unknown Compiler
#endif

    wsi_state_t wsi_state = {
        true,
        NULL,
        1280,
        720,
        1.0F,
        1.0F,
        1.0 / static_cast<double>(mcrt_tick_count_per_second()),
        mcrt_tick_count_now(),
        true};

    brx_wsi_init_connection();

    // ImGui_ImplWin32_EnableDpiAwareness();

    brx_wsi_init_main_window(
        "Brioche-ImGui",
        internal_key_press_handler,
        internal_key_release_handler,
        internal_button_press_handler,
        internal_button_release_handler,
        internal_scroll_up_handler,
        internal_scroll_down_handler,
        internal_motion_handler,
        internal_resize_handler,
        wsi_state.m_window_width,
        wsi_state.m_window_height,
        &wsi_state);

    brx_wsi_get_main_window_scale(&wsi_state.m_window_width_scale, &wsi_state.m_window_height_scale);

    {
        IMGUI_CHECKVERSION();

        ImGui::SetAllocatorFunctions(internal_imgui_malloc_wrapper, internal_imgui_free_wrapper);

        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::StyleColorsDark();
    }

#if defined(__GNUC__)
#if defined(__linux__)
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_ReshapeFunc(wsi_state.m_window_width, wsi_state.m_window_height);
#elif defined(__MACH__)
    ImGui_ImplOSX_Init(ImGui_ImplOSX_Platform_SetImeData);
    ImGui_ImplOSX_ReshapeFunc(wsi_state.m_window_width, wsi_state.m_window_height, wsi_state.m_window_width_scale, wsi_state.m_window_height_scale);
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)
    ImGui_ImplWin32_Init(brx_wsi_get_main_window());
#else
#error Unknown Compiler
#endif

    wsi_state.m_renderer = renderer_init(brx_wsi_get_connection(), &wsi_state.m_ui_model, &wsi_state.m_user_camera_model);

    ui_controller_init(&wsi_state.m_ui_controller);

    user_camera_controller_init(&wsi_state.m_user_camera_model, &wsi_state.m_user_camera_controller);

    wsi_state.m_renderer->attach_window(brx_wsi_get_main_window(), 1.0F / wsi_state.m_window_width_scale, 1.0F / wsi_state.m_window_height_scale);

    brx_wsi_show_main_window();

    brx_wsi_run_main_loop(
        [](void *tick_context) -> bool
        {
            wsi_state_t &wsi_state = (*static_cast<wsi_state_t *>(tick_context));

            if ((wsi_state.m_window_width > 0) && (wsi_state.m_window_height > 0) && (wsi_state.m_window_width_scale > 1E-3F) && (wsi_state.m_window_height_scale > 1E-3F))
            {
                float interval_time;
                {
                    uint64_t const tick_count_current_frame = mcrt_tick_count_now();
                    interval_time = static_cast<float>(static_cast<double>(tick_count_current_frame - wsi_state.m_tick_count_previous_frame) * wsi_state.m_tick_count_resolution);
                    wsi_state.m_tick_count_previous_frame = tick_count_current_frame;
                }

                // UI
                {
#if defined(__GNUC__)
#if defined(__linux__)
                    ImGui_ImplGLUT_NewFrame(interval_time);
#elif defined(__MACH__)
                    ImGui_ImplOSX_NewFrame(interval_time);
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)
                    ImGui_ImplWin32_NewFrame();
#else
#error Unknown Compiler
#endif
                    ImGui::NewFrame();

                    ui_simulate(brx_wsi_get_main_window(), &wsi_state.m_ui_model, &wsi_state.m_ui_controller);

                    // ImGui::EndFrame();
                    ImGui::Render();
                }

                // User Camera
                {
                    user_camera_simulate(interval_time, &wsi_state.m_user_camera_model, &wsi_state.m_user_camera_controller);
                }

                // Render
                wsi_state.m_renderer->draw(interval_time, &wsi_state.m_ui_model, &wsi_state.m_user_camera_model);
            }

            return true;
        },
        &wsi_state);

    {
        wsi_state.m_renderer->detach_window();

        renderer_destroy(wsi_state.m_renderer);
    }

#if defined(__GNUC__)
#if defined(__linux__)
    ImGui_ImplGLUT_Shutdown();
#elif defined(__MACH__)
    ImGui_ImplOSX_Shutdown();
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)
    ImGui_ImplWin32_Shutdown();
#else
#error Unknown Compiler
#endif

    ImGui::DestroyContext();

    brx_wsi_uninit_main_window();

    brx_wsi_uninit_connection();

    return 0;
}

static void *internal_imgui_malloc_wrapper(size_t size, void *)
{
    return mcrt_malloc(size, 16);
}

static void internal_imgui_free_wrapper(void *ptr, void *)
{
    return mcrt_free(ptr);
}

static void internal_key_press_handler(void *handler_context, int key, bool shift_key, bool caps_key, bool ctrl_key, bool alt_key)
{
    wsi_state_t &wsi_state = (*static_cast<wsi_state_t *>(handler_context));

    D3DUtil_CameraKeys mapped_camera_key;
    switch (key)
    {
    case ImGuiKey_W:
    {
        mapped_camera_key = CAM_MOVE_FORWARD;
    }
    break;
    case ImGuiKey_S:
    {
        mapped_camera_key = CAM_MOVE_BACKWARD;
    }
    break;
    case ImGuiKey_A:
    {
        mapped_camera_key = CAM_STRAFE_LEFT;
    }
    break;
    case ImGuiKey_D:
    {
        mapped_camera_key = CAM_STRAFE_RIGHT;
    }
    break;
    case ImGuiKey_Q:
    {
        mapped_camera_key = CAM_MOVE_UP;
    }
    break;
    case ImGuiKey_E:
    {
        mapped_camera_key = CAM_MOVE_DOWN;
    }
    break;

    default:
    {
        mapped_camera_key = CAM_UNKNOWN;
    }
    };

    wsi_state.m_user_camera_controller.m_first_person_camera.HandleKeyDownMessage(mapped_camera_key);
}

static void internal_key_release_handler(void *handler_context, int key, bool shift_key, bool caps_key, bool ctrl_key, bool alt_key)
{
    wsi_state_t &wsi_state = (*static_cast<wsi_state_t *>(handler_context));

    D3DUtil_CameraKeys mapped_camera_key;
    switch (key)
    {
    case ImGuiKey_W:
    {
        mapped_camera_key = CAM_MOVE_FORWARD;
    }
    break;
    case ImGuiKey_S:
    {
        mapped_camera_key = CAM_MOVE_BACKWARD;
    }
    break;
    case ImGuiKey_A:
    {
        mapped_camera_key = CAM_STRAFE_LEFT;
    }
    break;
    case ImGuiKey_D:
    {
        mapped_camera_key = CAM_STRAFE_RIGHT;
    }
    break;
    case ImGuiKey_Q:
    {
        mapped_camera_key = CAM_MOVE_UP;
    }
    break;
    case ImGuiKey_E:
    {
        mapped_camera_key = CAM_MOVE_DOWN;
    }
    break;
    default:
    {
        mapped_camera_key = CAM_UNKNOWN;
    }
    };

    wsi_state.m_user_camera_controller.m_first_person_camera.HandleKeyUpMessage(mapped_camera_key);
}

static void internal_button_press_handler(void *handler_context, int button, int window_x, int window_y)
{
}

static void internal_button_release_handler(void *handler_context, int button, int window_x, int window_y)
{
}

static void internal_scroll_up_handler(void *handler_context, int window_x, int window_y)
{
}

static void internal_scroll_down_handler(void *handler_context, int window_x, int window_y)
{
}

static void internal_motion_handler(void *handler_context, int window_x, int window_y, bool left_button, bool middle_button, bool right_button)
{
    wsi_state_t &wsi_state = (*static_cast<wsi_state_t *>(handler_context));

    float normalized_x = static_cast<float>(static_cast<double>(window_x) / static_cast<double>(wsi_state.m_window_width));
    float normalized_y = static_cast<float>(static_cast<double>(window_y) / static_cast<double>(wsi_state.m_window_height));

    wsi_state.m_user_camera_controller.m_first_person_camera.HandleMouseMoveMessage(normalized_x, normalized_y, left_button, middle_button, right_button);
}

static void internal_resize_handler(void *handler_context, int new_width, int new_height, float new_width_scale, float new_height_scale)
{
    wsi_state_t &wsi_state = (*static_cast<wsi_state_t *>(handler_context));

    if ((wsi_state.m_window_width != new_width) || (wsi_state.m_window_height != new_height) || (std::abs(wsi_state.m_window_width_scale - new_width_scale) > 1E-3F) || (std::abs(wsi_state.m_window_height_scale - new_height_scale) > 1E-3F))
    {
        if ((new_width > 0) && (new_height > 0) && (new_width_scale > 1E-3F) && (new_height_scale > 1E-3F))
        {
            wsi_state.m_renderer->on_window_resize(1.0F / wsi_state.m_window_width_scale, 1.0F / wsi_state.m_window_height_scale);
        }
        wsi_state.m_window_width = new_width;
        wsi_state.m_window_height = new_height;
        wsi_state.m_window_width_scale = new_width_scale;
        wsi_state.m_window_height_scale = new_height_scale;
    }
}

static void inline ui_controller_init(ui_controller_t *ui_controller)
{
    ui_controller->m_show_demo_window = true;
    ui_controller->m_show_another_window = false;
}

static void inline ui_simulate(void *platform_context, ui_model_t *ui_model, ui_controller_t *ui_controller)
{
    ImGuiIO const &io = ImGui::GetIO();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (ui_controller->m_show_demo_window)
    {
        ImGui::ShowDemoWindow(&ui_controller->m_show_demo_window);
    }

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::TextUnformatted("This is some useful text.");                // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &ui_controller->m_show_demo_window); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &ui_controller->m_show_another_window);

        ImGui::SliderFloat("float", &ui_model->m_f, 0.0f, 1.0f);       // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", &ui_model->m_clear_color[0]); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            ui_model->m_counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", ui_model->m_counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (ui_controller->m_show_another_window)
    {
        ImGui::Begin("Another Window", &ui_controller->m_show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
        {
            ui_controller->m_show_another_window = false;
        }
        ImGui::End();
    }
}

static void inline user_camera_controller_init(user_camera_model_t const *user_camera_model, user_camera_controller_t *user_camera_controller)
{
    DirectX::XMFLOAT3 const eye_position(user_camera_model->m_eye_position[0], user_camera_model->m_eye_position[1], user_camera_model->m_eye_position[2]);

    DirectX::XMFLOAT3 const eye_direction(user_camera_model->m_eye_direction[0], user_camera_model->m_eye_direction[1], user_camera_model->m_eye_direction[2]);

    DirectX::XMFLOAT3 const up_direction(user_camera_model->m_up_direction[0], user_camera_model->m_up_direction[1], user_camera_model->m_up_direction[2]);

    user_camera_controller->m_first_person_camera.SetEyePt(eye_position);

    user_camera_controller->m_first_person_camera.SetEyeDir(eye_direction);

    user_camera_controller->m_first_person_camera.SetUpDir(up_direction);
}

static void inline user_camera_simulate(float interval_time, user_camera_model_t *user_camera_model, user_camera_controller_t *user_camera_controller)
{
    user_camera_controller->m_first_person_camera.FrameMove(interval_time);

    DirectX::XMFLOAT3 eye_position;
    DirectX::XMStoreFloat3(&eye_position, user_camera_controller->m_first_person_camera.GetEyePt());

    DirectX::XMFLOAT3 eye_direction;
    DirectX::XMStoreFloat3(&eye_direction, user_camera_controller->m_first_person_camera.GetEyeDir());

    DirectX::XMFLOAT3 up_direction;
    DirectX::XMStoreFloat3(&up_direction, user_camera_controller->m_first_person_camera.GetUpDir());

    user_camera_model->m_eye_position[0] = eye_position.x;
    user_camera_model->m_eye_position[1] = eye_position.y;
    user_camera_model->m_eye_position[2] = eye_position.z;

    user_camera_model->m_eye_direction[0] = eye_direction.x;
    user_camera_model->m_eye_direction[1] = eye_direction.y;
    user_camera_model->m_eye_direction[2] = eye_direction.z;

    user_camera_model->m_up_direction[0] = up_direction.x;
    user_camera_model->m_up_direction[1] = up_direction.y;
    user_camera_model->m_up_direction[2] = up_direction.z;
}
