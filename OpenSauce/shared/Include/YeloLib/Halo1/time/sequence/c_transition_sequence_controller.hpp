/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/math/periodic_functions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>
#include <YeloLib/Halo1/time/interpolation/c_interp_linear.hpp>

namespace Yelo
{
	namespace Time { namespace Sequence
	{
		struct s_sequence_instance_state
		{
			byte m_index;		//!< Current sequence index of the instance.
			byte m_progress;	//!< Current progress through the current sequence (this is a real encoded to a byte).
		};

		struct s_sequence_instance
		{
			bool m_do_animation;													//!< Flag for whether the instance is animating.
			PAD24;

			Time::Interpolation::c_interp_linear<1> m_interpolator;					//!< Linear interpolator for progressing through a sequence over time.
			const TagGroups::s_transition_sequence_block* m_current_sequence_block;	//!< The sequence instances current sequence block.
		};

		template<int C>
		class c_transition_sequence_controller
			abstract
		{
		protected:
			struct
			{
				int16						sequence_instance_count;				//!< The number of sequence instances in the gamestate.
				PAD16;
				s_sequence_instance_state*	sequence_instance_states;				//!< Pointer to the sequence controllers gamestate memory.
			}m_gamestate;

		private:
			s_sequence_instance m_sequence_instances[C];							//!< Array of sequence instances to control.

			virtual void OnTransitionUpdate(const int16 instance, const real progress) = 0;
			virtual void OnSequenceChanged(const int16 instance, const byte index) = 0;

			virtual const TagGroups::s_transition_sequence_block* GetSequenceBlock(const int16 instance, const byte index) = 0;

		public:
			/*!
			 * Constructs and resets the sequence instances
			 */
			virtual void Ctor()
			{
				for(int i = 0; i < NUMBEROF(m_sequence_instances); i++)
				{
					m_sequence_instances[i].m_current_sequence_block = nullptr;
					m_sequence_instances[i].m_do_animation = false;
					m_sequence_instances[i].m_interpolator.Ctor();
				}
			}

			/*!
			 * Destructs and resets the sequence instances
			 */
			virtual void Dtor()
			{
				for(int i = 0; i < NUMBEROF(m_sequence_instances); i++)
				{
					m_sequence_instances[i].m_current_sequence_block = nullptr;
					m_sequence_instances[i].m_do_animation = false;
					m_sequence_instances[i].m_interpolator.Dtor();
				}
			}

			/*!
			 * \param gamestate
			 * Pointer to the controllers gamestate memory.
			 * 
			 * Sets the controllers gamestate memory pointer.
			 */
			void SetGameStateAllocation(s_sequence_instance_state* gamestate)
			{
				m_gamestate.sequence_instance_states = gamestate;
			}

			/*!
			 * \param instance_count
			 * The number of sequence instances to control in the gamestate.
			 * 
			 * Sets the sequence instances to the values stored in the gamestate.
			 */
			void SetToGamestate(const int16 instance_count)
			{
				// update the instance count
				m_gamestate.sequence_instance_count = instance_count;

				for(int i = 0; i < instance_count; i++)
				{
					// get the instance and instance state
					s_sequence_instance& sequence_instance = m_sequence_instances[i];
					s_sequence_instance_state& sequence_instance_state = m_gamestate.sequence_instance_states[i];

					// get the instances current sequence block and determine whether it is animating
					sequence_instance.m_current_sequence_block = GetSequenceBlock(i, sequence_instance_state.m_index);
					if(sequence_instance.m_current_sequence_block->next_sequence_block != NONE)
					{
						sequence_instance.m_do_animation = true;

						// decode the real progress value from the gamestate progress value and set the interpolator
						real progress = (1.0f / 256) * sequence_instance_state.m_progress;
						sequence_instance.m_interpolator.Set(sequence_instance.m_current_sequence_block->transition_period, progress);
					}
					else
						sequence_instance.m_do_animation = false;
				}
			}

			/*!
			 * \param delta
			 * Time in seconds since the last update.
			 * 
			 * Updates the progress of all sequence instances.
			 */
			void Update(const real delta)
			{
				for(int i = 0; i < m_gamestate.sequence_instance_count; i++)
				{
					s_sequence_instance& instance = m_sequence_instances[i];

					if(!instance.m_do_animation)
						continue;

					// update the interpolator and get the current value
					instance.m_interpolator.Update(delta);
					real* interp_value = instance.m_interpolator.GetValues();

					// encode the progress real value into the gamestate
					s_sequence_instance_state& sequence_instance_state = m_gamestate.sequence_instance_states[i];
					sequence_instance_state.m_progress = (byte)(256 * (*interp_value));

					// get the transition function result and update using the result
					real transition_result = blam::transition_function_evaluate((Enums::transition_function)instance.m_current_sequence_block->transition_function, *interp_value);
					OnTransitionUpdate(i, transition_result);

					// if the interpolation time has elapsed, move to the next sequence block or stop the animation
					if(*interp_value >= 1.0f)
					{
						if(instance.m_current_sequence_block->next_sequence_block != NONE)
							SetSequenceIndex(i, (byte)instance.m_current_sequence_block->next_sequence_block);
						else
							instance.m_do_animation = false;
					}
				}
			}

			/*!
			 * \param instance
			 * Index of the sequence instance to change.
			 * 
			 * \param index
			 * Sequence index to change the instance to.
			 * 
			 * Changes the sequence index of a sequence instance.
			 */
			virtual void SetSequenceIndex(const int16 instance, const byte index)
			{
				s_sequence_instance& sequence_instance = m_sequence_instances[instance];
				s_sequence_instance_state& sequence_instance_state = m_gamestate.sequence_instance_states[instance];

				// update the gamestate to the new sequence
				sequence_instance_state.m_index = index;
				sequence_instance_state.m_progress = 0;

				sequence_instance.m_current_sequence_block = GetSequenceBlock(instance, index);

				OnSequenceChanged(instance, index);

				// reset the interpolator and determine whether interpolation is required
				sequence_instance.m_interpolator.Reset();
				if(sequence_instance.m_current_sequence_block->transition_period == 0.0f)
					sequence_instance.m_do_animation = false;
				else
				{
					sequence_instance.m_do_animation = true;
					sequence_instance.m_interpolator.Begin(sequence_instance.m_current_sequence_block->transition_period);
				}
			}
		};
	};};
};