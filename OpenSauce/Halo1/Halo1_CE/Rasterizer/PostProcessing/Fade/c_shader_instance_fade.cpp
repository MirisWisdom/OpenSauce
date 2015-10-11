/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Fade/c_shader_instance_fade.hpp"

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"
#include "Rasterizer/PostProcessing/Fade/s_shader_fade_definition.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Fade
            {
                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Sets the fade shader.</summary>
                ///
                /// <param name="definition">[in] The shader definition.</param>
                void c_shader_instance_fade::SetShader(c_shader_postprocess* definition)
                {
                    m_members_fade.definition = CAST_PTR(c_shader_fade*, definition);
                    c_shader_instance::SetShader(definition);
                }

                void c_shader_instance_fade::Ctor()
                {
                    c_shader_instance::Ctor();

                    m_members_fade.definition = nullptr;
                }

                void c_shader_instance_fade::Dtor()
                {
                    c_shader_instance::Dtor();

                    m_members_fade.definition = nullptr;
                }

                /// <summary>Sets the shader instance variables.</summary>
                void c_shader_instance_fade::SetShaderInstanceVariables()
                {
                    auto definition = m_members.definition->GetShaderDefinition<s_shader_fade_definition>();
                    YELO_ASSERT_DISPLAY(definition != nullptr, "Fade shader has no tag definition");

                    auto effect = m_members.definition->GetEffect();
                    YELO_ASSERT_DISPLAY(effect != nullptr, "Fade shader has no valid effect");

                    definition->fade_amount_handle.SetVariable(effect, &m_members_fade.fade_amount, false);
                };

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Renders the fade.</summary>
                ///
                /// <remarks>Custom render function for fading an effects result. The fade effect swaps the
                ///     current target with the scene texture, and re draws it with alpha blending to fade the
                ///     result in/out.</remarks>
                ///
                /// <param name="render_device">[in] The render device.</param>
                /// <param name="quad_instance">[in] The quad instance.</param>
                /// <param name="fade_amount">  The fade amount.</param>
                ///
                /// <returns>S_OK if successful, non-zero if otherwise.</returns>
                HRESULT c_shader_instance_fade::Render(IDirect3DDevice9* render_device, Render::c_quad_instance* quad_instance, real fade_amount)
                {
                    m_members_fade.fade_amount = fade_amount;

                    // set the effect result as the scene texture
                    c_post_processing_main::Instance().Globals().scene_buffer_chain.SetSceneToLast();
                    // set the scene prior to the effect as the render target
                    c_post_processing_main::Instance().Globals().scene_buffer_chain.Flip();

                    // set the fade value to the shader
                    SetShaderInstanceVariables();

                    // set the render state to enable alpha blending
                    render_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                    render_device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
                    render_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                    render_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

                    // render the effect
                    auto hr = GetShader()->Render(render_device, quad_instance);

                    // reset the render state to disable alpha blending
                    render_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
                    render_device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
                    render_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
                    render_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

                    return hr;
                }
            }
        }
    }
}
#endif
