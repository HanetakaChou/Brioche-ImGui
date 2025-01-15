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

#include "imgui_pipeline_resource_binding.sli"
#include "../../Packed-Vector/shaders/packed_vector.sli"

brx_root_signature(imgui_root_signature_macro, imgui_root_signature_name)
brx_vertex_shader_parameter_begin(main)
brx_vertex_shader_parameter_in_vertex_index brx_vertex_shader_parameter_split
brx_vertex_shader_parameter_out_position brx_vertex_shader_parameter_split
brx_vertex_shader_parameter_out(brx_float2, out_vertex_texcoord, 0) brx_vertex_shader_parameter_split
brx_vertex_shader_parameter_out(brx_float4, out_vertex_color, 1)
brx_vertex_shader_parameter_end(main)
{
    brx_uint vertex_index;
    {
        brx_uint index_uniform_buffer_index_aligned_16 = (brx_uint(g_imgui_index_uint16_buffer_stride * brx_uint(brx_vertex_index)) >> 4);
        brx_uint index_uniform_buffer_offset_aligned_16 = (brx_uint(g_imgui_index_uint16_buffer_stride * brx_uint(brx_vertex_index)) & 15u);

        brx_uint4 packed_vector_index_binding = g_index_uniform_buffer[index_uniform_buffer_index_aligned_16];

        brx_uint index_uniform_buffer_index_aligned_4 = (index_uniform_buffer_offset_aligned_16 >> 2);
        brx_uint index_uniform_buffer_offset_aligned_4 = (index_uniform_buffer_offset_aligned_16 & 3u);

        brx_uint2 unpacked_vector_index_binding = R16G16_UINT_to_UINT2(packed_vector_index_binding[index_uniform_buffer_index_aligned_4]);

        vertex_index = (0u == index_uniform_buffer_offset_aligned_4) ? unpacked_vector_index_binding.x : unpacked_vector_index_binding.y;
    }

    brx_float2 vertex_position;
    {
        brx_uint vertex_position_uniform_buffer_index_aligned_16 = (brx_uint(g_imgui_vertex_position_buffer_stride * brx_uint(vertex_index)) >> 4);
        brx_uint vertex_position_uniform_buffer_offset_aligned_16 = (brx_uint(g_imgui_vertex_position_buffer_stride * brx_uint(vertex_index)) & 15u);

        brx_uint4 packed_vector_vertex_position_binding = g_vertex_position_uniform_buffer[vertex_position_uniform_buffer_index_aligned_16];
        brx_uint vertex_position_uniform_buffer_index_aligned_4 = (vertex_position_uniform_buffer_offset_aligned_16 >> 2);

        vertex_position = R16G16_SNORM_to_FLOAT2(packed_vector_vertex_position_binding[vertex_position_uniform_buffer_index_aligned_4]);
    }

    brx_float2 vertex_texcoord;
    {
        brx_uint vertex_texcoord_uniform_buffer_index_aligned_16 = (brx_uint(g_imgui_vertex_texcoord_buffer_stride * brx_uint(vertex_index)) >> 4);
        brx_uint vertex_texcoord_uniform_buffer_offset_aligned_16 = (brx_uint(g_imgui_vertex_texcoord_buffer_stride * brx_uint(vertex_index)) & 15u);

        brx_uint4 packed_vector_vertex_texcoord_binding = g_vertex_texcoord_uniform_buffer[vertex_texcoord_uniform_buffer_index_aligned_16];
        brx_uint vertex_texcoord_uniform_buffer_index_aligned_4 = (vertex_texcoord_uniform_buffer_offset_aligned_16 >> 2);

        vertex_texcoord = R16G16_UNORM_to_FLOAT2(packed_vector_vertex_texcoord_binding[vertex_texcoord_uniform_buffer_index_aligned_4]);
    }

    brx_float4 vertex_color;
    {
        brx_uint vertex_color_uniform_buffer_index_aligned_16 = (brx_uint(g_imgui_vertex_color_buffer_stride * brx_uint(vertex_index)) >> 4);
        brx_uint vertex_color_uniform_buffer_offset_aligned_16 = (brx_uint(g_imgui_vertex_color_buffer_stride * brx_uint(vertex_index)) & 15u);

        brx_uint4 packed_vector_vertex_color_binding = g_vertex_color_uniform_buffer[vertex_color_uniform_buffer_index_aligned_16];
        brx_uint vertex_color_uniform_buffer_index_aligned_4 = (vertex_color_uniform_buffer_offset_aligned_16 >> 2);

        vertex_color = R8G8B8A8_UNORM_to_FLOAT4(packed_vector_vertex_color_binding[vertex_color_uniform_buffer_index_aligned_4]);
    }

    brx_position = brx_float4(vertex_position, 0.0, 1.0);

    out_vertex_texcoord = vertex_texcoord;

    out_vertex_color = vertex_color;
}
