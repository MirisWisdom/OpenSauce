/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"
#include "Rasterizer/PostProcessing/Fade/c_system_fade.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_system_motionblur.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_system_bloom.hpp"
#include "Rasterizer/PostProcessing/FXAA/c_system_fxaa.hpp"
#include "Rasterizer/PostProcessing/Generic/Internal/c_system_internal.hpp"
#include "Rasterizer/PostProcessing/Generic/External/c_system_external.hpp"

#ifdef API_DEBUG
#define RENDER_STATE(state) "##state##", state
#else
#define RENDER_STATE(state) state
#endif

// TODO: Finish replacing all for( NUMBEROF() ) loops with range for (auto& subsystem : g_postprocess_subsystems)

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            static s_render_state_manager g_render_state_managers[] =
            {
                {
#ifdef API_DEBUG
                    "pp_render_stage_pre_blur",
#endif
                    {
                        { RENDER_STATE(D3DRS_BLENDOP), 0, D3DBLENDOP_ADD },
                        { RENDER_STATE(D3DRS_FILLMODE), 0, D3DFILL_SOLID },
                        { RENDER_STATE(D3DRS_SRCBLEND), 0, D3DBLEND_ONE },
                        { RENDER_STATE(D3DRS_DESTBLEND), 0, D3DBLEND_ONE },
                        { RENDER_STATE(D3DRS_ZENABLE), 0, false },
                        { RENDER_STATE(D3DRS_STENCILENABLE), 0, false },
                        { RENDER_STATE(D3DRS_COLORWRITEENABLE), 0, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA },
                        { RENDER_STATE(D3DRS_ALPHABLENDENABLE), 0, false },
                        { RENDER_STATE(D3DRS_ALPHATESTENABLE), 0, false },
                        { RENDER_STATE(D3DRS_FOGENABLE), 0, false },
                        { RENDER_STATE(D3DRS_FORCE_DWORD), 0, NONE },
                    }
                },
                {
#ifdef API_DEBUG
                    "pp_render_stage_pre_alpha_blended",
#endif
                    {
                        { RENDER_STATE(D3DRS_BLENDOP), 0, D3DBLENDOP_ADD },
                        { RENDER_STATE(D3DRS_FILLMODE), 0, D3DFILL_SOLID },
                        { RENDER_STATE(D3DRS_SRCBLEND), 0, D3DBLEND_ONE },
                        { RENDER_STATE(D3DRS_DESTBLEND), 0, D3DBLEND_ONE },
                        { RENDER_STATE(D3DRS_ZENABLE), 0, false },
                        { RENDER_STATE(D3DRS_STENCILENABLE), 0, false },
                        { RENDER_STATE(D3DRS_COLORWRITEENABLE), 0, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA },
                        { RENDER_STATE(D3DRS_ALPHABLENDENABLE), 0, false },
                        { RENDER_STATE(D3DRS_ALPHATESTENABLE), 0, false },
                        { RENDER_STATE(D3DRS_FOGENABLE), 0, false },
                        { RENDER_STATE(D3DRS_FORCE_DWORD), 0, NONE },
                    }
                },
                {
#ifdef API_DEBUG
                    "pp_render_stage_pre_hud",
#endif
                    {
                        { RENDER_STATE(D3DRS_BLENDOP), 0, D3DBLENDOP_ADD },
                        { RENDER_STATE(D3DRS_FILLMODE), 0, D3DFILL_SOLID },
                        { RENDER_STATE(D3DRS_SRCBLEND), 0, D3DBLEND_ONE },
                        { RENDER_STATE(D3DRS_DESTBLEND), 0, D3DBLEND_ONE },
                        { RENDER_STATE(D3DRS_ZENABLE), 0, false },
                        { RENDER_STATE(D3DRS_STENCILENABLE), 0, false },
                        { RENDER_STATE(D3DRS_COLORWRITEENABLE), 0, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA },
                        { RENDER_STATE(D3DRS_ALPHABLENDENABLE), 0, false },
                        { RENDER_STATE(D3DRS_FORCE_DWORD), 0, NONE },
                    }
                },
                {
#ifdef API_DEBUG
                    "pp_render_stage_pre_menu",
#endif
                    {
                        { RENDER_STATE(D3DRS_BLENDOP), 0, D3DBLENDOP_ADD },
                        { RENDER_STATE(D3DRS_FILLMODE), 0, D3DFILL_SOLID },
                        { RENDER_STATE(D3DRS_CULLMODE), 0, D3DCULL_CCW },
                        { RENDER_STATE(D3DRS_SRCBLEND), 0, D3DBLEND_ONE },
                        { RENDER_STATE(D3DRS_DESTBLEND), 0, D3DBLEND_ONE },
                        { RENDER_STATE(D3DRS_ZENABLE), 0, false },
                        { RENDER_STATE(D3DRS_STENCILENABLE), 0, false },
                        { RENDER_STATE(D3DRS_COLORWRITEENABLE), 0, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA },
                        { RENDER_STATE(D3DRS_ALPHABLENDENABLE), 0, false },
                        { RENDER_STATE(D3DRS_ALPHATESTENABLE), 0, false },
                        { RENDER_STATE(D3DRS_FORCE_DWORD), 0, NONE },
                    }
                },
                {
#ifdef API_DEBUG
                    "pp_render_stage_post_menu",
#endif
                    {
                        { RENDER_STATE(D3DRS_BLENDOP), 0, D3DBLENDOP_ADD },
                        { RENDER_STATE(D3DRS_FILLMODE), 0, D3DFILL_SOLID },
                        { RENDER_STATE(D3DRS_CULLMODE), 0, D3DCULL_CCW },
                        { RENDER_STATE(D3DRS_SRCBLEND), 0, D3DBLEND_ONE },
                        { RENDER_STATE(D3DRS_DESTBLEND), 0, D3DBLEND_ONE },
                        { RENDER_STATE(D3DRS_ZENABLE), 0, false },
                        { RENDER_STATE(D3DRS_STENCILENABLE), 0, false },
                        { RENDER_STATE(D3DRS_COLORWRITEENABLE), 0, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA },
                        { RENDER_STATE(D3DRS_ALPHABLENDENABLE), 0, false },
                        { RENDER_STATE(D3DRS_ALPHATESTENABLE), 0, false },
                        { RENDER_STATE(D3DRS_FORCE_DWORD), 0, NONE },
                    }
                },
                {
#ifdef API_DEBUG
                    "pp_render_stage_blur",
#endif
                    {
                        { RENDER_STATE(D3DRS_BLENDOP), 0, D3DBLENDOP_ADD },
                        { RENDER_STATE(D3DRS_FILLMODE), 0, D3DFILL_SOLID },
                        { RENDER_STATE(D3DRS_SRCBLEND), 0, D3DBLEND_ONE },
                        { RENDER_STATE(D3DRS_DESTBLEND), 0, D3DBLEND_ONE },
                        { RENDER_STATE(D3DRS_ZENABLE), 0, false },
                        { RENDER_STATE(D3DRS_STENCILENABLE), 0, false },
                        { RENDER_STATE(D3DRS_COLORWRITEENABLE), 0, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA },
                        { RENDER_STATE(D3DRS_ALPHABLENDENABLE), 0, false },
                        { RENDER_STATE(D3DRS_ALPHATESTENABLE), 0, false },
                        { RENDER_STATE(D3DRS_FOGENABLE), 0, false },
                        { RENDER_STATE(D3DRS_DEPTHBIAS), 0, 0 },
                        { RENDER_STATE(D3DRS_SLOPESCALEDEPTHBIAS), 0, 0 },
                        { RENDER_STATE(D3DRS_FORCE_DWORD), 0, NONE },
                    }
                },
            };
            static s_component_toggle<IPostProcessingComponent> g_postprocess_subsystems[] =
            {
                { false, &Fade::c_system_fade::Instance() },
                { false, &MotionBlur::c_system_motionblur::Instance() },
                { false, &Bloom::c_system_bloom::Instance() },
                { false, &FXAA::c_system_fxaa::Instance() },
                { false, &Generic::Internal::c_system_internal::Instance() },
                { false, &Generic::External::c_system_external::Instance() }
            };

            static s_component_toggle_ref<IPostProcessingCacheComponent> g_postprocess_cache_subsystems[] =
            {
                { 2, &Bloom::c_system_bloom::Instance() },
                { 4, &Generic::Internal::c_system_internal::Instance() }
            };

            static s_component_toggle_ref<IPostProcessingUpdatable> g_postprocess_updatable_subsystems[] =
            {
                { 1, &MotionBlur::c_system_motionblur::Instance() },
                { 2, &Bloom::c_system_bloom::Instance() },
                { 4, &Generic::Internal::c_system_internal::Instance() },
                { 5, &Generic::External::c_system_external::Instance() }
            };

            static s_component_toggle_ref<IPostProcessingRenderable> g_postprocess_renderable_subsystems[] =
            {
                { 1, &MotionBlur::c_system_motionblur::Instance() },
                { 2, &Bloom::c_system_bloom::Instance() },
                { 3, &FXAA::c_system_fxaa::Instance() },
                { 4, &Generic::Internal::c_system_internal::Instance() },
                { 5, &Generic::External::c_system_external::Instance() }
            };

            void s_render_state_manager::StoreAndSet(IDirect3DDevice9* render_device)
            {
                auto current = &m_states[0];
                while (current && current->m_state != D3DRS_FORCE_DWORD)
                {
                    render_device->GetRenderState(current->m_state, &current->m_stored_value);
                    render_device->SetRenderState(current->m_state, current->m_set_value);
                    current++;
                }
            }

            void s_render_state_manager::RestoreStates(IDirect3DDevice9* render_device)
            {
                auto current = &m_states[0];
                while (current && (current->m_state != D3DRS_FORCE_DWORD))
                {
                    render_device->SetRenderState(current->m_state, current->m_stored_value);
                    current++;
                }
            }

            void Initialize()
            {
                // if the main post processing component failed to initialize, return
                c_post_processing_main::Instance().Initialize();

                // initialise all of the subsystems
                for (auto& subsystem : g_postprocess_subsystems)
                {
                    subsystem.m_component->Initialize();
                }
            }

            void Dispose()
            {
                // dispose of all the subsystems
                for (auto& subsystem : g_postprocess_subsystems)
                {
                    subsystem.m_component->Dispose();
                }

                // dispose of the main post processing component and quad manager
                c_post_processing_main::Instance().Dispose();
            }

            void Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
            {
                if (CMDLINE_GET_PARAM(no_os_gfx).ParameterSet())
                {
                    return;
                }

                // initialize the global resources in the main post processing component
                // if this fails, return
                c_post_processing_main::Instance().InitializeResources_Base(pDevice, pParameters);

                YELO_ASSERT_DISPLAY(c_post_processing_main::Instance().IsReady(), "The main post processing component failed to initialise it's resources");

                if (!c_post_processing_main::Instance().IsReady())
                {
                    return;
                }

                // initialise the subsystems
                for (auto& subsystem : g_postprocess_subsystems)
                {
                    subsystem.m_component->InitializeResources_Base(pDevice, pParameters);
                    subsystem.is_ready = subsystem.m_component->IsReady();
                }

                c_post_processing_main::Instance().Globals().quad_collection.CreateQuadBuffers(*pDevice, c_post_processing_main::Instance().Globals().screen_dimensions);
            }

            void OnLostDevice()
            {
                // run device lost logic for all components
                for (auto& subsystem : g_postprocess_subsystems)
                {
                    subsystem.m_component->OnLostDevice_Base();
                    subsystem.is_ready = subsystem.m_component->IsReady();
                }

                // update the ready states of cache components
                for (auto& cache_subsystem : g_postprocess_cache_subsystems)
                {
                    g_postprocess_subsystems[cache_subsystem.component_index].is_ready = cache_subsystem.m_component->IsReady();
                }

                // run device lost logic for the main system resources
                c_post_processing_main::Instance().OnLostDevice_Base();

                c_post_processing_main::Instance().Globals().quad_collection.DestroyQuadBuffers();
            }

            void OnResetDevice(D3DPRESENT_PARAMETERS* pParameters)
            {
                if (CMDLINE_GET_PARAM(no_os_gfx).ParameterSet())
                {
                    return;
                }

                // run device reset logic for the main system resources
                c_post_processing_main::Instance().OnResetDevice_Base(pParameters);

                if (c_post_processing_main::Instance().IsUnloaded())
                {
                    return;
                }

                YELO_ASSERT_DISPLAY(c_post_processing_main::Instance().IsReady(), "The main post processing component failed to be reset");

                if (!c_post_processing_main::Instance().IsReady())
                {
                    return;
                }

                // run device reset logic for normal components
                for (auto& subsystem : g_postprocess_subsystems)
                {
                    subsystem.m_component->OnResetDevice_Base(pParameters);
                    subsystem.is_ready = subsystem.m_component->IsReady();
                }

                // update the ready states of cache components
                for (auto& cache_subsystem : g_postprocess_cache_subsystems)
                {
                    g_postprocess_subsystems[cache_subsystem.component_index].is_ready = cache_subsystem.m_component->IsReady();
                }

                c_post_processing_main::Instance().Globals().quad_collection.CreateQuadBuffers(*DX9::Direct3DDevice(), c_post_processing_main::Instance().Globals().screen_dimensions);
            }

            void Render() {}

            void Release()
            {
                // release normal component resources
                for (auto& subsystem : g_postprocess_subsystems)
                {
                    subsystem.m_component->ReleaseResources_Base();
                }

                // release the main system resources
                c_post_processing_main::Instance().ReleaseResources_Base();

                c_post_processing_main::Instance().Globals().quad_collection.DestroyQuadBuffers();
            }

            void InitializeForNewMap()
            {
                if (CMDLINE_GET_PARAM(no_os_gfx).ParameterSet())
                {
                    return;
                }

                // initialize cache dependencies in the main post processing component
                c_post_processing_main::Instance().Initialize_Cache();

                // initialize cache based components
                for (auto i = 0; i < NUMBEROF(g_postprocess_cache_subsystems); i++)
                {
                    g_postprocess_cache_subsystems[i].m_component->Initialize_Cache();
                }

                YELO_ASSERT_DISPLAY(c_post_processing_main::Instance().IsReady(), "The main post processing component failed to be initialised for a new map");
                if (!c_post_processing_main::Instance().IsReady())
                {
                    return;
                }

                // runs resource initialization for the cache components
                for (auto i = 0; i < NUMBEROF(g_postprocess_cache_subsystems); i++)
                {
                    g_postprocess_cache_subsystems[i].m_component->InitializeResources_Cache();
                    g_postprocess_subsystems[g_postprocess_cache_subsystems[i].component_index].is_ready = g_postprocess_cache_subsystems[i].m_component->IsReady();
                }

                c_post_processing_main::Instance().Globals().quad_collection.CreateQuadBuffers(*DX9::Direct3DDevice(), c_post_processing_main::Instance().Globals().screen_dimensions);
            }

            void DisposeFromOldMap()
            {
                // release all cache component resources
                for (auto i = 0; i < NUMBEROF(g_postprocess_cache_subsystems); i++)
                {
                    g_postprocess_cache_subsystems[i].m_component->ReleaseResources_Cache();
                }

                // release cache dependent resources of the main system
                c_post_processing_main::Instance().ReleaseResources_Cache();

                // dispose of cache components
                for (auto i = 0; i < NUMBEROF(g_postprocess_cache_subsystems); i++)
                {
                    g_postprocess_cache_subsystems[i].m_component->Dispose_Cache();
                }

                c_post_processing_main::Instance().Dispose_Cache();

                c_post_processing_main::Instance().Globals().quad_collection.DestroyQuadBuffers();
            }

            void Update(real delta_time)
            {
                // update global values
                c_post_processing_main::Instance().Update(delta_time);

                // update subsystems
                for (auto i = 0; i < NUMBEROF(g_postprocess_updatable_subsystems); i++)
                {
                    g_postprocess_updatable_subsystems[i].m_component->Update(delta_time);
                }
            }

            void RenderPreAlphaBlended()
            {
                c_post_processing_main::Instance().PollUpdate();

                RenderSystems(Enums::_postprocess_render_stage_pre_blur);
                RenderSystems(Enums::_postprocess_render_stage_blur);
                RenderSystems(Enums::_postprocess_render_stage_pre_alpha_blended);
            }

            void RenderPreHUD()
            {
                RenderSystems(Enums::_postprocess_render_stage_pre_hud);
            }

            void RenderPreMenu()
            {
                RenderSystems(Enums::_postprocess_render_stage_pre_ui);
            }

            void RenderPostMenu()
            {
                RenderSystems(Enums::_postprocess_render_stage_post_ui);
            }

            void LoadSystem()
            {
                if (CMDLINE_GET_PARAM(no_os_gfx).ParameterSet())
                {
                    return;
                }

                // the system has been loaded already
                if (!c_post_processing_main::Instance().IsUnloaded())
                {
                    return;
                }

                // load the main post processing component
                c_post_processing_main::Instance().Load();

                YELO_ASSERT_DISPLAY(c_post_processing_main::Instance().IsReady(), "The main post processing component failed to be loaded");

                if (!c_post_processing_main::Instance().IsReady())
                {
                    return;
                }

                // load all of the subsystems
                for (auto i = 0; i < NUMBEROF(g_postprocess_subsystems); i++)
                {
                    if (g_postprocess_subsystems[i].m_component->IsUnloaded())
                    {
                        g_postprocess_subsystems[i].m_component->Load();
                        g_postprocess_subsystems[i].is_ready = g_postprocess_subsystems[i].m_component->IsReady();
                    }
                }

                c_post_processing_main::Instance().Globals().quad_collection.CreateQuadBuffers(*DX9::Direct3DDevice(), c_post_processing_main::Instance().Globals().screen_dimensions);
            }

            void UnloadSystem()
            {
                // return if the system is already unloaded
                if (c_post_processing_main::Instance().IsUnloaded())
                {
                    return;
                }

                // unload all of the subsystems
                for (auto i = 0; i < NUMBEROF(g_postprocess_subsystems); i++)
                {
                    if (!g_postprocess_subsystems[i].m_component->IsUnloaded())
                    {
                        g_postprocess_subsystems[i].m_component->Unload();
                        g_postprocess_subsystems[i].is_ready = g_postprocess_subsystems[i].m_component->IsReady();
                    }
                }

                // unload the main post process components resources
                c_post_processing_main::Instance().Unload();

                c_post_processing_main::Instance().Globals().quad_collection.DestroyQuadBuffers();
            }

            void RenderSystems(Enums::postprocess_render_stage render_stage)
            {
                // return if the system is not ready
                if (!c_post_processing_main::Instance().IsReady())
                {
                    return;
                }
                // return if the engine is rendering a reflection buffer
                if (Render::IsRenderingReflection())
                {
                    return;
                }

                auto render_device = c_post_processing_main::Instance().Globals().render_device;

                // reset the render targets so that halos primary buffer is the scene texture
                c_post_processing_main::Instance().Globals().scene_buffer_chain.ResetTargets();

                // set the devices vertex and index sources to the quad buffers
                auto hr = c_post_processing_main::Instance().Globals().quad_collection.SetRenderDeviceBuffers(*render_device);
                if (SUCCEEDED(hr))
                {
                    // store and set the necessary render states for this render stage
                    g_render_state_managers[render_stage].StoreAndSet(render_device);
                    {
                        // render the subsystems that are ready
                        auto effects_applied = false;
                        for (auto i = 0; i < NUMBEROF(g_postprocess_renderable_subsystems); i++)
                        {
                            if (g_postprocess_subsystems[g_postprocess_renderable_subsystems[i].component_index].is_ready)
                            {
                                effects_applied |= g_postprocess_renderable_subsystems[i].m_component->Render(render_stage);
                            }
                        }
                        if (effects_applied)
                        {
                            // it's probable that the last surface rendered to isn't halos primary buffer
                            // in that case, stretchrect the result to the primary buffer
                            if (Render::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface != c_post_processing_main::Instance().Globals().scene_buffer_chain.GetSceneSurface())
                            {
                                render_device->StretchRect(
                                    c_post_processing_main::Instance().Globals().scene_buffer_chain.GetSceneSurface(),
                                    nullptr,
                                    Render::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface,
                                    nullptr,
                                    D3DTEXF_NONE);
                            }
                        }
                    }
                    // restore the render states
                    g_render_state_managers[render_stage].RestoreStates(render_device);

                    // reset the devices render target to halos primary buffer
                    render_device->SetRenderTarget(0, Render::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface);
                }
            }
        }
    }
}
#endif
