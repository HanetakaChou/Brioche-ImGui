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

#include "imgui_impl_brx.h"

#include <cstring>

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include "../shaders/imgui_pipeline_resource_binding.sli"

#include "../misc/fonts/noto_sans_ttf_compressed.h"
#include "../misc/fonts/noto_sans_japanese_ttf_compressed.h"
#include "../misc/fonts/noto_sans_simplified_chinese_ttf_compressed.h"
#include "../misc/fonts/noto_sans_traditional_chinese_ttf_compressed.h"

#include "../imgui.h"

struct ImGui_ImplBrx_Data
{
	uint32_t m_uniform_upload_buffer_offset_alignment;
	brx_uniform_upload_buffer *m_imgui_pipeline_none_update_vertex_position_uniform_buffer;
	brx_uniform_upload_buffer *m_imgui_pipeline_none_update_vertex_texcoord_uniform_buffer;
	brx_uniform_upload_buffer *m_imgui_pipeline_none_update_vertex_color_uniform_buffer;
	brx_uniform_upload_buffer *m_imgui_pipeline_none_update_index_uniform_buffer;
	brx_sampler *m_sampler;
	brx_sampled_asset_image *m_font_texture;
	brx_descriptor_set *m_imgui_pipeline_none_update_descriptor_set;
	brx_pipeline_layout *m_imgui_pipeline_layout;
	brx_graphics_pipeline *m_imgui_pipeline;

	inline ImGui_ImplBrx_Data()
		: m_uniform_upload_buffer_offset_alignment(~0U),
		  m_imgui_pipeline_none_update_vertex_position_uniform_buffer(NULL),
		  m_imgui_pipeline_none_update_vertex_texcoord_uniform_buffer(NULL),
		  m_imgui_pipeline_none_update_vertex_color_uniform_buffer(NULL),
		  m_imgui_pipeline_none_update_index_uniform_buffer(NULL),
		  m_sampler(NULL),
		  m_font_texture(NULL),
		  m_imgui_pipeline_none_update_descriptor_set(NULL),
		  m_imgui_pipeline_layout(NULL),
		  m_imgui_pipeline(NULL)
	{
	}

	inline ~ImGui_ImplBrx_Data()
	{
		assert(uint32_t(~0U) == this->m_uniform_upload_buffer_offset_alignment);
		assert(NULL == this->m_imgui_pipeline_none_update_vertex_position_uniform_buffer);
		assert(NULL == this->m_imgui_pipeline_none_update_vertex_texcoord_uniform_buffer);
		assert(NULL == this->m_imgui_pipeline_none_update_vertex_color_uniform_buffer);
		assert(NULL == this->m_imgui_pipeline_none_update_index_uniform_buffer);
		assert(NULL == this->m_sampler);
		assert(NULL == this->m_font_texture);
		assert(NULL == this->m_imgui_pipeline_none_update_descriptor_set);
		assert(NULL == this->m_imgui_pipeline_layout);
		assert(NULL == this->m_imgui_pipeline);
	}
};

static inline uint32_t _internal_tbb_align_up(uint32_t value, uint32_t alignment);

