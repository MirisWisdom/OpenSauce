/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Rasterizer/GBuffer/c_gbuffer.hpp"
#include "Rasterizer/GBuffer/Effects/c_gbuffer_fullscreen_effect.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace GBuffer
        {
            struct s_gbuffer_output_map;

            class c_gbuffer_rtclear_effect : public c_gbuffer_fullscreen_effect
            {
                D3DXHANDLE m_technique;
                LPDIRECT3DSURFACE9 m_original_target;
                const s_gbuffer_output_map* m_output_map;

            public:
                explicit c_gbuffer_rtclear_effect(const s_gbuffer_output_map& output_map);

                bool IsAvailable() override;
                bool Create(IDirect3DDevice9& device, const uint32 width, const uint32 height) override;
                void Destroy() override;

            private:
                D3DXHANDLE GetTechnique() override;

                void SetState(IDirect3DDevice9& device) override;

                void PreRender(IDirect3DDevice9& device, ID3DXEffect& effect) override;
                void PostRender(IDirect3DDevice9& device, ID3DXEffect& effect) override;
                void PrePass(IDirect3DDevice9& device, const uint32 pass) override;
                void PostPass(IDirect3DDevice9& device, const uint32 pass) override;
            };
        };
    };
};
#endif
