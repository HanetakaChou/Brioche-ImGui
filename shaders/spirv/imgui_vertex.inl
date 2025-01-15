constexpr uint32_t const imgui_vertex_shader_module_code[] = {
#ifndef NDEBUG
#include "debug/_internal_imgui_vertex.inl"
#else
#include "release/_internal_imgui_vertex.inl"
#endif
};
