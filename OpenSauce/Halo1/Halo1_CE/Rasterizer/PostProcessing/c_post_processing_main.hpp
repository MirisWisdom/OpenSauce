/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/files/packed_file.hpp>

#include "Rasterizer/RenderTargetChain.hpp"
#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingCacheComponent.hpp"
#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingUpdatable.hpp"
#include "Rasterizer/PostProcessing/c_quad_collection.hpp"

namespace Yelo
{
    namespace TagGroups
    {
        struct s_shader_postprocess_globals;
    }

    namespace DX9
    {
        struct c_gbuffer;
    }

    namespace Enums
    {
        enum pp_component_status : _enum;
    }

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            class c_post_processing_main :
                public IPostProcessingCacheComponent,
                public IPostProcessingUpdatable
            {
                static c_post_processing_main g_post_processing_main;
            public:
                static c_post_processing_main& Instance();

            private:
                struct
                {
                    struct
                    {
                        bool is_ready;
                        bool is_unloaded;
                        bool is_disabled;
                        PAD8;
                    } m_flags;

                    Enums::pp_component_status status;
                    PAD16;
                } m_members;

            public:
                struct s_post_processing_globals
                {
                    struct
                    {
                        bool has_postprocess_globals;
                        PAD24;
                    } m_flags;

                    IDirect3DDevice9* render_device;
                    D3DCAPS9 device_caps;

                    point2d screen_dimensions;
                    real_point2d screen_fov;
                    real_point2d pixel_size;

                    DX9::s_render_target_chain_scene scene_buffer_chain;
                    DX9::s_render_target_chain secondary_buffer_chain;
                    DX9::c_gbuffer* gbuffer;

                    c_packed_file shader_file;
                    TagGroups::s_shader_postprocess_globals* map_postprocess_globals;
                    c_quad_collection quad_collection;

                    D3DXMATRIX ortho_proj_matrix;

                    struct
                    {
                        real_point2d scaled_ratio;
                        real_point2d default_ratio;
                        real_point2d value;
                    } m_hud;

                    struct
                    {
                        real near_clip;
                        real far_clip;
                    } m_clipping;

                    real delta_time;

                    struct
                    {
                        bool player_has_control;
                        bool in_cinematic;
                        bool local_player_in_vehicle;
                        bool local_player_zoomed;
                    } m_activation_variables;
                } m_globals;

                s_post_processing_globals& Globals();
                bool IsReady() override;
                bool IsUnloaded() override;

                void Initialize() override;
                void Dispose() override;

                void InitializeResources_Base(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters) override;
                void OnLostDevice_Base() override;
                void OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters) override;
                void ReleaseResources_Base() override;

                void Unload() override;
                void Load() override;

                void Initialize_Cache() override;
                void Dispose_Cache() override;

            private:
                void GetRenderProperties(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters);

                bool CreateScreenBufferChain();
                void DestroyScreenBufferChain();
                bool CreateSecondaryBufferChain();
                void DestroySecondaryBufferChain();

                void GetPostprocessGlobalsTag();
                void ClearPostprocessGlobalsTag();

                void OpenGlobalShaderFile();
                void CloseGlobalShaderFile();

                void UpdateStatus();

            public:
                void PollUpdate();
            };
        }
    }
}
#endif
