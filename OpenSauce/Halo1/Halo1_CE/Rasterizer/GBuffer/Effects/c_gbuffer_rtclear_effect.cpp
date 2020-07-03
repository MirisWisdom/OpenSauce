/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/GBuffer/Effects/c_gbuffer_rtclear_effect.hpp"

#if !PLATFORM_IS_DEDI

#include "Rasterizer/GBuffer/Effects/c_gbuffer_effect_factory.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace GBuffer
        {
            c_gbuffer_rtclear_effect::c_gbuffer_rtclear_effect(const s_gbuffer_output_map& output_map)
                : c_gbuffer_fullscreen_effect(),
                  m_technique(nullptr),
                  m_original_target(nullptr),
                  m_output_map(&output_map) { }

            bool c_gbuffer_rtclear_effect::IsAvailable()
            {
                return c_gbuffer_fullscreen_effect::IsAvailable() && m_technique;
            }

            bool c_gbuffer_rtclear_effect::Create(IDirect3DDevice9& device, const uint32 width, const uint32 height)
            {
                c_gbuffer_fullscreen_effect::Create(device, width, height);

                if (!c_gbuffer_effect_factory::Get().CreateEffect(device, &GetEffectPtr(), "GBuffer_Clear"))
                {
                    return false;
                }
                m_technique = c_gbuffer_effect_factory::GetGBufferTechnique(GetEffect(), m_output_map->m_output_count, "Clear", "ALL");

                return IsAvailable();
            }

            void c_gbuffer_rtclear_effect::Destroy()
            {
                c_gbuffer_fullscreen_effect::Destroy();

                m_technique = nullptr;
            }

            D3DXHANDLE c_gbuffer_rtclear_effect::GetTechnique()
            {
                return m_technique;
            }

            void c_gbuffer_rtclear_effect::PreRender(IDirect3DDevice9& device, ID3DXEffect& effect)
            {
                device.GetRenderTarget(0, &m_original_target);

                device.GetRenderState(D3DRS_FILLMODE, &old_fillmode);
                device.GetRenderState(D3DRS_ZWRITEENABLE, &old_zwriteenable);
                device.GetRenderState(D3DRS_ALPHABLENDENABLE, &old_alphablenenable);
                device.GetRenderState(D3DRS_SRCBLEND, &old_srcblend);
                device.GetRenderState(D3DRS_DESTBLEND, &old_destblend);

                device.SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
                device.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
                device.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
                device.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
                device.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
            }

            void c_gbuffer_rtclear_effect::PostRender(IDirect3DDevice9& device, ID3DXEffect& effect)
            {
                device.SetRenderTarget(0, m_original_target);
                safe_release(m_original_target);
            }

            void c_gbuffer_rtclear_effect::PrePass(IDirect3DDevice9& device, const uint32 pass)
            {
                for (uint32 index = 0; index < m_output_map->m_output_count; index++)
                {
                    device.SetRenderTarget(index, m_output_map->m_outputs[pass][index]);
                }
            }

            void c_gbuffer_rtclear_effect::PostPass(IDirect3DDevice9& device, const uint32 pass)
            {
                for (uint32 index = 1; index < m_output_map->m_output_count; index++)
                {
                    device.SetRenderTarget(index, nullptr);
                }
            }
        };
    };
};
#endif
