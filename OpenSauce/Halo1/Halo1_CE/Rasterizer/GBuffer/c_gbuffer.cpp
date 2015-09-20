/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/GBuffer/c_gbuffer.hpp"

#if !PLATFORM_IS_DEDI

#include "Rasterizer/GBuffer/Effects/c_gbuffer_effect_factory.hpp"

#include <blamlib/Halo1/render/render_cameras.hpp>
#include <blamlib/Halo1/render/render.hpp>

namespace Yelo
{
    namespace Rasterizer
    {
        namespace GBuffer
        {
            c_gbuffer::c_gbuffer(const bool use_os_shaders)
                : m_use_os_shaders(use_os_shaders),
                  m_wvp_stored(false),
                  m_stored_worldviewproj(),
                  m_stored_wvp_index(0),
                  m_pixel_shader(nullptr),
                  m_vertex_shader(nullptr),
                  m_pixel_shader_input(),
                  m_techniques(),
                  m_rt_depth(),
                  m_rt_velocity(),
                  m_rt_normals_index(),
                  m_output_map(),
                  m_stored_state() {}

            bool c_gbuffer::Create(IDirect3DDevice9& device, const uint32 width, const uint32 height)
            {
                if (!CreateShaders(device))
                {
                    return false;
                }

                if (!CreateTargets(device, width, height))
                {
                    return false;
                }

                D3DCAPS9 device_caps;
                device.GetDeviceCaps(&device_caps);
                m_output_map.m_output_count = (device_caps.NumSimultaneousRTs > k_maximum_multi_render_target ? k_maximum_multi_render_target : device_caps.NumSimultaneousRTs);
                // cant alpha test and put data in the alpha channel with 1 render target
                if (m_output_map.m_output_count == 1)
                {
                    return false;
                }

                m_techniques.vs_bsp_techniques.n = c_gbuffer_effect_factory::GetGBufferTechnique(*m_vertex_shader, "BSP", "N");
                m_techniques.vs_bsp_techniques.n_v = c_gbuffer_effect_factory::GetGBufferTechnique(*m_vertex_shader, "BSP", "N_V");
                m_techniques.vs_object_techniques.n = c_gbuffer_effect_factory::GetGBufferTechnique(*m_vertex_shader, "Object", "N");
                m_techniques.vs_object_techniques.tbn = c_gbuffer_effect_factory::GetGBufferTechnique(*m_vertex_shader, "Object", "TBN");
                m_techniques.vs_object_techniques.n_v = c_gbuffer_effect_factory::GetGBufferTechnique(*m_vertex_shader, "Object", "N_V");
                m_techniques.vs_object_techniques.tbn_v = c_gbuffer_effect_factory::GetGBufferTechnique(*m_vertex_shader, "Object", "TBN_V");
                m_techniques.vs_object_techniques.none = c_gbuffer_effect_factory::GetGBufferTechnique(*m_vertex_shader, "Object", "NONE");

                m_techniques.ps_bsp_techniques.n = c_gbuffer_effect_factory::GetGBufferTechnique(*m_pixel_shader, m_output_map.m_output_count, "BSP", "N");
                m_techniques.ps_bsp_techniques.n_v = c_gbuffer_effect_factory::GetGBufferTechnique(*m_pixel_shader, m_output_map.m_output_count, "BSP", "N_V");
                m_techniques.ps_object_techniques.n = c_gbuffer_effect_factory::GetGBufferTechnique(*m_pixel_shader, m_output_map.m_output_count, "Object", "N");
                m_techniques.ps_object_techniques.tbn = c_gbuffer_effect_factory::GetGBufferTechnique(*m_pixel_shader, m_output_map.m_output_count, "Object", "TBN");
                m_techniques.ps_object_techniques.n_v = c_gbuffer_effect_factory::GetGBufferTechnique(*m_pixel_shader, m_output_map.m_output_count, "Object", "N_V");
                m_techniques.ps_object_techniques.tbn_v = c_gbuffer_effect_factory::GetGBufferTechnique(*m_pixel_shader, m_output_map.m_output_count, "Object", "TBN_V");
                m_techniques.ps_object_techniques.none = c_gbuffer_effect_factory::GetGBufferTechnique(*m_pixel_shader, m_output_map.m_output_count, "Object", "NONE");

                switch (m_output_map.m_output_count)
                {
                    case 2:
                        m_output_map.m_outputs[0][0] = m_rt_depth.IsEnabled() ? m_rt_depth.surface : nullptr;
                        m_output_map.m_outputs[1][0] = m_rt_velocity.IsEnabled() ? m_rt_velocity.surface : nullptr;
                        m_output_map.m_outputs[1][1] = m_rt_normals_index.IsEnabled() ? m_rt_normals_index.surface : nullptr;
                        break;
                    default:
                        m_output_map.m_outputs[0][0] = m_rt_depth.IsEnabled() ? m_rt_depth.surface : nullptr;
                        m_output_map.m_outputs[0][1] = m_rt_velocity.IsEnabled() ? m_rt_velocity.surface : nullptr;
                        m_output_map.m_outputs[0][2] = m_rt_normals_index.IsEnabled() ? m_rt_normals_index.surface : nullptr;
                        break;
                }
                return true;
            }

