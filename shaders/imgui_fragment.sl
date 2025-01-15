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

brx_root_signature(imgui_root_signature_macro, imgui_root_signature_name)
brx_early_depth_stencil
brx_pixel_shader_parameter_begin(main)
brx_pixel_shader_parameter_in_frag_coord brx_pixel_shader_parameter_split
brx_pixel_shader_parameter_in(brx_float2, in_interpolated_vertex_texcoord, 0) brx_pixel_shader_parameter_split
brx_pixel_shader_parameter_in(brx_float4, in_interpolated_vertex_color, 1) brx_pixel_shader_parameter_split
brx_pixel_shader_parameter_out(brx_float4, out_color, 0)
brx_pixel_shader_parameter_end(main)
{
    brx_float2 texcoord = in_interpolated_vertex_texcoord;
    brx_float4 mesh_color = in_interpolated_vertex_color;

    brx_float4 texture_color = brx_sample_2d(g_font_texture, g_sampler, texcoord);

    out_color = mesh_color * texture_color;
}
