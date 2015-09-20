/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/GBuffer/Effects/c_gbuffer_fullscreen_effect.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
    namespace Rasterizer
    {
        namespace GBuffer
        {
            c_gbuffer_fullscreen_effect::c_gbuffer_fullscreen_effect()
                : m_effect(nullptr) { }

            bool c_gbuffer_fullscreen_effect::IsAvailable()
            {
                return m_effect != nullptr;
            }

            bool c_gbuffer_fullscreen_effect::Create(IDirect3DDevice9& device, const uint32 width, const uint32 height)
            {
                TEXTURE_VERTEX quad[] =
                    {
                        { -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f },
                        { width - 0.5f, -0.5, 1.0f, 1.0f, 1.0f, 0.0f },
                        { -0.5, height - 0.5f, 1.0f, 1.0f, 0.0f, 1.0f },
                        { width - 0.5f, height - 0.5f, 1.0f, 1.0f, 1.0f, 1.0f }
                    };
                memcpy_s(m_vertices, sizeof(m_vertices), quad, sizeof(quad));
                return true;
            }

            void c_gbuffer_fullscreen_effect::Destroy()
            {
                safe_release(m_effect);
            }

            void c_gbuffer_fullscreen_effect::Render(IDirect3DDevice9& device)
            {
                if (!IsAvailable())
                {
                    return;
                }

                PreRender(device, *m_effect);

                device.SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
                m_effect->SetTechnique(GetTechnique());

                UINT pass_count;
                m_effect->Begin(&pass_count, 0);
                for (uint32 pass = 0; pass < pass_count; ++pass)
                {
                    PrePass(device, pass);

                    m_effect->BeginPass(pass);
                    device.DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vertices, sizeof(TEXTURE_VERTEX));
                    m_effect->EndPass();

                    PostPass(device, pass);
                }
                m_effect->End();

                PostRender(device, *m_effect);
            }

            ID3DXEffect& c_gbuffer_fullscreen_effect::GetEffect()
            {
                return *m_effect;
            }

            LPD3DXEFFECT& c_gbuffer_fullscreen_effect::GetEffectPtr()
            {
                return m_effect;
            }
        };
    };
};
#endif