            void c_gbuffer::Destroy()
            {
                safe_release(m_pixel_shader);
                safe_release(m_vertex_shader);
                DestroyTargets();
            }

            void c_gbuffer::Update()
            {
                m_stored_wvp_index = 1 - m_stored_wvp_index;
                m_wvp_stored = false;
            }

            void c_gbuffer::SetVertexShaderConstants(IDirect3DDevice9& device) const
            {
                auto& render_globals = *Render::RenderGlobals();
                Render::s_render_frustum_matricies matrices;
                render_globals.frustum.GetMatricies(matrices);

                device.SetVertexShaderConstantF(222, m_stored_worldviewproj[m_stored_wvp_index], 4);

                D3DXMATRIX world_view_transpose;
                matrices.world_view_transpose->ConvertTo4x4(world_view_transpose);
                device.SetVertexShaderConstantF(226, world_view_transpose, 3);

                D3DXVECTOR4 linear_depth_multiplier;
                linear_depth_multiplier.x = pow(render_globals.frustum.z_far, -1.0f);
                device.SetVertexShaderConstantF(229, linear_depth_multiplier, 1);
            }

            HRESULT c_gbuffer::Draw(IDirect3DDevice9& device, const s_draw_primitive_parameters& parameters)
            {
                auto result = S_OK;
                PushState(device);
                device.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

                SetVertexShaderConstants(device);

                m_vertex_shader->Begin(nullptr, 0);
                m_vertex_shader->BeginPass(0);

                UINT pass_count;
                m_pixel_shader->Begin(&pass_count, 0);

                for (uint32 pass = 0; pass < pass_count; ++pass)
                {
                    for (uint32 index = 0; index < m_output_map.m_output_count; index++)
                    {
                        device.SetRenderTarget(index, m_output_map.m_outputs[pass][index]);
                    }

                    m_pixel_shader->BeginPass(pass);
                    device.SetPixelShaderConstantF(0, m_pixel_shader_input, 1);
                    result = device.DrawIndexedPrimitive(parameters.m_type,
                                                         parameters.m_base_vertex_index,
                                                         parameters.m_min_vertex_index,
                                                         parameters.m_num_vertices,
                                                         parameters.m_start_index,
                                                         parameters.m_primitive_count);
                    m_pixel_shader->EndPass();

                    for (uint32 index = 1; index < m_output_map.m_output_count; index++)
                    {
                        device.SetRenderTarget(index, nullptr);
                    }
                }

                m_pixel_shader->End();

                m_vertex_shader->EndPass();
                m_vertex_shader->End();

                PopState(device);
                return result;
            }

            HRESULT c_gbuffer::DrawStructure(IDirect3DDevice9& device, const s_draw_call& parameters)
            {
                PreDrawStructure(parameters);
                return Draw(device, parameters.m_primitive);
            }

            HRESULT c_gbuffer::DrawObject(IDirect3DDevice9& device, const s_draw_call_object& parameters)
            {
                PreDrawObject(parameters);
                return Draw(device, parameters.m_primitive);
            }

            void c_gbuffer::StoreWorldViewProj(const D3DMATRIX& matrix)
            {
                if (!m_wvp_stored)
                {
                    m_stored_worldviewproj[1 - m_stored_wvp_index] = matrix;
                    m_wvp_stored = true;
                }
            }