extern "C" void ImGui_ImplBrx_Init(brx_device *device, uint32_t frame_throttling_count)
{
	{
		ImGuiIO &io = ImGui::GetIO();

		io.BackendRendererUserData = IM_NEW(ImGui_ImplBrx_Data){};
		io.BackendRendererName = "imgui_impl_brx";
		// We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
	}

	ImGuiIO const &io = ImGui::GetIO();

	ImGui_ImplBrx_Data *const backend_renderer = static_cast<ImGui_ImplBrx_Data *>(io.BackendRendererUserData);

	// Uniform Buffer
	{
		assert(uint32_t(~0U) == backend_renderer->m_uniform_upload_buffer_offset_alignment);
		backend_renderer->m_uniform_upload_buffer_offset_alignment = device->get_uniform_upload_buffer_offset_alignment();

		assert(NULL == backend_renderer->m_imgui_pipeline_none_update_vertex_position_uniform_buffer);
		backend_renderer->m_imgui_pipeline_none_update_vertex_position_uniform_buffer = device->create_uniform_upload_buffer(_internal_tbb_align_up(static_cast<uint32_t>(sizeof(imgui_pipeline_none_update_set_vertex_position_uniform_buffer_binding)), backend_renderer->m_uniform_upload_buffer_offset_alignment) * frame_throttling_count);

		assert(NULL == backend_renderer->m_imgui_pipeline_none_update_vertex_texcoord_uniform_buffer);
		backend_renderer->m_imgui_pipeline_none_update_vertex_texcoord_uniform_buffer = device->create_uniform_upload_buffer(_internal_tbb_align_up(static_cast<uint32_t>(sizeof(imgui_pipeline_none_update_set_vertex_texcoord_uniform_buffer_binding)), backend_renderer->m_uniform_upload_buffer_offset_alignment) * frame_throttling_count);

		assert(NULL == backend_renderer->m_imgui_pipeline_none_update_vertex_color_uniform_buffer);
		backend_renderer->m_imgui_pipeline_none_update_vertex_color_uniform_buffer = device->create_uniform_upload_buffer(_internal_tbb_align_up(static_cast<uint32_t>(sizeof(imgui_pipeline_none_update_set_vertex_color_uniform_buffer_binding)), backend_renderer->m_uniform_upload_buffer_offset_alignment) * frame_throttling_count);

		assert(NULL == backend_renderer->m_imgui_pipeline_none_update_index_uniform_buffer);
		backend_renderer->m_imgui_pipeline_none_update_index_uniform_buffer = device->create_uniform_upload_buffer(_internal_tbb_align_up(static_cast<uint32_t>(sizeof(imgui_pipeline_none_update_set_index_uniform_buffer_binding)), backend_renderer->m_uniform_upload_buffer_offset_alignment) * frame_throttling_count);
	}

	// Sampler
	{
		assert(NULL == backend_renderer->m_sampler);
		backend_renderer->m_sampler = device->create_sampler(BRX_SAMPLER_FILTER_LINEAR);
	}

	// Fonts Texture
	{
		brx_upload_command_buffer *const upload_command_buffer = device->create_upload_command_buffer();

		brx_graphics_command_buffer *const graphics_command_buffer = device->create_graphics_command_buffer();

		brx_upload_queue *const upload_queue = device->create_upload_queue();

		brx_graphics_queue *const graphics_queue = device->create_graphics_queue();

		brx_fence *const fence = device->create_fence(true);

		upload_command_buffer->begin();

		graphics_command_buffer->begin();

		brx_staging_upload_buffer *font_texture_staging_upload_buffer = NULL;
		{
			io.Fonts->AddFontFromMemoryCompressedTTF(noto_sans_ttf_compressed_data_base, noto_sans_ttf_compressed_size, 18.0F, NULL, io.Fonts->GetGlyphRangesDefault());
			ImFontConfig font_cfg{};
			font_cfg.MergeMode = true;
			io.Fonts->AddFontFromMemoryCompressedTTF(noto_sans_japanese_ttf_compressed_data_base, noto_sans_japanese_ttf_compressed_size, 18.0F, &font_cfg, io.Fonts->GetGlyphRangesJapanese());
			io.Fonts->AddFontFromMemoryCompressedTTF(noto_sans_simplified_chinese_ttf_compressed_data_base, noto_sans_simplified_chinese_ttf_compressed_size, 18.0F, &font_cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
			io.Fonts->AddFontFromMemoryCompressedTTF(noto_sans_traditional_chinese_ttf_compressed_data_base, noto_sans_traditional_chinese_ttf_compressed_size, 18.0F, &font_cfg, io.Fonts->GetGlyphRangesChineseFull());

			unsigned char *pixels;
			int width;
			int height;
			int bytes_per_pixel;
			io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);

			uint32_t const staging_upload_buffer_offset_alignment = device->get_staging_upload_buffer_offset_alignment();
			uint32_t const staging_upload_buffer_row_pitch_alignment = device->get_staging_upload_buffer_row_pitch_alignment();

			constexpr BRX_SAMPLED_ASSET_IMAGE_FORMAT const format = BRX_SAMPLED_ASSET_IMAGE_FORMAT_R8G8B8A8_UNORM;
			constexpr uint32_t const mip_levels = 1U;

			constexpr uint32_t const subresource_count = mip_levels;
			BRX_SAMPLED_ASSET_IMAGE_IMPORT_SUBRESOURCE_MEMCPY_DEST subresource_memcpy_dests[subresource_count];

			uint32_t const total_bytes = brx_sampled_asset_image_import_calculate_subresource_memcpy_dests(format, width, height, 1U, mip_levels, 1U, 0U, staging_upload_buffer_offset_alignment, staging_upload_buffer_row_pitch_alignment, subresource_count, &subresource_memcpy_dests[0]);
			font_texture_staging_upload_buffer = device->create_staging_upload_buffer(total_bytes);

			assert(NULL == backend_renderer->m_font_texture);
			backend_renderer->m_font_texture = device->create_sampled_asset_image(format, width, height, mip_levels);

			constexpr uint32_t const destination_mip_level = 0U;
			constexpr uint32_t const destination_subresource_index = brx_sampled_asset_image_import_calculate_subresource_index(destination_mip_level, 0U, 0U, mip_levels, 1U);
			assert(1U == subresource_memcpy_dests[destination_mip_level].output_slice_count);
			size_t const input_row_size = bytes_per_pixel * width;
			size_t const input_row_count = height;
			assert(input_row_size == subresource_memcpy_dests[destination_mip_level].output_row_size);
			assert(input_row_count == subresource_memcpy_dests[destination_mip_level].output_row_count);
			for (uint32_t row_index = 0U; row_index < input_row_count; ++row_index)
			{
				void *destination = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(font_texture_staging_upload_buffer->get_host_memory_range_base()) + (subresource_memcpy_dests[destination_subresource_index].staging_upload_buffer_offset + subresource_memcpy_dests[destination_subresource_index].output_row_pitch * row_index));
				void *source = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(pixels) + input_row_size * row_index);

				std::memcpy(destination, source, input_row_size);
			}

			upload_command_buffer->upload_from_staging_upload_buffer_to_sampled_asset_image(backend_renderer->m_font_texture, format, width, height, destination_mip_level, font_texture_staging_upload_buffer, subresource_memcpy_dests[destination_mip_level].staging_upload_buffer_offset, subresource_memcpy_dests[destination_mip_level].output_row_pitch, subresource_memcpy_dests[destination_mip_level].output_row_count);

			upload_command_buffer->release(0U, NULL, 1U, &backend_renderer->m_font_texture, &destination_mip_level, 0U, NULL);

			graphics_command_buffer->acquire(0U, NULL, 1U, &backend_renderer->m_font_texture, &destination_mip_level, 0U, NULL);
		}

		upload_command_buffer->end();

		graphics_command_buffer->end();

		upload_queue->submit_and_signal(upload_command_buffer);

		device->reset_fence(fence);

		graphics_queue->wait_and_submit(upload_command_buffer, graphics_command_buffer, fence);

		device->wait_for_fence(fence);

		device->destroy_staging_upload_buffer(font_texture_staging_upload_buffer);
		font_texture_staging_upload_buffer = NULL;

		device->destroy_fence(fence);

		device->destroy_upload_command_buffer(upload_command_buffer);

		device->destroy_graphics_command_buffer(graphics_command_buffer);

		device->destroy_upload_queue(upload_queue);

		device->destroy_graphics_queue(graphics_queue);

		io.Fonts->SetTexID((ImTextureID)backend_renderer->m_font_texture);
	}

	// Descriptor Layout
	brx_descriptor_set_layout *imgui_pipeline_none_update_set_layout = NULL;
	{
		BRX_DESCRIPTOR_SET_LAYOUT_BINDING const imgui_pipeline_none_update_set_layout_bindings[] = {
			{0U, BRX_DESCRIPTOR_TYPE_DYNAMIC_UNIFORM_BUFFER, 1U},
			{1U, BRX_DESCRIPTOR_TYPE_DYNAMIC_UNIFORM_BUFFER, 1U},
			{2U, BRX_DESCRIPTOR_TYPE_DYNAMIC_UNIFORM_BUFFER, 1U},
			{3U, BRX_DESCRIPTOR_TYPE_DYNAMIC_UNIFORM_BUFFER, 1U},
			{4U, BRX_DESCRIPTOR_TYPE_SAMPLER, 1U},
			{5U, BRX_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1U}};

		imgui_pipeline_none_update_set_layout = device->create_descriptor_set_layout(sizeof(imgui_pipeline_none_update_set_layout_bindings) / sizeof(imgui_pipeline_none_update_set_layout_bindings[0]), imgui_pipeline_none_update_set_layout_bindings);
	}

	// Descriptor
	{
		assert(NULL == backend_renderer->m_imgui_pipeline_none_update_descriptor_set);
		backend_renderer->m_imgui_pipeline_none_update_descriptor_set = device->create_descriptor_set(imgui_pipeline_none_update_set_layout, 0U);
		{
			constexpr uint32_t const dynamic_uniform_buffers_range = sizeof(imgui_pipeline_none_update_set_vertex_position_uniform_buffer_binding);
			device->write_descriptor_set(backend_renderer->m_imgui_pipeline_none_update_descriptor_set, 0U, BRX_DESCRIPTOR_TYPE_DYNAMIC_UNIFORM_BUFFER, 0U, 1U, &backend_renderer->m_imgui_pipeline_none_update_vertex_position_uniform_buffer, &dynamic_uniform_buffers_range, NULL, NULL, NULL, NULL, NULL, NULL);
		}
		{
			constexpr uint32_t const dynamic_uniform_buffers_range = sizeof(imgui_pipeline_none_update_set_vertex_texcoord_uniform_buffer_binding);
			device->write_descriptor_set(backend_renderer->m_imgui_pipeline_none_update_descriptor_set, 1U, BRX_DESCRIPTOR_TYPE_DYNAMIC_UNIFORM_BUFFER, 0U, 1U, &backend_renderer->m_imgui_pipeline_none_update_vertex_texcoord_uniform_buffer, &dynamic_uniform_buffers_range, NULL, NULL, NULL, NULL, NULL, NULL);
		}
		{
			constexpr uint32_t const dynamic_uniform_buffers_range = sizeof(imgui_pipeline_none_update_set_vertex_color_uniform_buffer_binding);
			device->write_descriptor_set(backend_renderer->m_imgui_pipeline_none_update_descriptor_set, 2U, BRX_DESCRIPTOR_TYPE_DYNAMIC_UNIFORM_BUFFER, 0U, 1U, &backend_renderer->m_imgui_pipeline_none_update_vertex_color_uniform_buffer, &dynamic_uniform_buffers_range, NULL, NULL, NULL, NULL, NULL, NULL);
		}
		{
			constexpr uint32_t const dynamic_uniform_buffers_range = sizeof(imgui_pipeline_none_update_set_index_uniform_buffer_binding);
			device->write_descriptor_set(backend_renderer->m_imgui_pipeline_none_update_descriptor_set, 3U, BRX_DESCRIPTOR_TYPE_DYNAMIC_UNIFORM_BUFFER, 0U, 1U, &backend_renderer->m_imgui_pipeline_none_update_index_uniform_buffer, &dynamic_uniform_buffers_range, NULL, NULL, NULL, NULL, NULL, NULL);
		}
		{
			device->write_descriptor_set(backend_renderer->m_imgui_pipeline_none_update_descriptor_set, 4U, BRX_DESCRIPTOR_TYPE_SAMPLER, 0U, 1U, NULL, NULL, NULL, NULL, NULL, NULL, &backend_renderer->m_sampler, NULL);
		}
		{
			brx_sampled_image const *const sample_image = backend_renderer->m_font_texture->get_sampled_image();
			device->write_descriptor_set(backend_renderer->m_imgui_pipeline_none_update_descriptor_set, 5U, BRX_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 0U, 1U, NULL, NULL, NULL, NULL, &sample_image, NULL, NULL, NULL);
		}
	}

	// Pipeline Layout
	{
		brx_descriptor_set_layout *const imgui_pipeline_descriptor_set_layouts[] = {
			imgui_pipeline_none_update_set_layout};

		assert(NULL == backend_renderer->m_imgui_pipeline_layout);
		backend_renderer->m_imgui_pipeline_layout = device->create_pipeline_layout(sizeof(imgui_pipeline_descriptor_set_layouts) / sizeof(imgui_pipeline_descriptor_set_layouts[0]), imgui_pipeline_descriptor_set_layouts);
	}

	device->destroy_descriptor_set_layout(imgui_pipeline_none_update_set_layout);
	imgui_pipeline_none_update_set_layout = NULL;
}

