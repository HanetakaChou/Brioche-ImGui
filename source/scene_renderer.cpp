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

#include "scene_renderer.h"
#include <cstring>
#include <assert.h>

scene_renderer::scene_renderer()
    : m_scene_color_format(BRX_COLOR_ATTACHMENT_FORMAT_A2R10G10B10_UNORM_PACK32),
      m_scene_color_render_pass(NULL),
      m_intermediate_width(0U),
      m_intermediate_height(0U),
      m_scene_color_image(NULL),
      m_scene_color_frame_buffer(NULL)
{
}

scene_renderer::~scene_renderer()
{
    assert(NULL == this->m_scene_color_render_pass);
    assert(0U == this->m_intermediate_width);
    assert(0U == this->m_intermediate_height);
    assert(NULL == this->m_scene_color_image);
    assert(NULL == this->m_scene_color_frame_buffer);
}

void scene_renderer::init(brx_device *device, uint32_t frame_throttling_count, ui_model_t *out_ui_model, user_camera_model_t *out_user_camera_model)
{
    // Render Pass
    {
        assert(NULL == this->m_scene_color_render_pass);

        BRX_RENDER_PASS_COLOR_ATTACHMENT const color_attachment = {
            this->m_scene_color_format,
            BRX_RENDER_PASS_COLOR_ATTACHMENT_LOAD_OPERATION_CLEAR,
            BRX_RENDER_PASS_COLOR_ATTACHMENT_STORE_OPERATION_FLUSH_FOR_SAMPLED_IMAGE};

        this->m_scene_color_render_pass = device->create_render_pass(1U, &color_attachment, NULL);
    }

    // Init User Input Model
    out_ui_model->m_clear_color[0] = 0.45F;
    out_ui_model->m_clear_color[1] = 0.55F;
    out_ui_model->m_clear_color[2] = 0.60F;
    out_ui_model->m_f = 0.0F;
    out_ui_model->m_counter = 0;

    out_user_camera_model->m_eye_position[0] = 0.0F;
    out_user_camera_model->m_eye_position[1] = 0.5F;
    out_user_camera_model->m_eye_position[2] = 5.0F;
    out_user_camera_model->m_eye_direction[0] = 0.0F;
    out_user_camera_model->m_eye_direction[1] = 0.0523359776F;
    out_user_camera_model->m_eye_direction[2] = -0.998629570F;
    out_user_camera_model->m_up_direction[0] = 0.0F;
    out_user_camera_model->m_up_direction[1] = 1.0F;
    out_user_camera_model->m_up_direction[2] = 0.0F;
    out_user_camera_model->m_fov = 0.7F;
    out_user_camera_model->m_near = 0.01F;
    out_user_camera_model->m_far = 1000.0F;
}

void scene_renderer::destroy(brx_device *device)
{
    // Render Pass
    {
        assert(NULL != this->m_scene_color_render_pass);

        device->destroy_render_pass(this->m_scene_color_render_pass);

        this->m_scene_color_render_pass = NULL;
    }
}

void scene_renderer::on_swap_chain_attach(brx_device *device, uint32_t swap_chain_image_width, uint32_t swap_chain_image_height)
{
    // Intermediate Images
    {
        assert(0U == this->m_intermediate_width);
        this->m_intermediate_width = swap_chain_image_width;

        assert(0U == this->m_intermediate_height);
        this->m_intermediate_height = swap_chain_image_height;

        assert(NULL == this->m_scene_color_image);
        this->m_scene_color_image = device->create_color_attachment_image(this->m_scene_color_format, this->m_intermediate_width, this->m_intermediate_height, true);
    }

    // Frame Buffer
    {
        assert(NULL == this->m_scene_color_frame_buffer);
        this->m_scene_color_frame_buffer = device->create_frame_buffer(this->m_scene_color_render_pass, this->m_intermediate_width, this->m_intermediate_height, 1U, &this->m_scene_color_image, NULL);
    }
}

brx_sampled_image const *scene_renderer::get_scene_color_image()
{
    return this->m_scene_color_image->get_sampled_image();
}

void scene_renderer::on_swap_chain_detach(brx_device *device)
{
    // Frame Buffer
    {
        assert(NULL != this->m_scene_color_frame_buffer);

        device->destroy_frame_buffer(this->m_scene_color_frame_buffer);

        this->m_scene_color_frame_buffer = NULL;
    }

    // Intermediate Images
    {
        assert(NULL != this->m_scene_color_image);
        device->destroy_color_attachment_image(this->m_scene_color_image);
        this->m_scene_color_image = NULL;

        assert(0U != this->m_intermediate_width);
        this->m_intermediate_width = 0U;

        assert(0U != this->m_intermediate_height);
        this->m_intermediate_height = 0U;
    }
}

void scene_renderer::draw(brx_graphics_command_buffer *command_buffer, uint32_t frame_throttling_index, float interval_time, ui_model_t const *ui_model, user_camera_model_t const *user_camera_model)
{
    float const aspect = static_cast<float>(this->m_intermediate_width) / static_cast<float>(this->m_intermediate_height);

    // Scene Color Pass
    {
        command_buffer->begin_debug_utils_label("Scene Color Pass");

        float const color_clear_values[4] = {ui_model->m_clear_color[0], ui_model->m_clear_color[1], ui_model->m_clear_color[2], 1.0F};
        command_buffer->begin_render_pass(this->m_scene_color_render_pass, this->m_scene_color_frame_buffer, this->m_intermediate_width, this->m_intermediate_height, 1U, &color_clear_values, NULL, NULL);

        command_buffer->end_render_pass();

        command_buffer->end_debug_utils_label();
    }
}
