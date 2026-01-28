#include "noto_sans_cjk_jp_ttf.h"

static uint8_t noto_sans_cjk_jp_ttf_data[] = {
#include "bin2h/_internal_noto_sans_cjk_jp_ttf.inl"
};

extern "C" void *brx_imgui_font_asset_get_noto_sans_cjk_jp_ttf_data_base()
{
	return noto_sans_cjk_jp_ttf_data;
}

extern "C" uint32_t brx_imgui_font_asset_get_noto_sans_cjk_jp_ttf_data_size()
{
	return sizeof(noto_sans_cjk_jp_ttf_data);
}