            void c_gbuffer::PushState(IDirect3DDevice9& device)
            {
                device.GetRenderTarget(0, &m_stored_state.m_texture);
                device.GetVertexShader(&m_stored_state.m_vertex_shader);
                device.GetRenderState(D3DRS_ZWRITEENABLE, &m_stored_state.m_z_write_enable);
            }

            void c_gbuffer::PopState(IDirect3DDevice9& device)
            {
                device.SetRenderState(D3DRS_ZWRITEENABLE, m_stored_state.m_z_write_enable);
                device.SetRenderTarget(0, m_stored_state.m_texture);
                device.SetVertexShader(m_stored_state.m_vertex_shader);
                safe_release(m_stored_state.m_texture);
                safe_release(m_stored_state.m_vertex_shader);
            }

            bool c_gbuffer::CreateShaders(IDirect3DDevice9& device)
            {
                auto& effect_factory = c_gbuffer_effect_factory::Get();
                if (!effect_factory.CreateEffect(device, &m_pixel_shader, "GBuffer_PS"))
                {
                    return false;
                }

                cstring gbuffer_shader_set;
                if (m_use_os_shaders)
                {
                    gbuffer_shader_set = "GBuffer_VS_OS";
                }
                else
                {
                    gbuffer_shader_set = "GBuffer_VS";
                }
                if (!effect_factory.CreateEffect(device, &m_vertex_shader, gbuffer_shader_set))
                {
                    return false;
                }
                return true;
            }

            bool c_gbuffer::CreateTargets(IDirect3DDevice9& device, const uint32 width, const uint32 height)
            {
                auto result = S_OK;
                result |= m_rt_depth.CreateTarget(&device, width, height, D3DFMT_R32F);
                result |= m_rt_velocity.CreateTarget(&device, width, height, D3DFMT_G16R16F);
                result |= m_rt_normals_index.CreateTarget(&device, width, height, D3DFMT_A8R8G8B8);
                return SUCCEEDED(result);
            }

            void c_gbuffer::DestroyTargets()
            {
                m_rt_depth.ReleaseTarget();
                m_rt_velocity.ReleaseTarget();
                m_rt_normals_index.ReleaseTarget();
            }


            void c_gbuffer::PreDrawStructure(const s_draw_call& parameters)
            {
                if (TEST_FLAG(parameters.m_option_flags, Flags::_gbuffer_render_options_velocity_bit))
                {
                    m_vertex_shader->SetTechnique(m_techniques.vs_bsp_techniques.n_v);
                    m_pixel_shader->SetTechnique(m_techniques.ps_bsp_techniques.n_v);
                    m_pixel_shader_input.z = 1.0f;
                }
                else
                {
                    m_vertex_shader->SetTechnique(m_techniques.vs_bsp_techniques.n);
                    m_pixel_shader->SetTechnique(m_techniques.ps_bsp_techniques.n);
                    m_pixel_shader_input.z = 0.0f;
                }
                m_pixel_shader_input.x = 2.0f * (1.0f / 255.0f);
                m_pixel_shader_input.y = 0.0f;
            }

            void c_gbuffer::PreDrawObject(const s_draw_call_object& parameters)
            {
                if (parameters.m_level_of_detail < 2)
                {
                    m_vertex_shader->SetTechnique(m_techniques.vs_object_techniques.n);
                    m_pixel_shader->SetTechnique(m_techniques.ps_object_techniques.n);
                }
                else if (TEST_FLAG(parameters.m_option_flags, Flags::_gbuffer_render_options_none_bit))
                {
                    m_vertex_shader->SetTechnique(m_techniques.vs_object_techniques.none);
                    m_pixel_shader->SetTechnique(m_techniques.ps_object_techniques.none);
                }
                else
                {
                    auto render_velocity = TEST_FLAG(parameters.m_option_flags, Flags::_gbuffer_render_options_velocity_bit);
                    if (TEST_FLAG(parameters.m_option_flags, Flags::_gbuffer_render_options_tangent_normals_binormals_bit))
                    {
                        m_vertex_shader->SetTechnique(
                            (render_velocity ?
                                 m_techniques.vs_object_techniques.tbn_v :
                                 m_techniques.vs_object_techniques.tbn));

                        m_pixel_shader->SetTechnique(
                            (render_velocity ?
                                 m_techniques.ps_object_techniques.tbn_v :
                                 m_techniques.ps_object_techniques.tbn));
                    }
                    else
                    {
                        m_vertex_shader->SetTechnique(
                            (render_velocity ?
                                 m_techniques.vs_object_techniques.n_v :
                                 m_techniques.vs_object_techniques.n));

                        m_pixel_shader->SetTechnique(
                            (render_velocity ?
                                 m_techniques.ps_object_techniques.n_v :
                                 m_techniques.ps_object_techniques.n));
                    }

                    if (render_velocity)
                    {
                        m_pixel_shader_input.z = 1.0f;
                    }
                    else
                    {
                        m_pixel_shader_input.z = 0.0f;
                    }
                }
                m_pixel_shader_input.x = static_cast<float>(parameters.m_mesh_index) * (1.0f / 255.0f);
                m_pixel_shader_input.y = static_cast<float>(parameters.m_team_index) * (1.0f / 255.0f);
            }

