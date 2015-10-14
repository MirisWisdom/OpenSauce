/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_shader_instance_motionblur.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/time/interpolation/interpolation.hpp>

#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/s_shader_motionblur_definition.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_shader_motionblur.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace MotionBlur
            {
                void c_shader_instance_motionblur::SetShader(c_shader_postprocess* definition)
                {
                    m_members_motionblur.definition = CAST_PTR(c_shader_motionblur*, definition);
                    c_shader_instance::SetShader(definition);
                }

                real& c_shader_instance_motionblur::BlurAmount()
                {
                    return m_members_motionblur.blur_amount;
                }

                void c_shader_instance_motionblur::Ctor()
                {
                    c_shader_instance::Ctor();

                    m_members_motionblur.m_flags.vignette_enabled_prev = false;
                    m_members_motionblur.m_flags.vignette_enabled = false;

                    m_members_motionblur.definition = nullptr;

                    m_members_motionblur.start_vignette = 0.0f;
                    m_members_motionblur.target_vignette = 0.0f;
                    m_members_motionblur.vignette_interp.Ctor();
                }

                void c_shader_instance_motionblur::Dtor()
                {
                    c_shader_instance::Dtor();

                    m_members_motionblur.m_flags.vignette_enabled_prev = false;
                    m_members_motionblur.m_flags.vignette_enabled = false;

                    m_members_motionblur.definition = nullptr;

                    m_members_motionblur.start_vignette = 0.0f;
                    m_members_motionblur.target_vignette = 0.0f;
                    m_members_motionblur.vignette_interp.Ctor();
                }

                void c_shader_instance_motionblur::UpdateShaderInstance(real delta_time)
                {
                    m_members_motionblur.m_flags.vignette_enabled_prev = m_members_motionblur.m_flags.vignette_enabled;
                    m_members_motionblur.m_flags.vignette_enabled = c_post_processing_main::Instance().Globals().m_activation_variables.player_has_control &&
                        !c_post_processing_main::Instance().Globals().m_activation_variables.local_player_in_vehicle;

                    if (m_members_motionblur.m_flags.vignette_enabled_prev != m_members_motionblur.m_flags.vignette_enabled)
                    {
                        if (m_members_motionblur.m_flags.vignette_enabled)
                        {
                            Time::Interpolation::InterpolateValues<1>(&m_members_motionblur.start_vignette, &m_members_motionblur.target_vignette,
                                                                      m_members_motionblur.vignette_interp.GetValues(), &m_members_motionblur.start_vignette);
                            m_members_motionblur.target_vignette = 1.0f;
                            m_members_motionblur.vignette_interp.Begin(0.25);
                        }
                        else
                        {
                            Time::Interpolation::InterpolateValues<1>(&m_members_motionblur.start_vignette, &m_members_motionblur.target_vignette,
                                                                      m_members_motionblur.vignette_interp.GetValues(), &m_members_motionblur.start_vignette);
                            m_members_motionblur.target_vignette = 0.0f;
                            m_members_motionblur.vignette_interp.Begin(0.25);
                        }
                    }
                    else
                    {
                        m_members_motionblur.vignette_interp.Update(delta_time);
                    }
                }

                void c_shader_instance_motionblur::SetShaderInstanceVariables()
                {
                    auto definition = m_members_motionblur.definition->GetShaderDefinition<s_shader_motionblur_definition>();
                    YELO_ASSERT_DISPLAY(definition != nullptr, "Motion blur shader class has no valid definition");

                    auto effect = m_members_motionblur.definition->GetEffect();
                    YELO_ASSERT_DISPLAY(effect != nullptr, "Motion blur shader class has no effect");

                    real vignette_amount;
                    Time::Interpolation::InterpolateValues<1>(&m_members_motionblur.start_vignette, &m_members_motionblur.target_vignette, m_members_motionblur.vignette_interp.GetValues(), &vignette_amount);

                    definition->blur_amount_var.SetVariable(effect, &m_members_motionblur.blur_amount);
                    definition->vignette_amount_var.SetVariable(effect, &vignette_amount);
                }
            }
        }
    }
}
#endif
