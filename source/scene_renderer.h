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

#ifndef _SCENE_RENDERER_H_
#define _SCENE_RENDERER_H_ 1

#include "../thirdparty/Brioche/include/brx_device.h"
#include "user_input_model.h"

class scene_renderer
{
	BRX_COLOR_ATTACHMENT_IMAGE_FORMAT const m_scene_color_format;
	brx_render_pass *m_scene_color_render_pass;

	uint32_t m_intermediate_width;
	uint32_t m_intermediate_height;
	brx_color_attachment_image *m_scene_color_image;
	brx_frame_buffer *m_scene_color_frame_buffer;

public:
	scene_renderer();

	~scene_renderer();

	void init(brx_device *device, uint32_t frame_throttling_count, ui_model_t *out_ui_model, user_camera_model_t *out_user_camera_model);

	void destroy(brx_device *device);

	void on_swap_chain_attach(brx_device *device, uint32_t swap_chain_image_width, uint32_t swap_chain_image_height);

	brx_sampled_image const *get_scene_color_image();

	void on_swap_chain_detach(brx_device *device);

	void draw(brx_graphics_command_buffer *command_buffer, uint32_t frame_throttling_index, float interval_time, ui_model_t const *ui_model, user_camera_model_t const *user_camera_model);
};

#endif