extern "C" void ImGui_ImplBrx_Init_Pipeline(brx_device *device, brx_render_pass *imgui_render_pass)
{
	ImGuiIO const &io = ImGui::GetIO();

	ImGui_ImplBrx_Data *const backend_renderer = static_cast<ImGui_ImplBrx_Data *>(io.BackendRendererUserData);

	// Pipeline
	{
		assert(NULL == backend_renderer->m_imgui_pipeline);
#define brx_shader_byte_code_stringizing(string) #string
#define brx_shader_byte_code_x_stringizing(string) brx_shader_byte_code_stringizing(string)
		// clang-format off
#include brx_shader_byte_code_x_stringizing(../shaders/brx_shader_byte_code/imgui_vertex.inl)
#include brx_shader_byte_code_x_stringizing(../shaders/brx_shader_byte_code/imgui_fragment.inl)
		// clang-format on
#undef brx_shader_byte_code_stringizing
#undef brx_shader_byte_code_x_stringizing
		backend_renderer->m_imgui_pipeline = device->create_graphics_pipeline(imgui_render_pass, backend_renderer->m_imgui_pipeline_layout, sizeof(imgui_vertex_shader_module_code), imgui_vertex_shader_module_code, sizeof(imgui_fragment_shader_module_code), imgui_fragment_shader_module_code, false, false, false, BRX_GRAPHICS_PIPELINE_DEPTH_COMPARE_OPERATION_UNKNOWN, true, BRX_GRAPHICS_PIPELINE_BLEND_OPERATION_OVER);
	}
}

