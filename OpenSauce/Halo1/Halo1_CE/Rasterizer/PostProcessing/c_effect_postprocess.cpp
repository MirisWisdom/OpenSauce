/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/c_effect_postprocess.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include "Rasterizer/PostProcessing/c_shader_postprocess.hpp"
#include "Rasterizer/PostProcessing/c_shader_instance.hpp"
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"
#include "Rasterizer/PostProcessing/Fade/c_system_fade.hpp"
#include "Rasterizer/GBuffer.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            void c_effect_postprocess::ClearMembers()
            {
                m_members.effect_definition = nullptr;
                m_members.m_shaders.list = nullptr;
                m_members.m_shaders.count = 0;
            }

            void c_effect_postprocess::SetEffectDefinition(TagGroups::s_effect_postprocess_definition* definition)
            {
                m_members.effect_definition = definition;
            }

            bool c_effect_postprocess::IsValid()
            {
                return m_members.effect_definition->runtime.flags.valid_effect_bit;
            }

            void c_effect_postprocess::Ctor()
            {
                ClearMembers();
            }

            void c_effect_postprocess::Dtor()
            {
                ClearMembers();
            }

            void c_effect_postprocess::SetupEffectPostCreation()
            {
                // determine whether this effect uses the gbuffer at any point
                auto current = m_members.m_shaders.list;
                while (current)
                {
                    m_members.effect_definition->runtime.flags.uses_gbuffer_bit |= current->UsesGBuffer();
                    current = current->GetNext();
                }
            }

            void c_effect_postprocess::AddShaderInstance(c_shader_instance* instance)
            {
                AppendLinkedListNode(m_members.m_shaders.list, instance);
                m_members.m_shaders.count++;
            }

            void c_effect_postprocess::Validate()
            {
                m_members.effect_definition->runtime.flags.valid_effect_bit = ValidateImpl();
            }

            bool c_effect_postprocess::ValidateImpl()
            {
                auto valid = false;
                do
                {
                    if (!m_members.effect_definition)
                    {
                        break;
                    }
                    if (!m_members.m_shaders.list)
                    {
                        break;
                    }
                    if (m_members.m_shaders.count == 0)
                    {
                        break;
                    }

                    valid = true;
                } while (false);

                // the effect is invalid if one of it's shader instances is invalid
                auto current = m_members.m_shaders.list;
                while (current && valid)
                {
                    current->Validate();

                    valid &= current->IsValid();
                    current = current->GetNext();
                }

                return valid;
            }

            void c_effect_postprocess::Update(real delta_time)
            {
                auto current = m_members.m_shaders.list;
                while (current)
                {
                    current->UpdateShaderInstance(delta_time);
                    current = current->GetNext();
                }
            }

            HRESULT c_effect_postprocess::Render(IDirect3DDevice9* render_device, Render::c_quad_instance* render_quad)
            {
                return Render(render_device, render_quad, 1.0f);
            }

            HRESULT c_effect_postprocess::Render(IDirect3DDevice9* render_device, Render::c_quad_instance* render_quad, real fade_amount)
            {
                // if the effect is invalid, return
                if (!IsActive())
                {
                    return E_FAIL;
                }

                // fade amount is zero so do not render the effect
                if (fade_amount == 0.0f)
                {
                    return E_FAIL;
                }

                // set the scene render chain so that the source and scene textures are the same at the start of the effect
                c_post_processing_main::Instance().Globals().scene_buffer_chain.m_first_render = true;
                c_post_processing_main::Instance().Globals().secondary_buffer_chain.m_first_render = true;

                auto hr = E_FAIL;
                // apply each shader in order
                auto current = m_members.m_shaders.list;
                while (current)
                {
                    if (current->IsActive())
                    {
                        // set the shaders values to those in the instance
                        current->SetShaderInstanceVariables();
                        // render the shader using the effects quad instance
                        hr = current->GetShader()->Render(render_device, render_quad);

                        if (FAILED(hr))
                        {
                            break;
                        }
                    }
                    current = current->GetNext();
                }

                // if the fade amount is less than 1.0f, fade the effects result
                if (fade_amount < 1.0f)
                {
                    hr |= Fade::c_system_fade::Instance().FadeCurrentResult(fade_amount);
                }

                // if errors occurred leave the render-target as-is, otherwise swap the targets so that the result of 
                // this effect becomes the scene texture for the next
                if (SUCCEEDED(hr))
                {
                    c_post_processing_main::Instance().Globals().scene_buffer_chain.SetSceneToLast();
                }
                return hr;
            }

            bool c_effect_postprocess::IsActive()
            {
                bool active = m_members.effect_definition->runtime.flags.valid_effect_bit;
                if (m_members.effect_definition->runtime.flags.uses_gbuffer_bit && !DX9::c_gbuffer_system::g_system_enabled)
                {
                    active = false;
                }
                return active;
            }
        }
    }
}
#endif
