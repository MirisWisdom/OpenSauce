/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/c_shader_postprocess.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include "Rasterizer/PostProcessing/PostProcessingErrorReporting.hpp"
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"
#include "Rasterizer/PostProcessing/c_shader_include_manager.hpp"
#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_base.hpp"
#include "Rasterizer/GBuffer.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            void c_shader_postprocess::ClearMembers()
            {
                m_members.definition = nullptr;
                m_members.source_data = nullptr;
            }

            void c_shader_postprocess::SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition)
            {
                m_members.definition = definition;
            }

            void c_shader_postprocess::SetSourceData(c_shader_data_base* source)
            {
                m_members.source_data = source;
            }

            LPD3DXEFFECT c_shader_postprocess::GetEffect()
            {
                return m_members.definition->runtime.dx_effect;
            }

            bool c_shader_postprocess::IsValid()
            {
                return m_members.definition->runtime.flags.valid_shader_bit;
            }

            void c_shader_postprocess::Ctor()
            {
                ClearMembers();
            }

            void c_shader_postprocess::Dtor()
            {
                ClearMembers();
            }

            HRESULT c_shader_postprocess::LoadShader(IDirect3DDevice9* render_device)
            {
                // attempt to load the shader
                auto result = LoadShaderImpl(render_device);

                if (FAILED(result))
                {
                    return result;
                }

                // it was thought this would load the shaders bitmaps, alas it does not...but we do it anyway
                blam::predicted_resources_precache(m_members.definition->predicted_resources);

                // if the shader loaded successfully get the standard variable handles and initialize their values
                GetHandles();
                SetVariables();

                return result;
            }

            void c_shader_postprocess::UnloadShader()
            {
                ClearHandles();
                UnloadShaderImpl();
            }

            void c_shader_postprocess::OnDeviceLost()
            {
                ClearHandles();
                OnDeviceLostImpl();
            }

            HRESULT c_shader_postprocess::OnDeviceReset()
            {
                auto result = OnDeviceResetImpl();

                if (FAILED(result))
                {
                    return result;
                }

                // if the shader reset successfully get the standard variable handles and initialize their values
                GetHandles();
                SetVariables();

                return result;
            }

            void c_shader_postprocess::Validate()
            {
                m_members.definition->runtime.flags.valid_shader_bit = ValidateImpl();
            }

            void c_shader_postprocess::GetHandles()
            {
                m_members.definition->runtime.postprocess_handle = GetTechniqueHandle();
                YELO_ASSERT_DISPLAY(m_members.definition->runtime.postprocess_handle != nullptr, "Unabled to find the active technique in the effect");

                m_members.definition->ortho_wvp_matrix.Initialize(m_members.definition->runtime.dx_effect,
                                                                  "ORTHOWORLDVIEWPROJECTION", true);
                m_members.definition->scene_size.Initialize(m_members.definition->runtime.dx_effect,
                                                            "SCENESIZE", true);
                m_members.definition->hud_scale.Initialize(m_members.definition->runtime.dx_effect,
                                                           "HUDSCALE", true);
                m_members.definition->pixel_size.Initialize(m_members.definition->runtime.dx_effect,
                                                            "PIXELSIZE", true);
                m_members.definition->near_clip_dist.Initialize(m_members.definition->runtime.dx_effect,
                                                                "NEARCLIPDISTANCE", true);
                m_members.definition->far_clip_dist.Initialize(m_members.definition->runtime.dx_effect,
                                                               "FARCLIPDISTANCE", true);
                m_members.definition->tex_source.Initialize(m_members.definition->runtime.dx_effect,
                                                            "TEXSOURCE", true);
                m_members.definition->tex_scene.Initialize(m_members.definition->runtime.dx_effect,
                                                           "TEXSCENE", true);
                m_members.definition->tex_buffer.Initialize(m_members.definition->runtime.dx_effect,
                                                            "TEXBUFFER", true);
            }

            void c_shader_postprocess::ClearHandles()
            {
                m_members.definition->runtime.postprocess_handle = nullptr;
                m_members.definition->ortho_wvp_matrix.ClearHandles();
                m_members.definition->scene_size.ClearHandles();
                m_members.definition->screen_fov.ClearHandles();
                m_members.definition->hud_scale.ClearHandles();
                m_members.definition->pixel_size.ClearHandles();
                m_members.definition->near_clip_dist.ClearHandles();
                m_members.definition->far_clip_dist.ClearHandles();
                m_members.definition->tex_source.ClearHandles();
                m_members.definition->tex_scene.ClearHandles();
                m_members.definition->tex_buffer.ClearHandles();
            }

            bool c_shader_postprocess::ValidateImpl()
            {
                auto valid = false;
                do
                {
                    if (!m_members.definition->runtime.dx_effect)
                    {
                        break;
                    }
                    if (!m_members.definition->runtime.active_technique)
                    {
                        break;
                    }
                    if (!m_members.definition->runtime.postprocess_handle)
                    {
                        break;
                    }
                    if (!m_members.definition->ortho_wvp_matrix.IsUsed())
                    {
                        break;
                    }
                    if (!m_members.source_data)
                    {
                        break;
                    }
                    if (!ValidateShaderVersions())
                    {
                        break;
                    }

                    valid = true;
                } while (false);

                return valid;
            }

            HRESULT c_shader_postprocess::LoadShaderImpl(IDirect3DDevice9* render_device)
            {
                auto hr = S_OK;
                LPD3DXBUFFER error_buffer = nullptr;

                // if the shader does not have the required data something is horrifically wrong
                YELO_ASSERT_DISPLAY(m_members.definition, "c_shader_postprocess has no shader assigned to it");
                YELO_ASSERT_DISPLAY(m_members.source_data, "c_shader_postprocess has no source data object assigned to it");
                if (!m_members.definition || !m_members.source_data)
                {
                    return E_FAIL;
                }

                // if the effect has already been created (derived shaders) do not recreate it
                if (m_members.definition->runtime.dx_effect == nullptr)
                {
                    // get the shader data and its size
                    DWORD data_size = 0;
                    auto data = m_members.source_data->GetData(data_size);

                    // if the data is not available, return E_FAIL
                    YELO_ASSERT_DISPLAY(data != nullptr, "source data object has no data to use");
                    YELO_ASSERT_DISPLAY(data_size != 0, "source data object has no data to use");

                    // for ascii files loaded from external sources, their includes are handled manually
                    c_shader_include_manager include_manager(m_members.source_data->GetIncludePath());

                    // create the effect
                    // macros can be used, and are defined in the source shader data struct
                    hr = D3DXCreateEffect(render_device,
                                          data,
                                          data_size,
                                          m_members.source_data->GetMacros(),
                                          &include_manager,
                                          D3DXSHADER_OPTIMIZATION_LEVEL3,
                                          nullptr,
                                          &m_members.definition->runtime.dx_effect,
                                          &error_buffer);

                    // if effect creation failed, report the error
                    if (FAILED(hr))
                    {
                        ErrorReporting::Write("Failed to load shader:");
                        ErrorReporting::Write(m_members.source_data->DataSourceID());
                        if (error_buffer)
                        {
                            ErrorReporting::WriteD3DXErrors(error_buffer);
                        }
                    }
                    safe_release(error_buffer);
                }

                m_members.definition->runtime.active_technique = GetActiveTechnique();
                YELO_ASSERT_DISPLAY(m_members.definition->runtime.active_technique != nullptr, "a c_shader_postprocess does not have an active technique");

                return hr;
            }

            void c_shader_postprocess::UnloadShaderImpl()
            {
                if (m_members.source_data)
                {
                    m_members.source_data->Dispose();
                }
                safe_release(m_members.definition->runtime.dx_effect);
            }

            void c_shader_postprocess::OnDeviceLostImpl()
            {
                if (m_members.definition->runtime.dx_effect)
                {
                    m_members.definition->runtime.dx_effect->OnLostDevice();
                }
            }

            HRESULT c_shader_postprocess::OnDeviceResetImpl()
            {
                auto hr = E_FAIL;
                if (m_members.definition->runtime.dx_effect)
                {
                    hr = m_members.definition->runtime.dx_effect->OnResetDevice();
                }
                return hr;
            }

            bool c_shader_postprocess::ValidateShaderVersions()
            {
                auto render_device = c_post_processing_main::Instance().Globals().render_device;
                auto& device_caps = c_post_processing_main::Instance().Globals().device_caps;

                // iterate through each effect
                auto effect = m_members.definition->runtime.dx_effect;

                // iterate through each technique
                auto technique = effect->GetTechniqueByName(m_members.definition->runtime.active_technique->name);

                if (!technique)
                {
                    return false;
                }

                D3DXTECHNIQUE_DESC technique_description;
                effect->GetTechniqueDesc(technique, &technique_description);
                for (UINT j = 0; j < technique_description.Passes; j++)
                {
                    // iterate through each pass
                    auto pass = effect->GetPass(technique, j);
                    D3DXPASS_DESC pass_description;
                    effect->GetPassDesc(pass, &pass_description);
                    // get the pixel and vertex shader versions
                    union
                    {
                        DWORD version;

                        struct
                        {
                            union
                            {
                                WORD major_minor_version;

                                struct
                                {
                                    char minor_version;
                                    char major_version;
                                };
                            };

                            WORD type;
                        };
                    } pass_vs_version, pass_ps_version;
                    pass_vs_version.version = D3DXGetShaderVersion(pass_description.pVertexShaderFunction);
                    pass_ps_version.version = D3DXGetShaderVersion(pass_description.pPixelShaderFunction);

                    // check the shaders can be used on the current graphics device
                    auto ps_version_ok = !(pass_vs_version.version > device_caps.VertexShaderVersion);
                    auto vs_version_ok = !(pass_ps_version.version > device_caps.PixelShaderVersion);
                    if (!ps_version_ok || !vs_version_ok)
                    {
                        ErrorReporting::Write("Failed to load shader:");
                        ErrorReporting::Write(m_members.source_data->DataSourceID());
                        ErrorReporting::WriteLine("The device does not support the required shader model [VS:%i.%i, PS:%i.%i]",
                                                  pass_vs_version.major_version, pass_vs_version.minor_version, pass_ps_version.major_version, pass_ps_version.minor_version);
                        return false;
                    }

                    // if either the pixel or vertex shader version is 3.0 or above, then ensure that
                    // they are both of the same version.
                    if ((pass_ps_version.version >= D3DPS_VERSION(3,0) || pass_vs_version.version >= D3DPS_VERSION(3,0))
                        && (pass_ps_version.major_minor_version != pass_vs_version.major_minor_version))
                    {
                        ErrorReporting::Write("Failed to load shader:");
                        ErrorReporting::Write(m_members.source_data->DataSourceID());
                        ErrorReporting::WriteLine("Incompatible shader models [VS:%i.%i, PS:%i.%i]",
                                                  pass_vs_version.major_version, pass_vs_version.minor_version, pass_ps_version.major_version, pass_ps_version.minor_version);
                        return false;
                    }
                }
                return true;
            }

            TagGroups::s_technique_definition* c_shader_postprocess::GetActiveTechnique()
            {
                auto& device_caps = c_post_processing_main::Instance().Globals().device_caps;

                byte vertex_shader_model = D3DSHADER_VERSION_MAJOR(device_caps.VertexShaderVersion);
                byte pixel_shader_model = D3DSHADER_VERSION_MAJOR(device_caps.VertexShaderVersion);

                // use the lowest shader model to be safe
                auto shader_model = (vertex_shader_model > pixel_shader_model ? pixel_shader_model : vertex_shader_model);

                TagGroups::s_technique_definition* technique = nullptr;
                // look for a shader model 3 technique
                for (auto i = 0; i < m_members.definition->techniques.Count && technique == nullptr; i++)
                {
                    auto& element = m_members.definition->techniques[i];
                    if (shader_model > 2 && element.shader_model.sm_3_0_bit)
                    {
                        technique = &m_members.definition->techniques[i];
                    }
                }
                // look for a shader model 2 technique
                for (auto i = 0; i < m_members.definition->techniques.Count && technique == nullptr; i++)
                {
                    auto& element = m_members.definition->techniques[i];
                    if (shader_model > 1 && element.shader_model.sm_2_0_bit)
                    {
                        technique = &m_members.definition->techniques[i];
                    }
                }
                // look for a shader model 1 technique
                for (auto i = 0; i < m_members.definition->techniques.Count && technique == nullptr; i++)
                {
                    auto& element = m_members.definition->techniques[i];
                    if (shader_model > 0 && element.shader_model.sm_1_0_bit)
                    {
                        technique = &m_members.definition->techniques[i];
                    }
                }
                return technique;
            }

            D3DXHANDLE c_shader_postprocess::GetTechniqueHandle()
            {
                return m_members.definition->runtime.dx_effect->GetTechniqueByName(m_members.definition->runtime.active_technique->name);
            }

            void c_shader_postprocess::SetVariables()
            {
                auto& globals = c_post_processing_main::Instance().Globals();

                auto effect = m_members.definition->runtime.dx_effect;

                effect->SetTechnique(m_members.definition->runtime.postprocess_handle);

                m_members.definition->ortho_wvp_matrix.SetVariable(effect, &globals.ortho_proj_matrix);
                m_members.definition->scene_size.SetVariable(effect, &globals.screen_dimensions);
                m_members.definition->screen_fov.SetVariable(effect, &globals.screen_fov);
                m_members.definition->hud_scale.SetVariable(effect, &globals.m_hud.value);

                m_members.definition->pixel_size.SetVariable(effect, &globals.pixel_size);
                m_members.definition->near_clip_dist.SetVariable(effect, &globals.m_clipping.near_clip);
                m_members.definition->far_clip_dist.SetVariable(effect, &globals.m_clipping.far_clip);
                m_members.definition->tex_scene.SetVariable(effect, nullptr);
                m_members.definition->tex_source.SetVariable(effect, nullptr);
                m_members.definition->tex_buffer.SetVariable(effect, nullptr);

                auto variable_used = false;
                globals.gbuffer->SetDepth(effect, variable_used);
                m_members.definition->runtime.flags.uses_gbuffer_bit |= variable_used;

                globals.gbuffer->SetVelocity(effect, variable_used);
                m_members.definition->runtime.flags.uses_gbuffer_bit |= variable_used;

                globals.gbuffer->SetNormals(effect, variable_used);
                m_members.definition->runtime.flags.uses_gbuffer_bit |= variable_used;

                globals.gbuffer->SetIndex(effect, variable_used);
                m_members.definition->runtime.flags.uses_gbuffer_bit |= variable_used;
            }

            void c_shader_postprocess::UpdateVariables()
            {
                auto& globals = c_post_processing_main::Instance().Globals();

                // set any standard variables that change per tick
                m_members.definition->screen_fov.SetVariable(m_members.definition->runtime.dx_effect, &globals.screen_fov);
                m_members.definition->hud_scale.SetVariable(m_members.definition->runtime.dx_effect, &globals.m_hud.value);
                m_members.definition->near_clip_dist.SetVariable(m_members.definition->runtime.dx_effect, &globals.m_clipping.near_clip);
                m_members.definition->far_clip_dist.SetVariable(m_members.definition->runtime.dx_effect, &globals.m_clipping.far_clip);
            }

            HRESULT c_shader_postprocess::Render(IDirect3DDevice9* render_device, Render::c_quad_instance* render_quad)
            {
                auto effect = m_members.definition->runtime.dx_effect;

                YELO_ASSERT_DISPLAY(effect, "a c_shader_postprocess is trying to be rendered when its effect is NULL");

                // start the effect
                UINT pass_count;
                HRESULT_ERETURN(effect->Begin( &pass_count, 0 ));

                // if there is no active technique, or the techniques pass count does not match that from the effect
                YELO_ASSERT_DISPLAY(m_members.definition->runtime.active_technique, "a c_shader_postprocess is tring to be rendered when it has no active technique");
                YELO_ASSERT_DISPLAY(m_members.definition->runtime.active_technique->passes.Count == pass_count, "a c_shader_postprocess' active technique pass count does not match the effect technique");

                auto& main_chain = c_post_processing_main::Instance().Globals().scene_buffer_chain;
                auto& secondary_chain = c_post_processing_main::Instance().Globals().secondary_buffer_chain;

                // render each pass of the effect
                auto hr = E_FAIL;
                for (int32 pass = 0; pass < m_members.definition->runtime.active_technique->passes.Count; ++pass)
                {
                    auto& pass_element = m_members.definition->runtime.active_technique->passes[pass];

                    // shaders can choose to write to either the main chain or a secondary buffer chain
                    DX9::s_render_target_chain* render_chain;
                    switch (pass_element.render_chain)
                    {
                        case 1:
                            render_chain = &secondary_chain;
                            break;
                        default:
                            render_chain = &main_chain;
                    }

                    // if the shader uses the scene texture, set it
                    if (m_members.definition->tex_scene.IsUsed())
                    {
                        m_members.definition->tex_scene.SetVariable(effect, main_chain.GetSceneTexture());
                    }

                    // if the shader uses the source texture, set it
                    if (m_members.definition->tex_source.IsUsed())
                    {
                        m_members.definition->tex_source.SetVariable(effect, main_chain.m_first_render ? main_chain.GetSceneTexture() : main_chain.GetNextTexture());
                    }

                    // if the shader uses the buffer texture, set it
                    // if the buffer has not been written to yet and the buffer texture is not cleared set to the scene texture
                    if (m_members.definition->tex_buffer.IsUsed())
                    {
                        if (secondary_chain.m_first_render && !pass_element.flags.clear_buffer_texture_bit)
                        {
                            m_members.definition->tex_buffer.SetVariable(effect, main_chain.GetSceneTexture());
                        }
                        else
                        {
                            m_members.definition->tex_buffer.SetVariable(effect, secondary_chain.GetNextTexture());
                        }
                    }

                    // when copy_scene_to_target is true the scene texture is stretchrect'd to the target texture
                    // this is so that the shader can draw onto, and sample, the current scene texture
                    if (pass_element.flags.copy_scene_to_target_bit)
                    {
                        render_device->StretchRect(main_chain.GetSceneSurface(), nullptr, render_chain->GetCurrentSurface(), nullptr, D3DTEXF_POINT);
                    }

                    // clear the buffer texture if necessary
                    // this should be avoided where possible as it is very expensive
                    if (pass_element.flags.clear_buffer_texture_bit)
                    {
                        render_device->SetRenderTarget(0, secondary_chain.GetNextSurface());
                        render_device->Clear(0L, nullptr, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0L);
                    }

                    // set the render target to the render chains target surface
                    render_device->SetRenderTarget(0, render_chain->GetCurrentSurface());

                    // if the pass needs a clear target, clear it
                    // this should be avoided where possible as it is very expensive
                    if (pass_element.flags.clear_target_bit)
                    {
                        render_device->Clear(0L, nullptr, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0L);
                    }

                    // render the pass
                    effect->BeginPass(pass);
                    {
                        // the vertex and index sources have already been set by the quad manager, the quad instance struct details
                        // the location of the quad in the buffers
                        hr = render_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
                                                                 render_quad->Quad().start_vertex,
                                                                 0,
                                                                 render_quad->Quad().vertex_count,
                                                                 render_quad->Quad().start_index,
                                                                 render_quad->Quad().primitive_count);
                    };
                    effect->EndPass();

                    // swap the render chain textures so this passes result becomes the next passes source texture
                    render_chain->m_first_render = false;
                    render_chain->Flip();
                }
                effect->End();

                return hr;
            }
        }
    }
}
#endif