extern "C" void ImGui_ImplBrx_RenderDrawData(brx_graphics_command_buffer *graphics_command_buffer, uint32_t frame_throttling_index)
{
	ImDrawData const *const draw_data = ImGui::GetDrawData();

	// Avoid rendering when minimized
	if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f)
	{
		return;
	}

	// TODO: Avoid overflow
	if (draw_data->TotalVtxCount >= IMGUI_MAX_VERTEX_COUNT || draw_data->TotalIdxCount >= IMGUI_MAX_INDEX_COUNT)
	{
		assert(0);
		return;
	}

	ImGuiIO const &io = ImGui::GetIO();

	ImGui_ImplBrx_Data const *const backend_renderer = static_cast<ImGui_ImplBrx_Data *>(io.BackendRendererUserData);

	// Upload
	{
		imgui_pipeline_none_update_set_vertex_position_uniform_buffer_binding *const imgui_pipeline_none_update_set_vertex_position_uniform_buffer_binding_destination = reinterpret_cast<imgui_pipeline_none_update_set_vertex_position_uniform_buffer_binding *>(reinterpret_cast<uintptr_t>(backend_renderer->m_imgui_pipeline_none_update_vertex_position_uniform_buffer->get_host_memory_range_base()) + _internal_tbb_align_up(static_cast<uint32_t>(sizeof(imgui_pipeline_none_update_set_vertex_position_uniform_buffer_binding)), backend_renderer->m_uniform_upload_buffer_offset_alignment) * frame_throttling_index);

		imgui_pipeline_none_update_set_vertex_texcoord_uniform_buffer_binding *const imgui_pipeline_none_update_set_vertex_texcoord_uniform_buffer_binding_destination = reinterpret_cast<imgui_pipeline_none_update_set_vertex_texcoord_uniform_buffer_binding *>(reinterpret_cast<uintptr_t>(backend_renderer->m_imgui_pipeline_none_update_vertex_texcoord_uniform_buffer->get_host_memory_range_base()) + _internal_tbb_align_up(static_cast<uint32_t>(sizeof(imgui_pipeline_none_update_set_vertex_texcoord_uniform_buffer_binding)), backend_renderer->m_uniform_upload_buffer_offset_alignment) * frame_throttling_index);

		imgui_pipeline_none_update_set_vertex_color_uniform_buffer_binding *const imgui_pipeline_none_update_set_vertex_color_uniform_buffer_binding_destination = reinterpret_cast<imgui_pipeline_none_update_set_vertex_color_uniform_buffer_binding *>(reinterpret_cast<uintptr_t>(backend_renderer->m_imgui_pipeline_none_update_vertex_color_uniform_buffer->get_host_memory_range_base()) + _internal_tbb_align_up(static_cast<uint32_t>(sizeof(imgui_pipeline_none_update_set_vertex_color_uniform_buffer_binding)), backend_renderer->m_uniform_upload_buffer_offset_alignment) * frame_throttling_index);

		imgui_pipeline_none_update_set_index_uniform_buffer_binding *const imgui_pipeline_none_update_set_index_uniform_buffer_binding_destination = reinterpret_cast<imgui_pipeline_none_update_set_index_uniform_buffer_binding *>(reinterpret_cast<uintptr_t>(backend_renderer->m_imgui_pipeline_none_update_index_uniform_buffer->get_host_memory_range_base()) + _internal_tbb_align_up(static_cast<uint32_t>(sizeof(imgui_pipeline_none_update_set_index_uniform_buffer_binding)), backend_renderer->m_uniform_upload_buffer_offset_alignment) * frame_throttling_index);

		// Vertex Buffer & Index Buffer
		{
			DirectX::PackedVector::XMSHORTN2 *const vertex_position_destination = reinterpret_cast<DirectX::PackedVector::XMSHORTN2 *>(&imgui_pipeline_none_update_set_vertex_position_uniform_buffer_binding_destination->g_vertex_position_uniform_buffer[0]);

			DirectX::PackedVector::XMUSHORTN2 *const vertex_texcoord_destination = reinterpret_cast<DirectX::PackedVector::XMUSHORTN2 *>(&imgui_pipeline_none_update_set_vertex_texcoord_uniform_buffer_binding_destination->g_vertex_texcoord_uniform_buffer[0]);

			DirectX::PackedVector::XMUBYTEN4 *const vertex_color_destination = reinterpret_cast<DirectX::PackedVector::XMUBYTEN4 *>(&imgui_pipeline_none_update_set_vertex_color_uniform_buffer_binding_destination->g_vertex_color_uniform_buffer[0]);

			uint16_t *const index_destination = reinterpret_cast<uint16_t *>(&imgui_pipeline_none_update_set_index_uniform_buffer_binding_destination->g_index_uniform_buffer[0]);

			DirectX::XMMATRIX const projection_matrix = DirectX::XMMatrixOrthographicOffCenterLH(draw_data->DisplayPos.x, draw_data->DisplayPos.x + draw_data->DisplaySize.x, draw_data->DisplayPos.y + draw_data->DisplaySize.y, draw_data->DisplayPos.y, -1.0F, 1.0F);

			int global_vertex_offset = 0;
			int global_index_offset = 0;
			for (int cmd_list_index = 0; cmd_list_index < draw_data->CmdListsCount; ++cmd_list_index)
			{
				ImDrawList const *const draw_list = draw_data->CmdLists[cmd_list_index];

				for (int vertex_index = 0u; vertex_index < draw_list->VtxBuffer.Size; ++vertex_index)
				{
					DirectX::XMFLOAT2 raw_position(draw_list->VtxBuffer.Data[vertex_index].pos.x, draw_list->VtxBuffer.Data[vertex_index].pos.y);

					DirectX::XMVECTOR ndc_position = DirectX::XMVector2TransformCoord(DirectX::XMLoadFloat2(&raw_position), projection_matrix);

					DirectX::PackedVector::XMStoreShortN2(&vertex_position_destination[global_vertex_offset + vertex_index], ndc_position);

					DirectX::XMFLOAT2 raw_texcoord(draw_list->VtxBuffer.Data[vertex_index].uv.x, draw_list->VtxBuffer.Data[vertex_index].uv.y);

					DirectX::PackedVector::XMStoreUShortN2(&vertex_texcoord_destination[global_vertex_offset + vertex_index], DirectX::XMLoadFloat2(&raw_texcoord));

					vertex_color_destination[global_vertex_offset + vertex_index].v = draw_list->VtxBuffer.Data[vertex_index].col;
				}

				int index_index = 0;

				for (int cmd_index = 0; cmd_index < draw_list->CmdBuffer.Size; ++cmd_index)
				{
					ImDrawCmd const *const draw_cmd = &draw_list->CmdBuffer[cmd_index];

					for (unsigned int elem_index = 0u; elem_index < draw_cmd->ElemCount; ++elem_index)
					{
						assert((draw_cmd->IdxOffset + elem_index) < static_cast<unsigned int>(draw_list->IdxBuffer.Size));
						index_destination[global_index_offset + index_index] = global_vertex_offset + draw_cmd->VtxOffset + draw_list->IdxBuffer.Data[draw_cmd->IdxOffset + elem_index];
						++index_index;
					}
				}

				assert(draw_list->IdxBuffer.Size == index_index);

				global_vertex_offset += draw_list->VtxBuffer.Size;
				global_index_offset += index_index;
			}
			assert(draw_data->TotalVtxCount == global_vertex_offset);
			assert(draw_data->TotalIdxCount == global_index_offset);
		}
	}

	// Render
	{
		graphics_command_buffer->set_view_port(static_cast<uint32_t>(draw_data->DisplaySize.x), static_cast<uint32_t>(draw_data->DisplaySize.y));

		graphics_command_buffer->bind_graphics_pipeline(backend_renderer->m_imgui_pipeline);

		brx_descriptor_set *const descritor_sets[] = {
			backend_renderer->m_imgui_pipeline_none_update_descriptor_set};

		uint32_t const dynamic_offsets[] = {
			_internal_tbb_align_up(static_cast<uint32_t>(sizeof(imgui_pipeline_none_update_set_vertex_position_uniform_buffer_binding)), backend_renderer->m_uniform_upload_buffer_offset_alignment) * frame_throttling_index,
			_internal_tbb_align_up(static_cast<uint32_t>(sizeof(imgui_pipeline_none_update_set_vertex_texcoord_uniform_buffer_binding)), backend_renderer->m_uniform_upload_buffer_offset_alignment) * frame_throttling_index,
			_internal_tbb_align_up(static_cast<uint32_t>(sizeof(imgui_pipeline_none_update_set_vertex_color_uniform_buffer_binding)), backend_renderer->m_uniform_upload_buffer_offset_alignment) * frame_throttling_index,
			_internal_tbb_align_up(static_cast<uint32_t>(sizeof(imgui_pipeline_none_update_set_index_uniform_buffer_binding)), backend_renderer->m_uniform_upload_buffer_offset_alignment) * frame_throttling_index};

		graphics_command_buffer->bind_graphics_descriptor_sets(backend_renderer->m_imgui_pipeline_layout, sizeof(descritor_sets) / sizeof(descritor_sets[0]), descritor_sets, sizeof(dynamic_offsets) / sizeof(dynamic_offsets[0]), dynamic_offsets);

		// ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
		// platform_io.Renderer_RenderState =

#if 0
		int global_index_offset = 0;
		for (int cmd_list_index = 0; cmd_list_index < draw_data->CmdListsCount; ++cmd_list_index)
		{
			ImDrawList const* const draw_list = draw_data->CmdLists[cmd_list_index];

			for (int cmd_index = 0; cmd_index < draw_list->CmdBuffer.Size; ++cmd_index)
			{
				ImDrawCmd const* const draw_cmd = &draw_list->CmdBuffer[cmd_index];

				assert(NULL == draw_cmd->UserCallback);
				assert(ImDrawCallback_ResetRenderState != draw_cmd->UserCallback);

				ImVec2 const clip_off = draw_data->DisplayPos;
				ImVec2 const clip_min(draw_cmd->ClipRect.x - clip_off.x, draw_cmd->ClipRect.y - clip_off.y);
				ImVec2 const clip_max(draw_cmd->ClipRect.z - clip_off.x, draw_cmd->ClipRect.w - clip_off.y);

				// This should always be true // otherwise, we do NOT need to upload the vertex/index buffer
				assert(clip_max.x > clip_min.x && clip_max.y > clip_min.y);

				graphics_command_buffer->set_scissor(static_cast<int32_t>(clip_min.x), static_cast<int32_t>(clip_min.y), static_cast<uint32_t>(clip_max.x - clip_min.x), static_cast<uint32_t>(clip_max.y - clip_min.y));

				assert(backend_renderer->m_font_texture == reinterpret_cast<brx_sampled_asset_image*>(draw_cmd->GetTexID()));
				graphics_command_buffer->draw(draw_cmd->ElemCount, 1U, (global_index_offset + draw_cmd->IdxOffset), 0U);
			}

			global_index_offset += draw_list->IdxBuffer.Size;
		}
		assert(draw_data->TotalIdxCount == global_index_offset);
#else
		// TODO: verify whether it is safe to simply ignore the scissor

		for (int cmd_list_index = 0; cmd_list_index < draw_data->CmdListsCount; ++cmd_list_index)
		{
			ImDrawList const *const draw_list = draw_data->CmdLists[cmd_list_index];

			for (int cmd_index = 0; cmd_index < draw_list->CmdBuffer.Size; ++cmd_index)
			{
				ImDrawCmd const *const draw_cmd = &draw_list->CmdBuffer[cmd_index];

				assert(NULL == draw_cmd->UserCallback);
				assert(ImDrawCallback_ResetRenderState != draw_cmd->UserCallback);

				assert(backend_renderer->m_font_texture == reinterpret_cast<brx_sampled_asset_image *>(draw_cmd->GetTexID()));
			}
		}

		graphics_command_buffer->set_scissor(0, 0, static_cast<uint32_t>(draw_data->DisplaySize.x), static_cast<uint32_t>(draw_data->DisplaySize.y));

		graphics_command_buffer->draw(draw_data->TotalIdxCount, 1U, 0U, 0U);
#endif

		// platform_io.Renderer_RenderState = NULL;
	}
}