            bool c_gbuffer::SetEffectVariables(ID3DXEffect& effect,
                bool& effect_has_variable,
                const cstring texture_semantic,
                const Render::s_render_target& target,
                const cstring x_handle_semantic, const int32 x_index,
                const cstring y_handle_semantic, const int32 y_index,
                const cstring z_handle_semantic, const int32 z_index,
                const cstring w_handle_semantic, const int32 w_index) const
            {
                effect_has_variable = false;

                auto texture_handle = effect.GetParameterBySemantic(nullptr, texture_semantic);
                effect_has_variable = texture_handle != nullptr;
                if (!texture_handle)
                {
                    return true;
                }
                if (!target.IsEnabled())
                {
                    return false;
                }
                effect.SetTexture(texture_handle, target.texture);

                auto result = true;
                if (x_handle_semantic)
                {
                    result &= SetEffectInteger(effect, x_handle_semantic, x_index);
                }
                if (y_handle_semantic)
                {
                    result &= SetEffectInteger(effect, y_handle_semantic, y_index);
                }
                if (z_handle_semantic)
                {
                    result &= SetEffectInteger(effect, z_handle_semantic, z_index);
                }
                if (w_handle_semantic)
                {
                    result &= SetEffectInteger(effect, w_handle_semantic, w_index);
                }
                return result;
            }

            bool c_gbuffer::SetEffectInteger(ID3DXEffect& effect, const cstring semantic, const int32 value) const
            {
                auto index_handle = effect.GetParameterBySemantic(nullptr, semantic);
                if (!index_handle)
                {
                    return false;
                }
                effect.SetInt(index_handle, value);
                return true;
            }

            bool c_gbuffer::SetDepth(ID3DXEffect& effect, bool& variable_used) const
            {
                return SetEffectVariables(effect, variable_used, "TEXDEPTH", m_rt_depth,
                                          "GBUFFER_DEPTH_X", k_gbuffer_depth_x);
            }

            bool c_gbuffer::SetVelocity(ID3DXEffect& effect, bool& variable_used) const
            {
                return SetEffectVariables(effect, variable_used, "TEXVELOCITY", m_rt_velocity,
                                          "GBUFFER_VELOCITY_X", k_gbuffer_velocity_x,
                                          "GBUFFER_VELOCITY_Y", k_gbuffer_velocity_y);
            }

            bool c_gbuffer::SetNormals(ID3DXEffect& effect, bool& variable_used) const
            {
                return SetEffectVariables(effect, variable_used, "TEXNORMALS", m_rt_normals_index,
                                          "GBUFFER_NORMALS_X", k_gbuffer_normals_x,
                                          "GBUFFER_NORMALS_Y", k_gbuffer_normals_y);
            }

            Render::s_render_target& c_gbuffer::GetDepthTarget()
            {
                return m_rt_depth;
            }

            Render::s_render_target& c_gbuffer::GetVelocityTarget()
            {
                return m_rt_velocity;
            }

            Render::s_render_target& c_gbuffer::GetNormalsIndexTarget()
            {
                return m_rt_normals_index;
            }

            s_gbuffer_output_map& c_gbuffer::OutputMap()
            {
                return m_output_map;
            }

            bool c_gbuffer::SetIndex(ID3DXEffect& effect, bool& variable_used) const
            {
                return SetEffectVariables(effect, variable_used, "TEXINDEX", m_rt_normals_index,
                                          "GBUFFER_INDEX_X", k_gbuffer_index_x,
                                          "GBUFFER_INDEX_Y", k_gbuffer_index_y);
            }
        };
    };
};
#endif
