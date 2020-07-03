/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Rasterizer/DX9/DX9.hpp>

namespace Yelo
{
    namespace Rasterizer
    {
        namespace GBuffer
        {
            class c_gbuffer_fullscreen_effect
            {
                LPD3DXEFFECT m_effect;
                TEXTURE_VERTEX m_vertices[4];

            public:
                explicit c_gbuffer_fullscreen_effect();

                virtual ~c_gbuffer_fullscreen_effect() { }

                virtual bool IsAvailable();
                virtual bool Create(IDirect3DDevice9& device, const uint32 width, const uint32 height);
                virtual void Destroy();
                void Render(IDirect3DDevice9& device);

            protected:
                ID3DXEffect& GetEffect();
                LPD3DXEFFECT& GetEffectPtr();

            private:
                virtual D3DXHANDLE GetTechnique() = 0;

                virtual void SetState(IDirect3DDevice9& device) { };
                
                virtual void PreRender(IDirect3DDevice9& device, ID3DXEffect& effect) { };
                virtual void PostRender(IDirect3DDevice9& device, ID3DXEffect& effect) { };
                virtual void PrePass(IDirect3DDevice9& device, const uint32 pass) { };
                virtual void PostPass(IDirect3DDevice9& device, const uint32 pass) { };
            };
        };
    };
};
#endif
