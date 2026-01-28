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

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2023-04-17: BREAKING: Removed call to ImGui::NewFrame() from ImGui_ImplGLUT_NewFrame(). Needs to be called from the main application loop, like with every other backends.
//  2022-09-26: Inputs: Renamed ImGuiKey_ModXXX introduced in 1.87 to ImGuiMod_XXX (old names still supported).
//  2022-01-26: Inputs: replaced short-lived io.AddKeyModsEvent() (added two weeks ago) with io.AddKeyEvent() using ImGuiKey_ModXXX flags. Sorry for the confusion.
//  2022-01-17: Inputs: calling new io.AddMousePosEvent(), io.AddMouseButtonEvent(), io.AddMouseWheelEvent() API (1.87+).
//  2022-01-10: Inputs: calling new io.AddKeyEvent(), io.AddKeyModsEvent() + io.SetKeyEventNativeData() API (1.87+). Support for full ImGuiKey range.
//  2019-04-03: Misc: Renamed imgui_impl_freeglut.cpp/.h to imgui_impl_glut.cpp/.h.
//  2019-03-25: Misc: Made io.DeltaTime always above zero.
//  2018-11-30: Misc: Setting up io.BackendPlatformName so it can be displayed in the About Window.
//  2018-03-22: Added GLUT Platform binding.

#include "../imgui.h"
#ifndef IMGUI_DISABLE
#include "imgui_impl_glut.h"
#define GL_SILENCE_DEPRECATION

extern "C" bool ImGui_ImplGLUT_Init()
{
    ImGuiIO &io = ImGui::GetIO();
    IMGUI_CHECKVERSION();

#ifdef FREEGLUT
    io.BackendPlatformName = "imgui_impl_glut (freeglut)";
#else
    io.BackendPlatformName = "imgui_impl_glut";
#endif

    return true;
}

extern "C" void ImGui_ImplGLUT_Shutdown()
{
    ImGuiIO &io = ImGui::GetIO();
    io.BackendPlatformName = nullptr;
}

extern "C" void ImGui_ImplGLUT_NewFrame(float delta_time_in_seconds)
{
    // Setup time step
    ImGuiIO &io = ImGui::GetIO();
    io.DeltaTime = delta_time_in_seconds;
}

static void ImGui_ImplGLUT_UpdateKeyModifiers(bool ctrl, bool shift, bool alt)
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddKeyEvent(ImGuiMod_Ctrl, ctrl);
    io.AddKeyEvent(ImGuiMod_Shift, shift);
    io.AddKeyEvent(ImGuiMod_Alt, alt);
}

static void ImGui_ImplGLUT_AddKeyEvent(ImGuiKey key, bool down, int native_keycode)
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddKeyEvent(key, down);
    io.SetKeyEventNativeData(key, native_keycode, -1); // To support legacy indexing (<1.87 user code)
}