extern "C" void ImGui_ImplBrx_Shutdown_Pipeline(brx_device *device)
{
	ImGuiIO const &io = ImGui::GetIO();

	ImGui_ImplBrx_Data *const backend_renderer = static_cast<ImGui_ImplBrx_Data *>(io.BackendRendererUserData);

	// Pipeline
	{
		assert(NULL != backend_renderer->m_imgui_pipeline);
		device->destroy_graphics_pipeline(backend_renderer->m_imgui_pipeline);
		backend_renderer->m_imgui_pipeline = NULL;
	}
}

extern "C" void ImGui_ImplBrx_Shutdown(brx_device *device)
{
	ImGuiIO &io = ImGui::GetIO();

	ImGui_ImplBrx_Data *const backend_renderer = static_cast<ImGui_ImplBrx_Data *>(io.BackendRendererUserData);

	// Pipeline Layout
	{
		assert(NULL != backend_renderer->m_imgui_pipeline_layout);
		device->destroy_pipeline_layout(backend_renderer->m_imgui_pipeline_layout);
		backend_renderer->m_imgui_pipeline_layout = NULL;
	}

	// Descriptor
	{
		assert(NULL != backend_renderer->m_imgui_pipeline_none_update_descriptor_set);
		device->destroy_descriptor_set(backend_renderer->m_imgui_pipeline_none_update_descriptor_set);
		backend_renderer->m_imgui_pipeline_none_update_descriptor_set = NULL;
	}

	// Fonts Texture
	{
		assert(NULL != backend_renderer->m_font_texture);
		device->destroy_sampled_asset_image(backend_renderer->m_font_texture);
		backend_renderer->m_font_texture = NULL;
	}

	// Sampler
	{
		assert(NULL != backend_renderer->m_sampler);
		device->destroy_sampler(backend_renderer->m_sampler);
		backend_renderer->m_sampler = NULL;
	}

	// Uniform Buffer
	{
		assert(NULL != backend_renderer->m_imgui_pipeline_none_update_vertex_position_uniform_buffer);
		device->destroy_uniform_upload_buffer(backend_renderer->m_imgui_pipeline_none_update_vertex_position_uniform_buffer);
		backend_renderer->m_imgui_pipeline_none_update_vertex_position_uniform_buffer = NULL;

		assert(NULL != backend_renderer->m_imgui_pipeline_none_update_vertex_texcoord_uniform_buffer);
		device->destroy_uniform_upload_buffer(backend_renderer->m_imgui_pipeline_none_update_vertex_texcoord_uniform_buffer);
		backend_renderer->m_imgui_pipeline_none_update_vertex_texcoord_uniform_buffer = NULL;

		assert(NULL != backend_renderer->m_imgui_pipeline_none_update_vertex_color_uniform_buffer);
		device->destroy_uniform_upload_buffer(backend_renderer->m_imgui_pipeline_none_update_vertex_color_uniform_buffer);
		backend_renderer->m_imgui_pipeline_none_update_vertex_color_uniform_buffer = NULL;

		assert(NULL != backend_renderer->m_imgui_pipeline_none_update_index_uniform_buffer);
		device->destroy_uniform_upload_buffer(backend_renderer->m_imgui_pipeline_none_update_index_uniform_buffer);
		backend_renderer->m_imgui_pipeline_none_update_index_uniform_buffer = NULL;

		assert(uint32_t(~0U) != backend_renderer->m_uniform_upload_buffer_offset_alignment);
		backend_renderer->m_uniform_upload_buffer_offset_alignment = uint32_t(~0U);
	}

	{
		IM_DELETE(backend_renderer);

		io.BackendRendererName = NULL;
		io.BackendRendererUserData = NULL;
		io.BackendFlags &= ~ImGuiBackendFlags_RendererHasVtxOffset;
	}
}

static inline uint32_t _internal_tbb_align_up(uint32_t value, uint32_t alignment)
{
	//
	//  Copyright (c) 2005-2019 Intel Corporation
	//
	//  Licensed under the Apache License, Version 2.0 (the "License");
	//  you may not use this file except in compliance with the License.
	//  You may obtain a copy of the License at
	//
	//      http://www.apache.org/licenses/LICENSE-2.0
	//
	//  Unless required by applicable law or agreed to in writing, software
	//  distributed under the License is distributed on an "AS IS" BASIS,
	//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	//  See the License for the specific language governing permissions and
	//  limitations under the License.
	//

	// [alignUp](https://github.com/oneapi-src/oneTBB/blob/tbb_2019/src/tbbmalloc/shared_utils.h#L42)

	assert(alignment != static_cast<uint32_t>(0));

	// power-of-2 alignment
	assert((alignment & (alignment - static_cast<uint32_t>(1))) == static_cast<uint32_t>(0));

	return (((value - static_cast<uint32_t>(1)) | (alignment - static_cast<uint32_t>(1))) + static_cast<uint32_t>(1));
}
