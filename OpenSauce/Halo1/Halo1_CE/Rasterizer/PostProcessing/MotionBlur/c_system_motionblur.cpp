/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_system_motionblur.hpp"

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"
#include "Rasterizer/PostProcessing/c_effect_postprocess.hpp"
#include "Rasterizer/PostProcessing/c_effect_instance.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_shader_motionblur.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_shader_instance_motionblur.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_settings_motionblur.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/s_shader_motionblur_definition.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace MotionBlur
            {
                c_system_motionblur c_system_motionblur::g_motionblur_system;

                static TagGroups::s_pass_definition g_shader_passes_sm3[] =
                {
                    {
                        "Mask",
                        { 0, 0, 0 },
                        0
                    },
                    {
                        "Blur",
                        { 0, 0, 0 },
                        0
                    }
                };
                static TagGroups::s_technique_definition g_shader_techniques[] =
                {
                    {
                        "",
                        { 1, 1, 0 },
                        { 0 },
                        { 0, NULL, NULL }
                    },
                    {
                        "PostProcess",
                        { 0, 0, 1 },
                        { 0 },
                        { NUMBEROF(g_shader_passes_sm3), &g_shader_passes_sm3[0], NULL }
                    }
                };
                static s_shader_motionblur_definition g_shader_motionblur_definition;
                static TagGroups::s_effect_postprocess_definition g_effect_motionblur_definition;

                static c_shader_motionblur g_shader_motionblur;
                static c_shader_instance_motionblur g_shader_instance_motionblur;
                static c_effect_postprocess g_effect_motionblur;
                static c_effect_instance g_effect_instance;

                c_system_motionblur& c_system_motionblur::Instance()
                {
                    return g_motionblur_system;
                }

                bool& c_system_motionblur::Enabled()
                {
                    return m_members.m_flags.is_enabled;
                }

                bool c_system_motionblur::IsReady()
                {
                    return m_members.m_flags.is_ready;
                }

                bool c_system_motionblur::IsUnloaded()
                {
                    return m_members.m_flags.is_unloaded;
                }

                real& c_system_motionblur::BlurAmount()
                {
                    return g_shader_instance_motionblur.BlurAmount();
                }

                void c_system_motionblur::Initialize()
                {
                    c_settings_motionblur::Register(Settings::Manager());

                    m_members.status = Enums::pp_component_status_uninitialised;

                    m_members.m_flags.is_ready = false;
                    m_members.m_flags.is_unloaded = false;

                    g_shader_motionblur_definition.runtime.flags.valid_shader_bit = false;
                    g_shader_motionblur_definition.techniques.Count = NUMBEROF(g_shader_techniques);
                    g_shader_motionblur_definition.techniques.Address = &g_shader_techniques[0];

                    g_effect_motionblur_definition.quad_definition.tessellation.x = 5;
                    g_effect_motionblur_definition.quad_definition.tessellation.y = 5;
                    g_effect_motionblur_definition.quad_definition.x_bounds.lower = 0;
                    g_effect_motionblur_definition.quad_definition.x_bounds.upper = 1;
                    g_effect_motionblur_definition.quad_definition.y_bounds.lower = 0;
                    g_effect_motionblur_definition.quad_definition.y_bounds.upper = 1;
                    g_effect_motionblur_definition.runtime.flags.valid_effect_bit = false;
                    g_effect_motionblur_definition.runtime.flags.is_active_bit = true;

                    g_shader_motionblur.Ctor();
                    g_shader_motionblur.SetShaderDefinition(&g_shader_motionblur_definition);
                    g_shader_motionblur.SetupShader();

                    g_shader_instance_motionblur.Ctor();
                    g_shader_instance_motionblur.SetShader(&g_shader_motionblur);
                    g_shader_instance_motionblur.SetupShaderInstance();

                    g_effect_motionblur.Ctor();
                    g_effect_motionblur.SetEffectDefinition(&g_effect_motionblur_definition);
                    g_effect_motionblur.AddShaderInstance(&g_shader_instance_motionblur);
                    g_effect_motionblur.SetupEffect();

                    g_effect_instance.Ctor();
                    g_effect_instance.SetEffect(&g_effect_motionblur);
                    g_effect_instance.SetQuadDefinition(&g_effect_motionblur_definition.quad_definition);
                    g_effect_instance.SetupEffectInstance();
                }

                void c_system_motionblur::Dispose()
                {
                    m_members.status = Enums::pp_component_status_uninitialised;

                    m_members.m_flags.is_ready = false;
                    m_members.m_flags.is_unloaded = false;

                    g_effect_instance.Dtor();
                    g_effect_motionblur.Dtor();
                    g_shader_instance_motionblur.Dtor();
                    g_shader_motionblur.Dtor();

                    c_settings_motionblur::Unregister(Settings::Manager());
                }

                void c_system_motionblur::InitializeResources_Base(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters)
                {
                    auto hr = E_FAIL;
                    // create the direct3d resources of the shader
                    do
                    {
                        if (FAILED(hr = CreateShader()))
                        {
                            break;
                        }
                        if (FAILED(hr = CreateEffect()))
                        {
                            break;
                        }
                    } while (false);

                    Validate();

                    if (SUCCEEDED(hr))
                    {
                        m_members.status = Enums::pp_component_status_initialised;
                    }
                    else
                    {
                        m_members.status = Enums::pp_component_status_initialisation_failed;
                    }

                    UpdateStatus();
                }

                void c_system_motionblur::OnLostDevice_Base()
                {
                    if (!m_members.m_flags.is_unloaded)
                    {
                        SetDeviceLost();
                    }
                    Validate();

                    UpdateStatus();
                }

                void c_system_motionblur::OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters)
                {
                    // call on device reset on the shaders effect
                    auto hr = E_FAIL;
                    if (!m_members.m_flags.is_unloaded)
                    {
                        hr = SetDeviceReset();
                    }
                    Validate();

                    if (SUCCEEDED(hr))
                    {
                        m_members.status = Enums::pp_component_status_initialised;
                    }
                    else
                    {
                        m_members.status = Enums::pp_component_status_initialisation_failed;
                    }

                    UpdateStatus();
                }

                void c_system_motionblur::ReleaseResources_Base()
                {
                    DestroyShader();
                    DestroyEffect();
                    Validate();

                    m_members.status = Enums::pp_component_status_uninitialised;

                    UpdateStatus();
                }

                void c_system_motionblur::Unload()
                {
                    // release the direct3d resources of the shader
                    DestroyShader();
                    DestroyEffect();
                    Validate();

                    m_members.status = Enums::pp_component_status_uninitialised;
                    m_members.m_flags.is_unloaded = true;

                    UpdateStatus();
                }

                void c_system_motionblur::Load()
                {
                    auto hr = E_FAIL;
                    // create the direct3d resources of the shader
                    do
                    {
                        if (FAILED(hr = CreateShader()))
                        {
                            break;
                        }
                        if (FAILED(hr = CreateEffect()))
                        {
                            break;
                        }
                    } while (false);

                    Validate();

                    if (SUCCEEDED(hr))
                    {
                        m_members.status = Enums::pp_component_status_initialised;
                    }
                    else
                    {
                        m_members.status = Enums::pp_component_status_initialisation_failed;
                    }

                    m_members.m_flags.is_unloaded = false;

                    UpdateStatus();
                }

                bool c_system_motionblur::Render(Enums::postprocess_render_stage render_stage)
                {
                    auto applied = false;
                    do
                    {
                        if (render_stage != Enums::_postprocess_render_stage_blur)
                        {
                            break;
                        }
                        if (!m_members.m_flags.is_enabled)
                        {
                            break;
                        }

                        applied = Render(c_post_processing_main::Instance().Globals().render_device);
                    } while (false);

                    return applied;
                }

                void c_system_motionblur::Update(real delta_time)
                {
                    g_shader_motionblur.UpdateVariables();
                    g_effect_motionblur.Update(delta_time);
                    g_effect_instance.UpdateEffectInstance(delta_time);
                }

                HRESULT c_system_motionblur::CreateShader()
                {
                    return g_shader_motionblur.LoadShader(c_post_processing_main::Instance().Globals().render_device);
                }

                void c_system_motionblur::DestroyShader()
                {
                    g_shader_motionblur.UnloadShader();
                }

                HRESULT c_system_motionblur::CreateEffect()
                {
                    g_effect_motionblur.SetupEffectPostCreation();
                    return g_effect_instance.LoadEffectInstance();
                }

                void c_system_motionblur::DestroyEffect()
                {
                    g_effect_instance.UnloadEffectInstance();
                }

                void c_system_motionblur::SetDeviceLost()
                {
                    g_shader_motionblur.OnDeviceLost();
                }

                HRESULT c_system_motionblur::SetDeviceReset()
                {
                    return g_shader_motionblur.OnDeviceReset();
                }

                void c_system_motionblur::Validate()
                {
                    g_shader_motionblur.Validate();
                    g_shader_instance_motionblur.Validate();
                    g_effect_motionblur.Validate();
                    g_effect_instance.Validate();
                }

                void c_system_motionblur::UpdateStatus()
                {
                    m_members.m_flags.is_ready = false;
                    do
                    {
                        if (m_members.status != Enums::pp_component_status_initialised)
                        {
                            break;
                        }
                        if (!g_effect_instance.IsValid())
                        {
                            break;
                        }

                        m_members.m_flags.is_ready = true;
                    } while (false);
                }

                bool c_system_motionblur::Render(IDirect3DDevice9* render_device)
                {
                    return SUCCEEDED(g_effect_instance.Render(render_device));
                }
            }
        }
    }
}
#endif
