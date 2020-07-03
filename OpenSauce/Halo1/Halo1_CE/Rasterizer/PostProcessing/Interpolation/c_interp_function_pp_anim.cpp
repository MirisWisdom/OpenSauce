/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Interpolation/c_interp_function_pp_anim.hpp"

#if !PLATFORM_IS_DEDI

#include <blamlib/Halo1/rasterizer/rasterizer.hpp>

#include <YeloLib/Halo1/shaders/shader_postprocess_generic_definitions.hpp>

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            template class c_interp_function_pp_anim<1>;
            template class c_interp_function_pp_anim<2>;
            template class c_interp_function_pp_anim<3>;
            template class c_interp_function_pp_anim<4>;

            template<int ValueCount>
            c_interp_function_pp_anim<ValueCount>::c_interp_function_pp_anim(TagGroups::s_shader_postprocess_value_animation_function* animation)
            {
                m_members_pp_anim.animation = animation;
            }

            template<int ValueCount>
            void c_interp_function_pp_anim<ValueCount>::Ctor()
            {
                c_interp_function::Ctor();
            }

            template<int ValueCount>
            void c_interp_function_pp_anim<ValueCount>::Dtor()
            {
                c_interp_function::Dtor();

                m_members_pp_anim.animation = nullptr;
            }

            template<int ValueCount>
            void c_interp_function_pp_anim<ValueCount>::Update(real delta_time)
            {
                auto function_input = 0.0f;

                // if animation_rate != 0.0f then only update the variable when the update rate has been reached.
                auto update = false;
                if (m_members_pp_anim.animation->animation_rate != 0.0f)
                {
                    m_members_function.update_time += delta_time;
                    while (m_members_function.update_time > m_members_pp_anim.animation->animation_rate)
                    {
                        update = true;
                        m_members_function.update_time -= m_members_pp_anim.animation->animation_rate;
                    }
                }
                else
                {
                    update = true;
                }

                if (!update)
                {
                    return;
                }

                if (m_members_pp_anim.animation->animation_duration == 0.0f)
                {
                    function_input = 0.0f;
                }
                else
                {
                    function_input = CAST(real, Rasterizer::FrameParameters()->elapsed_time / m_members_pp_anim.animation->animation_duration);
                }

                for (int32 i = 0; i < ValueCount; i++)
                {
                    // if the function is noise and we want multichannel noise, get a different value for each channel
                    // the use of a 7 here is arbitrary
                    if ((m_members_pp_anim.animation->function == Enums::_periodic_function_noise) &&
                        (m_members_pp_anim.animation->flags.multichannel_noise_bit))
                    {
                        function_input += (7 * i) * function_input;
                    }

                    m_members.values[i] = blam::periodic_function_evaluate(
                        static_cast<Enums::periodic_function>(m_members_pp_anim.animation->function),
                        function_input
                    );
                    // invert if necessary
                    if (m_members_pp_anim.animation->flags.inverted_bit)
                    {
                        m_members.values[i] = 1.0f - m_members.values[i];
                    }
                }

                // if we should ignore the alpha channel, set value1 to 1.0f.
                if (m_members_pp_anim.animation->flags.ignore_alpha_bit)
                {
                    m_members.values[0] = 1.0f;
                }
            }
        }
    }
}
#endif
