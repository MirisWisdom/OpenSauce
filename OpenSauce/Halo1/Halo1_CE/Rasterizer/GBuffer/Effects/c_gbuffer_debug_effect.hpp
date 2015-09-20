/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Rasterizer/GBuffer/Effects/c_gbuffer_fullscreen_effect.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace GBuffer
        {
            class c_gbuffer_debug_effect : public c_gbuffer_fullscreen_effect
            {
                D3DXHANDLE m_technique_single;
                D3DXHANDLE m_technique_all;
                D3DXHANDLE m_far_clip_handle;
                D3DXHANDLE m_target_handle;

                bool m_depth_set;
                bool m_velocity_set;
                bool m_normals_set;
                bool m_index_set;

                int16 m_debug_target;
                DWORD old_depthbias, old_fillmode, old_srcblend, old_dest_blend, old_zenable, old_zwriteenable, old_stencilenable;

            public:
                explicit c_gbuffer_debug_effect();

                void SetDebugTarget(const int16 target);

                bool IsAvailable() override;
                bool Create(IDirect3DDevice9& device, const uint32 width, const uint32 height) override;
                void Destroy() override;

            private:
                D3DXHANDLE GetTechnique() override;
                void PreRender(IDirect3DDevice9& device, ID3DXEffect& effect) override;
                void PostRender(IDirect3DDevice9& device, ID3DXEffect& effect) override;
            };
        };
    };
};
#endif