static char internal_imgui_key_to_character(ImGuiKey key, bool shift, bool caps)
{
    static constexpr char imgui_key_to_character_unshift_uncaps_table[ImGuiKey_NamedKey_COUNT] = {
        // digits row
        [ImGuiKey_0 - ImGuiKey_NamedKey_BEGIN] = '0',
        [ImGuiKey_1 -
            ImGuiKey_NamedKey_BEGIN] = '1',
        [ImGuiKey_2 -
            ImGuiKey_NamedKey_BEGIN] = '2',
        [ImGuiKey_3 -
            ImGuiKey_NamedKey_BEGIN] = '3',
        [ImGuiKey_4 -
            ImGuiKey_NamedKey_BEGIN] = '4',
        [ImGuiKey_5 -
            ImGuiKey_NamedKey_BEGIN] = '5',
        [ImGuiKey_6 -
            ImGuiKey_NamedKey_BEGIN] = '6',
        [ImGuiKey_7 -
            ImGuiKey_NamedKey_BEGIN] = '7',
        [ImGuiKey_8 -
            ImGuiKey_NamedKey_BEGIN] = '8',
        [ImGuiKey_9 -
            ImGuiKey_NamedKey_BEGIN] = '9',

        // letters -> lowercase
        [ImGuiKey_A -
            ImGuiKey_NamedKey_BEGIN] = 'a',
        [ImGuiKey_B -
            ImGuiKey_NamedKey_BEGIN] = 'b',
        [ImGuiKey_C -
            ImGuiKey_NamedKey_BEGIN] = 'c',
        [ImGuiKey_D -
            ImGuiKey_NamedKey_BEGIN] = 'd',
        [ImGuiKey_E -
            ImGuiKey_NamedKey_BEGIN] = 'e',
        [ImGuiKey_F -
            ImGuiKey_NamedKey_BEGIN] = 'f',
        [ImGuiKey_G -
            ImGuiKey_NamedKey_BEGIN] = 'g',
        [ImGuiKey_H -
            ImGuiKey_NamedKey_BEGIN] = 'h',
        [ImGuiKey_I -
            ImGuiKey_NamedKey_BEGIN] = 'i',
        [ImGuiKey_J -
            ImGuiKey_NamedKey_BEGIN] = 'j',
        [ImGuiKey_K -
            ImGuiKey_NamedKey_BEGIN] = 'k',
        [ImGuiKey_L -
            ImGuiKey_NamedKey_BEGIN] = 'l',
        [ImGuiKey_M -
            ImGuiKey_NamedKey_BEGIN] = 'm',
        [ImGuiKey_N -
            ImGuiKey_NamedKey_BEGIN] = 'n',
        [ImGuiKey_O -
            ImGuiKey_NamedKey_BEGIN] = 'o',
        [ImGuiKey_P -
            ImGuiKey_NamedKey_BEGIN] = 'p',
        [ImGuiKey_Q -
            ImGuiKey_NamedKey_BEGIN] = 'q',
        [ImGuiKey_R -
            ImGuiKey_NamedKey_BEGIN] = 'r',
        [ImGuiKey_S -
            ImGuiKey_NamedKey_BEGIN] = 's',
        [ImGuiKey_T -
            ImGuiKey_NamedKey_BEGIN] = 't',
        [ImGuiKey_U -
            ImGuiKey_NamedKey_BEGIN] = 'u',
        [ImGuiKey_V -
            ImGuiKey_NamedKey_BEGIN] = 'v',
        [ImGuiKey_W -
            ImGuiKey_NamedKey_BEGIN] = 'w',
        [ImGuiKey_X -
            ImGuiKey_NamedKey_BEGIN] = 'x',
        [ImGuiKey_Y -
            ImGuiKey_NamedKey_BEGIN] = 'y',
        [ImGuiKey_Z -
            ImGuiKey_NamedKey_BEGIN] = 'z',

        // punctuation (US, unshifted)
        [ImGuiKey_Minus -
            ImGuiKey_NamedKey_BEGIN] = '-',
        [ImGuiKey_Equal -
            ImGuiKey_NamedKey_BEGIN] = '=',
        [ImGuiKey_LeftBracket -
            ImGuiKey_NamedKey_BEGIN] = '[',
        [ImGuiKey_RightBracket -
            ImGuiKey_NamedKey_BEGIN] = ']',
        [ImGuiKey_Backslash -
            ImGuiKey_NamedKey_BEGIN] = '\\',
        [ImGuiKey_Semicolon -
            ImGuiKey_NamedKey_BEGIN] = ';',
        [ImGuiKey_Apostrophe -
            ImGuiKey_NamedKey_BEGIN] = '\'',
        [ImGuiKey_GraveAccent -
            ImGuiKey_NamedKey_BEGIN] = '`',
        [ImGuiKey_Comma -
            ImGuiKey_NamedKey_BEGIN] = ',',
        [ImGuiKey_Period -
            ImGuiKey_NamedKey_BEGIN] = '.',
        [ImGuiKey_Slash -
            ImGuiKey_NamedKey_BEGIN] = '/',

        // space
        [ImGuiKey_Space -
            ImGuiKey_NamedKey_BEGIN] = ' ',

        // keypad
        [ImGuiKey_Keypad0 -
            ImGuiKey_NamedKey_BEGIN] = '0',
        [ImGuiKey_Keypad1 -
            ImGuiKey_NamedKey_BEGIN] = '1',
        [ImGuiKey_Keypad2 -
            ImGuiKey_NamedKey_BEGIN] = '2',
        [ImGuiKey_Keypad3 -
            ImGuiKey_NamedKey_BEGIN] = '3',
        [ImGuiKey_Keypad4 -
            ImGuiKey_NamedKey_BEGIN] = '4',
        [ImGuiKey_Keypad5 -
            ImGuiKey_NamedKey_BEGIN] = '5',
        [ImGuiKey_Keypad6 -
            ImGuiKey_NamedKey_BEGIN] = '6',
        [ImGuiKey_Keypad7 -
            ImGuiKey_NamedKey_BEGIN] = '7',
        [ImGuiKey_Keypad8 -
            ImGuiKey_NamedKey_BEGIN] = '8',
        [ImGuiKey_Keypad9 -
            ImGuiKey_NamedKey_BEGIN] = '9',
        [ImGuiKey_KeypadDivide -
            ImGuiKey_NamedKey_BEGIN] = '/',
        [ImGuiKey_KeypadMultiply -
            ImGuiKey_NamedKey_BEGIN] = '*',
        [ImGuiKey_KeypadSubtract -
            ImGuiKey_NamedKey_BEGIN] = '-',
        [ImGuiKey_KeypadAdd -
            ImGuiKey_NamedKey_BEGIN] = '+',
        [ImGuiKey_KeypadDecimal -
            ImGuiKey_NamedKey_BEGIN] = '.',

        // control chars
        [ImGuiKey_Escape -
            ImGuiKey_NamedKey_BEGIN] = '\x1B', // ESC
        [ImGuiKey_Backspace -
            ImGuiKey_NamedKey_BEGIN] = '\b',
        [ImGuiKey_Tab -
            ImGuiKey_NamedKey_BEGIN] = '\t',
        [ImGuiKey_Enter -
            ImGuiKey_NamedKey_BEGIN] = '\n',
        [ImGuiKey_KeypadEnter -
            ImGuiKey_NamedKey_BEGIN] = '\n',
        [ImGuiKey_Delete -
            ImGuiKey_NamedKey_BEGIN] = '\x7F', // DEL
    };

    static constexpr char imgui_key_to_character_shift_uncaps_table[ImGuiKey_NamedKey_COUNT] = {
        // digits row with shift
        [ImGuiKey_1 - ImGuiKey_NamedKey_BEGIN] = '!',
        [ImGuiKey_2 -
            ImGuiKey_NamedKey_BEGIN] = '@',
        [ImGuiKey_3 -
            ImGuiKey_NamedKey_BEGIN] = '#',
        [ImGuiKey_4 -
            ImGuiKey_NamedKey_BEGIN] = '$',
        [ImGuiKey_5 -
            ImGuiKey_NamedKey_BEGIN] = '%',
        [ImGuiKey_6 -
            ImGuiKey_NamedKey_BEGIN] = '^',
        [ImGuiKey_7 -
            ImGuiKey_NamedKey_BEGIN] = '&',
        [ImGuiKey_8 -
            ImGuiKey_NamedKey_BEGIN] = '*',
        [ImGuiKey_9 -
            ImGuiKey_NamedKey_BEGIN] = '(',
        [ImGuiKey_0 -
            ImGuiKey_NamedKey_BEGIN] = ')',

        // letters -> uppercase
        [ImGuiKey_A -
            ImGuiKey_NamedKey_BEGIN] = 'A',
        [ImGuiKey_B -
            ImGuiKey_NamedKey_BEGIN] = 'B',
        [ImGuiKey_C -
            ImGuiKey_NamedKey_BEGIN] = 'C',
        [ImGuiKey_D -
            ImGuiKey_NamedKey_BEGIN] = 'D',
        [ImGuiKey_E -
            ImGuiKey_NamedKey_BEGIN] = 'E',
        [ImGuiKey_F -
            ImGuiKey_NamedKey_BEGIN] = 'F',
        [ImGuiKey_G -
            ImGuiKey_NamedKey_BEGIN] = 'G',
        [ImGuiKey_H -
            ImGuiKey_NamedKey_BEGIN] = 'H',
        [ImGuiKey_I -
            ImGuiKey_NamedKey_BEGIN] = 'I',
        [ImGuiKey_J -
            ImGuiKey_NamedKey_BEGIN] = 'J',
        [ImGuiKey_K -
            ImGuiKey_NamedKey_BEGIN] = 'K',
        [ImGuiKey_L -
            ImGuiKey_NamedKey_BEGIN] = 'L',
        [ImGuiKey_M -
            ImGuiKey_NamedKey_BEGIN] = 'M',
        [ImGuiKey_N -
            ImGuiKey_NamedKey_BEGIN] = 'N',
        [ImGuiKey_O -
            ImGuiKey_NamedKey_BEGIN] = 'O',
        [ImGuiKey_P -
            ImGuiKey_NamedKey_BEGIN] = 'P',
        [ImGuiKey_Q -
            ImGuiKey_NamedKey_BEGIN] = 'Q',
        [ImGuiKey_R -
            ImGuiKey_NamedKey_BEGIN] = 'R',
        [ImGuiKey_S -
            ImGuiKey_NamedKey_BEGIN] = 'S',
        [ImGuiKey_T -
            ImGuiKey_NamedKey_BEGIN] = 'T',
        [ImGuiKey_U -
            ImGuiKey_NamedKey_BEGIN] = 'U',
        [ImGuiKey_V -
            ImGuiKey_NamedKey_BEGIN] = 'V',
        [ImGuiKey_W -
            ImGuiKey_NamedKey_BEGIN] = 'W',
        [ImGuiKey_X -
            ImGuiKey_NamedKey_BEGIN] = 'X',
        [ImGuiKey_Y -
            ImGuiKey_NamedKey_BEGIN] = 'Y',
        [ImGuiKey_Z -
            ImGuiKey_NamedKey_BEGIN] = 'Z',

        // punctuation with shift
        [ImGuiKey_Minus -
            ImGuiKey_NamedKey_BEGIN] = '_',
        [ImGuiKey_Equal -
            ImGuiKey_NamedKey_BEGIN] = '+',
        [ImGuiKey_LeftBracket -
            ImGuiKey_NamedKey_BEGIN] = '{',
        [ImGuiKey_RightBracket -
            ImGuiKey_NamedKey_BEGIN] = '}',
        [ImGuiKey_Backslash -
            ImGuiKey_NamedKey_BEGIN] = '|',
        [ImGuiKey_Semicolon -
            ImGuiKey_NamedKey_BEGIN] = ':',
        [ImGuiKey_Apostrophe -
            ImGuiKey_NamedKey_BEGIN] = '"',
        [ImGuiKey_GraveAccent -
            ImGuiKey_NamedKey_BEGIN] = '~',
        [ImGuiKey_Comma -
            ImGuiKey_NamedKey_BEGIN] = '<',
        [ImGuiKey_Period -
            ImGuiKey_NamedKey_BEGIN] = '>',
        [ImGuiKey_Slash -
            ImGuiKey_NamedKey_BEGIN] = '?',

        // space
        [ImGuiKey_Space -
            ImGuiKey_NamedKey_BEGIN] = ' ',

        // keypad
        [ImGuiKey_Keypad0 -
            ImGuiKey_NamedKey_BEGIN] = '0',
        [ImGuiKey_Keypad1 -
            ImGuiKey_NamedKey_BEGIN] = '1',
        [ImGuiKey_Keypad2 -
            ImGuiKey_NamedKey_BEGIN] = '2',
        [ImGuiKey_Keypad3 -
            ImGuiKey_NamedKey_BEGIN] = '3',
        [ImGuiKey_Keypad4 -
            ImGuiKey_NamedKey_BEGIN] = '4',
        [ImGuiKey_Keypad5 -
            ImGuiKey_NamedKey_BEGIN] = '5',
        [ImGuiKey_Keypad6 -
            ImGuiKey_NamedKey_BEGIN] = '6',
        [ImGuiKey_Keypad7 -
            ImGuiKey_NamedKey_BEGIN] = '7',
        [ImGuiKey_Keypad8 -
            ImGuiKey_NamedKey_BEGIN] = '8',
        [ImGuiKey_Keypad9 -
            ImGuiKey_NamedKey_BEGIN] = '9',
        [ImGuiKey_KeypadDivide -
            ImGuiKey_NamedKey_BEGIN] = '/',
        [ImGuiKey_KeypadMultiply -
            ImGuiKey_NamedKey_BEGIN] = '*',
        [ImGuiKey_KeypadSubtract -
            ImGuiKey_NamedKey_BEGIN] = '-',
        [ImGuiKey_KeypadAdd -
            ImGuiKey_NamedKey_BEGIN] = '+',
        [ImGuiKey_KeypadDecimal -
            ImGuiKey_NamedKey_BEGIN] = '.',

        // control chars
        [ImGuiKey_Escape -
            ImGuiKey_NamedKey_BEGIN] = '\x1B', // ESC
        [ImGuiKey_Backspace -
            ImGuiKey_NamedKey_BEGIN] = '\b',
        [ImGuiKey_Tab -
            ImGuiKey_NamedKey_BEGIN] = '\t',
        [ImGuiKey_Enter -
            ImGuiKey_NamedKey_BEGIN] = '\n',
        [ImGuiKey_KeypadEnter -
            ImGuiKey_NamedKey_BEGIN] = '\n',
        [ImGuiKey_Delete -
            ImGuiKey_NamedKey_BEGIN] = '\x7F', // DEL
    };

    static constexpr char imgui_key_to_character_unshift_caps_table[ImGuiKey_NamedKey_COUNT] = {
        // digits row
        [ImGuiKey_0 - ImGuiKey_NamedKey_BEGIN] = '0',
        [ImGuiKey_1 -
            ImGuiKey_NamedKey_BEGIN] = '1',
        [ImGuiKey_2 -
            ImGuiKey_NamedKey_BEGIN] = '2',
        [ImGuiKey_3 -
            ImGuiKey_NamedKey_BEGIN] = '3',
        [ImGuiKey_4 -
            ImGuiKey_NamedKey_BEGIN] = '4',
        [ImGuiKey_5 -
            ImGuiKey_NamedKey_BEGIN] = '5',
        [ImGuiKey_6 -
            ImGuiKey_NamedKey_BEGIN] = '6',
        [ImGuiKey_7 -
            ImGuiKey_NamedKey_BEGIN] = '7',
        [ImGuiKey_8 -
            ImGuiKey_NamedKey_BEGIN] = '8',
        [ImGuiKey_9 -
            ImGuiKey_NamedKey_BEGIN] = '9',

        // letters -> uppercase
        [ImGuiKey_A -
            ImGuiKey_NamedKey_BEGIN] = 'A',
        [ImGuiKey_B -
            ImGuiKey_NamedKey_BEGIN] = 'B',
        [ImGuiKey_C -
            ImGuiKey_NamedKey_BEGIN] = 'C',
        [ImGuiKey_D -
            ImGuiKey_NamedKey_BEGIN] = 'D',
        [ImGuiKey_E -
            ImGuiKey_NamedKey_BEGIN] = 'E',
        [ImGuiKey_F -
            ImGuiKey_NamedKey_BEGIN] = 'F',
        [ImGuiKey_G -
            ImGuiKey_NamedKey_BEGIN] = 'G',
        [ImGuiKey_H -
            ImGuiKey_NamedKey_BEGIN] = 'H',
        [ImGuiKey_I -
            ImGuiKey_NamedKey_BEGIN] = 'I',
        [ImGuiKey_J -
            ImGuiKey_NamedKey_BEGIN] = 'J',
        [ImGuiKey_K -
            ImGuiKey_NamedKey_BEGIN] = 'K',
        [ImGuiKey_L -
            ImGuiKey_NamedKey_BEGIN] = 'L',
        [ImGuiKey_M -
            ImGuiKey_NamedKey_BEGIN] = 'M',
        [ImGuiKey_N -
            ImGuiKey_NamedKey_BEGIN] = 'N',
        [ImGuiKey_O -
            ImGuiKey_NamedKey_BEGIN] = 'O',
        [ImGuiKey_P -
            ImGuiKey_NamedKey_BEGIN] = 'P',
        [ImGuiKey_Q -
            ImGuiKey_NamedKey_BEGIN] = 'Q',
        [ImGuiKey_R -
            ImGuiKey_NamedKey_BEGIN] = 'R',
        [ImGuiKey_S -
            ImGuiKey_NamedKey_BEGIN] = 'S',
        [ImGuiKey_T -
            ImGuiKey_NamedKey_BEGIN] = 'T',
        [ImGuiKey_U -
            ImGuiKey_NamedKey_BEGIN] = 'U',
        [ImGuiKey_V -
            ImGuiKey_NamedKey_BEGIN] = 'V',
        [ImGuiKey_W -
            ImGuiKey_NamedKey_BEGIN] = 'W',
        [ImGuiKey_X -
            ImGuiKey_NamedKey_BEGIN] = 'X',
        [ImGuiKey_Y -
            ImGuiKey_NamedKey_BEGIN] = 'Y',
        [ImGuiKey_Z -
            ImGuiKey_NamedKey_BEGIN] = 'Z',

        // punctuation (US, unshifted)
        [ImGuiKey_Minus -
            ImGuiKey_NamedKey_BEGIN] = '-',
        [ImGuiKey_Equal -
            ImGuiKey_NamedKey_BEGIN] = '=',
        [ImGuiKey_LeftBracket -
            ImGuiKey_NamedKey_BEGIN] = '[',
        [ImGuiKey_RightBracket -
            ImGuiKey_NamedKey_BEGIN] = ']',
        [ImGuiKey_Backslash -
            ImGuiKey_NamedKey_BEGIN] = '\\',
        [ImGuiKey_Semicolon -
            ImGuiKey_NamedKey_BEGIN] = ';',
        [ImGuiKey_Apostrophe -
            ImGuiKey_NamedKey_BEGIN] = '\'',
        [ImGuiKey_GraveAccent -
            ImGuiKey_NamedKey_BEGIN] = '`',
        [ImGuiKey_Comma -
            ImGuiKey_NamedKey_BEGIN] = ',',
        [ImGuiKey_Period -
            ImGuiKey_NamedKey_BEGIN] = '.',
        [ImGuiKey_Slash -
            ImGuiKey_NamedKey_BEGIN] = '/',

        // space
        [ImGuiKey_Space -
            ImGuiKey_NamedKey_BEGIN] = ' ',

        // keypad
        [ImGuiKey_Keypad0 -
            ImGuiKey_NamedKey_BEGIN] = '0',
        [ImGuiKey_Keypad1 -
            ImGuiKey_NamedKey_BEGIN] = '1',
        [ImGuiKey_Keypad2 -
            ImGuiKey_NamedKey_BEGIN] = '2',
        [ImGuiKey_Keypad3 -
            ImGuiKey_NamedKey_BEGIN] = '3',
        [ImGuiKey_Keypad4 -
            ImGuiKey_NamedKey_BEGIN] = '4',
        [ImGuiKey_Keypad5 -
            ImGuiKey_NamedKey_BEGIN] = '5',
        [ImGuiKey_Keypad6 -
            ImGuiKey_NamedKey_BEGIN] = '6',
        [ImGuiKey_Keypad7 -
            ImGuiKey_NamedKey_BEGIN] = '7',
        [ImGuiKey_Keypad8 -
            ImGuiKey_NamedKey_BEGIN] = '8',
        [ImGuiKey_Keypad9 -
            ImGuiKey_NamedKey_BEGIN] = '9',
        [ImGuiKey_KeypadDivide -
            ImGuiKey_NamedKey_BEGIN] = '/',
        [ImGuiKey_KeypadMultiply -
            ImGuiKey_NamedKey_BEGIN] = '*',
        [ImGuiKey_KeypadSubtract -
            ImGuiKey_NamedKey_BEGIN] = '-',
        [ImGuiKey_KeypadAdd -
            ImGuiKey_NamedKey_BEGIN] = '+',
        [ImGuiKey_KeypadDecimal -
            ImGuiKey_NamedKey_BEGIN] = '.',

        // control chars
        [ImGuiKey_Escape -
            ImGuiKey_NamedKey_BEGIN] = '\x1B', // ESC
        [ImGuiKey_Backspace -
            ImGuiKey_NamedKey_BEGIN] = '\b',
        [ImGuiKey_Tab -
            ImGuiKey_NamedKey_BEGIN] = '\t',
        [ImGuiKey_Enter -
            ImGuiKey_NamedKey_BEGIN] = '\n',
        [ImGuiKey_KeypadEnter -
            ImGuiKey_NamedKey_BEGIN] = '\n',
        [ImGuiKey_Delete -
            ImGuiKey_NamedKey_BEGIN] = '\x7F', // DEL
    };

    static constexpr char imgui_key_to_character_shift_caps_table[ImGuiKey_NamedKey_COUNT] = {
        // digits row with shift
        [ImGuiKey_1 - ImGuiKey_NamedKey_BEGIN] = '!',
        [ImGuiKey_2 -
            ImGuiKey_NamedKey_BEGIN] = '@',
        [ImGuiKey_3 -
            ImGuiKey_NamedKey_BEGIN] = '#',
        [ImGuiKey_4 -
            ImGuiKey_NamedKey_BEGIN] = '$',
        [ImGuiKey_5 -
            ImGuiKey_NamedKey_BEGIN] = '%',
        [ImGuiKey_6 -
            ImGuiKey_NamedKey_BEGIN] = '^',
        [ImGuiKey_7 -
            ImGuiKey_NamedKey_BEGIN] = '&',
        [ImGuiKey_8 -
            ImGuiKey_NamedKey_BEGIN] = '*',
        [ImGuiKey_9 -
            ImGuiKey_NamedKey_BEGIN] = '(',
        [ImGuiKey_0 -
            ImGuiKey_NamedKey_BEGIN] = ')',

        // letters -> lowercase
        [ImGuiKey_A -
            ImGuiKey_NamedKey_BEGIN] = 'a',
        [ImGuiKey_B -
            ImGuiKey_NamedKey_BEGIN] = 'b',
        [ImGuiKey_C -
            ImGuiKey_NamedKey_BEGIN] = 'c',
        [ImGuiKey_D -
            ImGuiKey_NamedKey_BEGIN] = 'd',
        [ImGuiKey_E -
            ImGuiKey_NamedKey_BEGIN] = 'e',
        [ImGuiKey_F -
            ImGuiKey_NamedKey_BEGIN] = 'f',
        [ImGuiKey_G -
            ImGuiKey_NamedKey_BEGIN] = 'g',
        [ImGuiKey_H -
            ImGuiKey_NamedKey_BEGIN] = 'h',
        [ImGuiKey_I -
            ImGuiKey_NamedKey_BEGIN] = 'i',
        [ImGuiKey_J -
            ImGuiKey_NamedKey_BEGIN] = 'j',
        [ImGuiKey_K -
            ImGuiKey_NamedKey_BEGIN] = 'k',
        [ImGuiKey_L -
            ImGuiKey_NamedKey_BEGIN] = 'l',
        [ImGuiKey_M -
            ImGuiKey_NamedKey_BEGIN] = 'm',
        [ImGuiKey_N -
            ImGuiKey_NamedKey_BEGIN] = 'n',
        [ImGuiKey_O -
            ImGuiKey_NamedKey_BEGIN] = 'o',
        [ImGuiKey_P -
            ImGuiKey_NamedKey_BEGIN] = 'p',
        [ImGuiKey_Q -
            ImGuiKey_NamedKey_BEGIN] = 'q',
        [ImGuiKey_R -
            ImGuiKey_NamedKey_BEGIN] = 'r',
        [ImGuiKey_S -
            ImGuiKey_NamedKey_BEGIN] = 's',
        [ImGuiKey_T -
            ImGuiKey_NamedKey_BEGIN] = 't',
        [ImGuiKey_U -
            ImGuiKey_NamedKey_BEGIN] = 'u',
        [ImGuiKey_V -
            ImGuiKey_NamedKey_BEGIN] = 'v',
        [ImGuiKey_W -
            ImGuiKey_NamedKey_BEGIN] = 'w',
        [ImGuiKey_X -
            ImGuiKey_NamedKey_BEGIN] = 'x',
        [ImGuiKey_Y -
            ImGuiKey_NamedKey_BEGIN] = 'y',
        [ImGuiKey_Z -
            ImGuiKey_NamedKey_BEGIN] = 'z',

        // punctuation with shift
        [ImGuiKey_Minus -
            ImGuiKey_NamedKey_BEGIN] = '_',
        [ImGuiKey_Equal -
            ImGuiKey_NamedKey_BEGIN] = '+',
        [ImGuiKey_LeftBracket -
            ImGuiKey_NamedKey_BEGIN] = '{',
        [ImGuiKey_RightBracket -
            ImGuiKey_NamedKey_BEGIN] = '}',
        [ImGuiKey_Backslash -
            ImGuiKey_NamedKey_BEGIN] = '|',
        [ImGuiKey_Semicolon -
            ImGuiKey_NamedKey_BEGIN] = ':',
        [ImGuiKey_Apostrophe -
            ImGuiKey_NamedKey_BEGIN] = '"',
        [ImGuiKey_GraveAccent -
            ImGuiKey_NamedKey_BEGIN] = '~',
        [ImGuiKey_Comma -
            ImGuiKey_NamedKey_BEGIN] = '<',
        [ImGuiKey_Period -
            ImGuiKey_NamedKey_BEGIN] = '>',
        [ImGuiKey_Slash -
            ImGuiKey_NamedKey_BEGIN] = '?',

        // space
        [ImGuiKey_Space -
            ImGuiKey_NamedKey_BEGIN] = ' ',

        // keypad
        [ImGuiKey_Keypad0 -
            ImGuiKey_NamedKey_BEGIN] = '0',
        [ImGuiKey_Keypad1 -
            ImGuiKey_NamedKey_BEGIN] = '1',
        [ImGuiKey_Keypad2 -
            ImGuiKey_NamedKey_BEGIN] = '2',
        [ImGuiKey_Keypad3 -
            ImGuiKey_NamedKey_BEGIN] = '3',
        [ImGuiKey_Keypad4 -
            ImGuiKey_NamedKey_BEGIN] = '4',
        [ImGuiKey_Keypad5 -
            ImGuiKey_NamedKey_BEGIN] = '5',
        [ImGuiKey_Keypad6 -
            ImGuiKey_NamedKey_BEGIN] = '6',
        [ImGuiKey_Keypad7 -
            ImGuiKey_NamedKey_BEGIN] = '7',
        [ImGuiKey_Keypad8 -
            ImGuiKey_NamedKey_BEGIN] = '8',
        [ImGuiKey_Keypad9 -
            ImGuiKey_NamedKey_BEGIN] = '9',
        [ImGuiKey_KeypadDivide -
            ImGuiKey_NamedKey_BEGIN] = '/',
        [ImGuiKey_KeypadMultiply -
            ImGuiKey_NamedKey_BEGIN] = '*',
        [ImGuiKey_KeypadSubtract -
            ImGuiKey_NamedKey_BEGIN] = '-',
        [ImGuiKey_KeypadAdd -
            ImGuiKey_NamedKey_BEGIN] = '+',
        [ImGuiKey_KeypadDecimal -
            ImGuiKey_NamedKey_BEGIN] = '.',

        // control chars
        [ImGuiKey_Escape -
            ImGuiKey_NamedKey_BEGIN] = '\x1B', // ESC
        [ImGuiKey_Backspace -
            ImGuiKey_NamedKey_BEGIN] = '\b',
        [ImGuiKey_Tab -
            ImGuiKey_NamedKey_BEGIN] = '\t',
        [ImGuiKey_Enter -
            ImGuiKey_NamedKey_BEGIN] = '\n',
        [ImGuiKey_KeypadEnter -
            ImGuiKey_NamedKey_BEGIN] = '\n',
        [ImGuiKey_Delete -
            ImGuiKey_NamedKey_BEGIN] = '\x7F', // DEL
    };

    if ((key >= ImGuiKey_NamedKey_BEGIN) && (key < ImGuiKey_NamedKey_END))
    {
        if (!shift)
        {
            if (!caps)
            {
                return imgui_key_to_character_unshift_uncaps_table[key - ImGuiKey_NamedKey_BEGIN];
            }
            else
            {
                return imgui_key_to_character_unshift_caps_table[key - ImGuiKey_NamedKey_BEGIN];
            }
        }
        else
        {
            if (!caps)
            {
                return imgui_key_to_character_shift_uncaps_table[key - ImGuiKey_NamedKey_BEGIN];
            }
            else
            {
                return imgui_key_to_character_shift_caps_table[key - ImGuiKey_NamedKey_BEGIN];
            }
        }
    }
    else
    {
        return '\0';
    }
}

