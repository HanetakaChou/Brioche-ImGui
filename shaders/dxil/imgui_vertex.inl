#define BYTE uint8_t
#ifndef NDEBUG
#include "debug/_internal_imgui_vertex.inl"
#else
#include "release/_internal_imgui_vertex.inl"
#endif
#undef BYTE