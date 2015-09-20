/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/GBuffer/Effects/c_gbuffer_debug_effect.hpp"

#if !PLATFORM_IS_DEDI

#include <blamlib/Halo1/render/render.hpp>
#include <blamlib/Halo1/rasterizer/rasterizer.hpp>
#include "Rasterizer/GBuffer/c_gbuffer.hpp"
#include "Rasterizer/GBuffer/Effects/c_gbuffer_effect_factory.hpp"
#include "Rasterizer/GBuffer.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace GBuffer
        {
            c_gbuffer_debug_effect::c_gbuffer_debug_effect()
                : c_gbuffer_fullscreen_effect(std::vector<D3DRENDERSTATETYPE> { D3DRS_FILLMODE, D3DRS_ZWRITEENABLE, D3DRS_ALPHABLENDENABLE, D3DRS_SRCBLEND, D3DRS_DESTBLEND }),
                  m_technique_single(nullptr),
                  m_technique_all(nullptr),
                  m_far_clip_handle(nullptr),
                  m_target_handle(nullptr),
                  m_depth_set(false),
                  m_velocity_set(false),
                  m_normals_set(false),
                  m_index_set(false),
                  m_debug_target(NONE) {}

            void c_gbuffer_debug_effect::SetDebugTarget(const int16 target)
            {
                m_debug_target = target;
            }

            bool c_gbuffer_debug_effect::IsAvailable()
            {
                return c_gbuffer_fullscreen_effect::IsAvailable() &&
                    m_technique_single &&
                    m_technique_all &&
                    m_far_clip_handle &&
                    m_target_handle &&
                    m_depth_set &&
                    m_velocity_set &&
                    m_normals_set &&
                    m_index_set;
            }

            bool c_gbuffer_debug_effect::Create(IDirect3DDevice9& device, const uint32 width, const uint32 height)
            {
                c_gbuffer_fullscreen_effect::Create(device, width, height);

                if (!c_gbuffer_effect_factory::Get().CreateEffect(device, &GetEffectPtr(), "GBuffer_Debug"))
                {
                    return false;
                }

                auto& effect = GetEffect();
                m_technique_single = effect.GetTechniqueByName("DebugRTSingle");
                m_technique_all = effect.GetTechniqueByName("DebugRTAll");

                m_far_clip_handle = effect.GetParameterByName(nullptr, "FarClipDistance");
                m_target_handle = effect.GetParameterByName(nullptr, "RenderTarget");

                bool variable_used;
                m_depth_set = GetGBuffer().SetDepth(effect, variable_used);
                m_velocity_set = GetGBuffer().SetVelocity(effect, variable_used);
                m_normals_set = GetGBuffer().SetNormals(effect, variable_used);
                m_index_set = GetGBuffer().SetIndex(effect, variable_used);

                return IsAvailable();
            }

            void c_gbuffer_debug_effect::Destroy()
            {
                c_gbuffer_fullscreen_effect::Destroy();

                m_technique_single = nullptr;
                m_technique_all = nullptr;
                m_far_clip_handle = nullptr;
                m_target_handle = nullptr;
                m_depth_set = false;
                m_velocity_set = false;
                m_normals_set = false;
                m_index_set = false;
            }

            D3DXHANDLE c_gbuffer_debug_effect::GetTechnique()
            {
                return m_debug_target == NONE ? m_technique_all : m_technique_single;
            }

            void c_gbuffer_debug_effect::SetState(IDirect3DDevice9& device)
            {
                device.SetRenderState(D3DRS_DEPTHBIAS, 0);
                device.SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
                device.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
                device.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
                device.SetRenderState(D3DRS_ZENABLE, FALSE);
                device.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
                device.SetRenderState(D3DRS_STENCILENABLE, FALSE);
            }

            void c_gbuffer_debug_effect::PreRender(IDirect3DDevice9& device, ID3DXEffect& effect)
            {
                auto& rt = Render::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary];

                device.SetRenderTarget(0, rt.surface);
                effect.SetFloat(m_far_clip_handle, Render::RenderGlobals()->frustum.z_far);
                effect.SetInt(m_target_handle, m_debug_target);
            }
        };
    };
};
#endif