extern "C" void ImGui_ImplGLUT_KeyboardFunc(int key, bool ctrl, bool shift, bool alt, bool caps)
{
    ImGuiKey const imgui_key = static_cast<ImGuiKey>(key);

    char const c = internal_imgui_key_to_character(imgui_key, shift, caps);

    if ('\0' != c)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.AddInputCharacter((unsigned int)c);
    }

    ImGui_ImplGLUT_AddKeyEvent(imgui_key, true, key);
    ImGui_ImplGLUT_UpdateKeyModifiers(ctrl, shift, alt);
}

extern "C" void ImGui_ImplGLUT_KeyboardUpFunc(int key, bool ctrl, bool shift, bool alt, bool caps)
{
    ImGuiKey imgui_key = static_cast<ImGuiKey>(key);
    ImGui_ImplGLUT_AddKeyEvent(imgui_key, false, key);
    ImGui_ImplGLUT_UpdateKeyModifiers(ctrl, shift, alt);
}

extern "C" void ImGui_ImplGLUT_MouseFunc(int button, bool down, int x, int y)
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddMousePosEvent((float)x, (float)y);

    ImGuiMouseButton imgui_button = static_cast<ImGuiMouseButton>(button);
    assert(ImGuiMouseButton_Left == imgui_button || ImGuiMouseButton_Right == imgui_button || ImGuiMouseButton_Middle == imgui_button);

    io.AddMouseButtonEvent(imgui_button, down);
}

extern "C" void ImGui_ImplGLUT_MouseWheelFunc(bool dir, int x, int y)
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddMousePosEvent((float)x, (float)y);

    // true: UP
    // false: DOWN
    io.AddMouseWheelEvent(0.0f, dir ? 1.0f : -1.0f);
}

extern "C" void ImGui_ImplGLUT_ReshapeFunc(int w, int h)
{
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)w, (float)h);
}

extern "C" void ImGui_ImplGLUT_MotionFunc(int x, int y)
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddMousePosEvent((float)x, (float)y);
}

//-----------------------------------------------------------------------------

#endif // #ifndef IMGUI_DISABLE
