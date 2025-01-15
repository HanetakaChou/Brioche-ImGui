constexpr uint32_t const imgui_fragment_shader_module_code[] = {
#ifndef NDEBUG
#include "debug/_internal_imgui_fragment.inl"
#else
#include "release/_internal_imgui_fragment.inl"
#endif
};
