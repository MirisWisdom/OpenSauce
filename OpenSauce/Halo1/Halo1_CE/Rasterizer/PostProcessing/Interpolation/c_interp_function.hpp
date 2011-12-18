/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once
#include "Rasterizer/Rasterizer.hpp"
#include "Game/EngineFunctions.hpp"
#include "Rasterizer/PostProcessing/Interpolation/c_interp_base.hpp"
#include <Blam/Halo1/shader_postprocess_definitions.hpp>

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		template<int C>
		class c_interp_function : public c_interp_base<C>
		{
		protected:
			struct
			{
				TagGroups::s_shader_postprocess_value_animation_function* animation;
				real update_time;
				real animation_time;
			}m_members_function;

		public:
			c_interp_function(TagGroups::s_shader_postprocess_value_animation_function* animation) { m_members_function.animation = animation; }

			void Ctor()
			{
				c_interp_base::Ctor();

				m_members_function.update_time = 0.0f;
				m_members_function.animation_time = 0.0f;
			}

			void Dtor()
			{
				c_interp_base::Dtor();

				m_members_function.animation = NULL;
				m_members_function.update_time = 0.0f;
				m_members_function.animation_time = 0.0f;
			}

			void Update(real delta_time)
			{
				double function_input = 0.0f;

				// if animation_rate != 0.0f then only update the variable when the update rate has been reached.
				bool update = false;
				if(m_members_function.animation->animation_rate != 0.0f)
				{
					m_members_function.update_time += delta_time;
					while(m_members_function.update_time > m_members_function.animation->animation_rate)
					{
						update = true;
						m_members_function.update_time -= m_members_function.animation->animation_rate;
					}
				}
				else
					update = true;

				if(!update)
					return;

				if(m_members_function.animation->animation_duration == 0.0f)
					function_input = 0.0f;
				else
					function_input = Rasterizer::FrameInputs()->elapsed_time / m_members_function.animation->animation_duration;

				for(int32 i = 0; i < C; i++)
				{
					// if the function is noise and we want multichannel noise, get a different value for each channel
					// the use of a 7 here is arbitrary
					if((m_members_function.animation->function == Enums::_periodic_function_noise) &&
						(m_members_function.animation->flags.multichannel_noise_bit))
						function_input += (7 * i) * function_input;

					m_members.values[i] = (real)Engine::Math::PeriodicFunctionEvaluate(
						(Enums::periodic_function)m_members_function.animation->function,
						function_input
					);
					// invert if necessary
					if(m_members_function.animation->flags.inverted_bit)
						m_members.values[i] = 1.0f - m_members.values[i];
				}

				// if we should ignore the alpha channel, set value1 to 1.0f.
				if(m_members_function.animation->flags.ignore_alpha_bit)
					m_members.values[0] = 1.0f;
			}
		};
	};};
};