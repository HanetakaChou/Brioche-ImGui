#define BYTE uint8_t
#ifndef NDEBUG
#include "debug/_internal_imgui_fragment.inl"
#else
#include "release/_internal_imgui_fragment.inl"
#endif
#undef BYTE