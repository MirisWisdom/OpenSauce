/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_shader_instance_bloom.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/time/interpolation/interpolation.hpp>

#include "Rasterizer/PostProcessing/c_shader_postprocess.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Bloom
            {
                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Sets the shader this class will instance.</summary>
                ///
                /// <param name="definition">[in] The shader this class will instance.</param>
                void c_shader_instance_bloom::SetShader(c_shader_postprocess* definition)
                {
                    m_members_bloom.definition = CAST_PTR(c_shader_bloom*, definition);
                    c_shader_instance::SetShader(definition);
                }

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Sets the bloom shaders parameters to their initial values.</summary>
                ///
                /// <param name="definition">[in] The bloom globals object to initialise the parameters to.</param>
                void c_shader_instance_bloom::SetInitialValues(TagGroups::s_shader_postprocess_globals_bloom& definition)
                {
                    m_members_bloom.m_variable_animation.current_values = definition.bloom;
                    m_members_bloom.m_variable_animation.target_values = m_members_bloom.m_variable_animation.current_values;

                    m_members_bloom.m_variable_animation.size_interp.Reset();
                    m_members_bloom.m_variable_animation.exposure_interp.Reset();
                    m_members_bloom.m_variable_animation.mix_amount_interp.Reset();
                    m_members_bloom.m_variable_animation.minimum_color_interp.Reset();
                    m_members_bloom.m_variable_animation.maximum_color_interp.Reset();
                }

                void c_shader_instance_bloom::Ctor()
                {
                    c_shader_instance::Ctor();

                    m_members_bloom.definition = nullptr;
                    m_members_bloom.globals = nullptr;

                    m_members_bloom.m_variable_animation.size_interp.Reset();
                    m_members_bloom.m_variable_animation.exposure_interp.Reset();
                    m_members_bloom.m_variable_animation.mix_amount_interp.Reset();
                    m_members_bloom.m_variable_animation.minimum_color_interp.Reset();
                    m_members_bloom.m_variable_animation.maximum_color_interp.Reset();
                }

                void c_shader_instance_bloom::Dtor()
                {
                    c_shader_instance::Dtor();

                    m_members_bloom.definition = nullptr;
                    m_members_bloom.globals = nullptr;

                    m_members_bloom.m_variable_animation.size_interp.Reset();
                    m_members_bloom.m_variable_animation.exposure_interp.Reset();
                    m_members_bloom.m_variable_animation.mix_amount_interp.Reset();
                    m_members_bloom.m_variable_animation.minimum_color_interp.Reset();
                    m_members_bloom.m_variable_animation.maximum_color_interp.Reset();
                }

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Updates the interpolation classes of each bloom parameter.</summary>
                ///
                /// <param name="delta_time">The time that has passed in seconds since the last update.</param>
                void c_shader_instance_bloom::UpdateShaderInstance(real delta_time)
                {
                    m_members_bloom.m_variable_animation.size_interp.Update(delta_time);
                    m_members_bloom.m_variable_animation.exposure_interp.Update(delta_time);
                    m_members_bloom.m_variable_animation.mix_amount_interp.Update(delta_time);
                    m_members_bloom.m_variable_animation.minimum_color_interp.Update(delta_time);
                    m_members_bloom.m_variable_animation.maximum_color_interp.Update(delta_time);
                }

                /// <summary>Sets the shaders parameters to the current bloom values.</summary>
                void c_shader_instance_bloom::SetShaderInstanceVariables()
                {
                    auto definition = m_members.definition->GetShaderDefinition<s_shader_bloom_definition>();
                    YELO_ASSERT_DISPLAY(definition != nullptr, "Bloom shader has no tag definition");

                    auto effect = m_members.definition->GetEffect();
                    YELO_ASSERT_DISPLAY(effect != nullptr, "Bloom shader has no valid effect");

                    definition->size_handle.SetVariableInterp(effect,
                                                              &m_members_bloom.m_variable_animation.current_values.size,
                                                              &m_members_bloom.m_variable_animation.target_values.size,
                                                              m_members_bloom.m_variable_animation.size_interp.GetValues());
                    definition->exposure_handle.SetVariableInterp(effect,
                                                                  &m_members_bloom.m_variable_animation.current_values.exposure,
                                                                  &m_members_bloom.m_variable_animation.target_values.exposure,
                                                                  m_members_bloom.m_variable_animation.exposure_interp.GetValues());
                    definition->mix_amount_handle.SetVariableInterp(effect,
                                                                    &m_members_bloom.m_variable_animation.current_values.mix_amount,
                                                                    &m_members_bloom.m_variable_animation.target_values.mix_amount,
                                                                    m_members_bloom.m_variable_animation.mix_amount_interp.GetValues());
                    definition->minimum_color_handle.SetVariableInterp(effect,
                                                                       &m_members_bloom.m_variable_animation.current_values.minimum_color,
                                                                       &m_members_bloom.m_variable_animation.target_values.minimum_color,
                                                                       m_members_bloom.m_variable_animation.minimum_color_interp.GetValues());
                    definition->maximum_color_handle.SetVariableInterp(effect,
                                                                       &m_members_bloom.m_variable_animation.current_values.maximum_color,
                                                                       &m_members_bloom.m_variable_animation.target_values.maximum_color,
                                                                       m_members_bloom.m_variable_animation.maximum_color_interp.GetValues());
                };

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Sets bloom size.</summary>
                ///
                /// <param name="size">       The size.</param>
                /// <param name="change_time">The change time.</param>
                void c_shader_instance_bloom::SetBloomSize(real size, real change_time)
                {
                    real current;
                    Time::Interpolation::InterpolateValues<1>(
                        &m_members_bloom.m_variable_animation.current_values.size,
                        &m_members_bloom.m_variable_animation.target_values.size,
                        m_members_bloom.m_variable_animation.size_interp.GetValues(),
                        &current);

                    m_members_bloom.m_variable_animation.current_values.size = current;
                    m_members_bloom.m_variable_animation.target_values.size = size;
                    m_members_bloom.m_variable_animation.size_interp.Begin(change_time);
                }

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Sets bloom exposure.</summary>
                ///
                /// <param name="exposure">   The exposure.</param>
                /// <param name="change_time">The change time.</param>
                void c_shader_instance_bloom::SetBloomExposure(real exposure, real change_time)
                {
                    real current;
                    Time::Interpolation::InterpolateValues<1>(
                        &m_members_bloom.m_variable_animation.current_values.exposure,
                        &m_members_bloom.m_variable_animation.target_values.exposure,
                        m_members_bloom.m_variable_animation.exposure_interp.GetValues(),
                        &current);

                    m_members_bloom.m_variable_animation.current_values.exposure = current;
                    m_members_bloom.m_variable_animation.target_values.exposure = exposure;
                    m_members_bloom.m_variable_animation.exposure_interp.Begin(change_time);
                }

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Sets bloom mix amount.</summary>
                ///
                /// <param name="mix_amount"> The mix amount.</param>
                /// <param name="change_time">The change time.</param>
                void c_shader_instance_bloom::SetBloomMixAmount(real mix_amount, real change_time)
                {
                    real current;
                    Time::Interpolation::InterpolateValues<1>(
                        &m_members_bloom.m_variable_animation.current_values.mix_amount,
                        &m_members_bloom.m_variable_animation.target_values.mix_amount,
                        m_members_bloom.m_variable_animation.mix_amount_interp.GetValues(),
                        &current);

                    m_members_bloom.m_variable_animation.current_values.mix_amount = current;
                    m_members_bloom.m_variable_animation.target_values.mix_amount = mix_amount;
                    m_members_bloom.m_variable_animation.mix_amount_interp.Begin(change_time);
                }

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Sets bloom minimum color.</summary>
                ///
                /// <param name="minimum_color">The minimum color.</param>
                /// <param name="change_time">  The change time.</param>
                void c_shader_instance_bloom::SetBloomMinimumColor(const real_rgb_color& minimum_color, real change_time)
                {
                    real_rgb_color current;
                    Time::Interpolation::InterpolateValues<3>(
                        &m_members_bloom.m_variable_animation.current_values.minimum_color.red,
                        &m_members_bloom.m_variable_animation.target_values.minimum_color.red,
                        m_members_bloom.m_variable_animation.minimum_color_interp.GetValues(),
                        &current.red);

                    m_members_bloom.m_variable_animation.current_values.minimum_color = current;
                    m_members_bloom.m_variable_animation.target_values.minimum_color = minimum_color;
                    m_members_bloom.m_variable_animation.minimum_color_interp.Begin(change_time);
                }

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Sets bloom maximum color.</summary>
                ///
                /// <param name="maximum_color">The maximum color.</param>
                /// <param name="change_time">  The change time.</param>
                void c_shader_instance_bloom::SetBloomMaximumColor(const real_rgb_color& maximum_color, real change_time)
                {
                    real_rgb_color current;
                    Time::Interpolation::InterpolateValues<3>(
                        &m_members_bloom.m_variable_animation.current_values.maximum_color.red,
                        &m_members_bloom.m_variable_animation.target_values.maximum_color.red,
                        m_members_bloom.m_variable_animation.maximum_color_interp.GetValues(),
                        &current.red);

                    m_members_bloom.m_variable_animation.current_values.maximum_color = current;
                    m_members_bloom.m_variable_animation.target_values.maximum_color = maximum_color;
                    m_members_bloom.m_variable_animation.maximum_color_interp.Begin(change_time);
                }
            }
        }
    }
}
#endif
