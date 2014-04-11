/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/units/unit_structures.hpp>

namespace Yelo
{
	namespace Objects
	{
		const size_t s_unit_data::k_offset_zoom_level = FIELD_OFFSET(s_unit_datum, unit.zoom_level);
		const size_t s_unit_data::k_offset_desired_zoom_level = FIELD_OFFSET(s_unit_datum, unit.desired_zoom_level);

		const size_t s_unit_data::k_offset_zoom_level_yelo = FIELD_OFFSET(s_unit_datum, unit.zoom_level_yelo);
		const size_t s_unit_data::k_offset_desired_zoom_level_yelo = FIELD_OFFSET(s_unit_datum, unit.desired_zoom_level_yelo);
	};

	namespace blam
	{
		bool PLATFORM_API unit_animation_state_interruptable(const Objects::s_unit_animation_data& animation,
			_enum next_animation_state)
		{
			bool result = true;

			switch (animation.state)
			{
				case Enums::_unit_animation_state_impulse:
				case Enums::_unit_animation_state_melee:
				case Enums::_unit_animation_state_melee_airborne:
				case Enums::_unit_animation_state_throw_grenade:
				case Enums::_unit_animation_state_resurrect_front:
				case Enums::_unit_animation_state_resurrect_back:
				case Enums::_unit_animation_state_leap_start:
				case Enums::_unit_animation_state_leap_melee:
					if (next_animation_state != Enums::_unit_animation_state_unknown23)
						result = false;
					break;

				case Enums::_unit_animation_state_airborne_dead:
				case Enums::_unit_animation_state_landing_dead:
					if (next_animation_state < Enums::_unit_animation_state_airborne_dead || 
						next_animation_state > Enums::_unit_animation_state_landing_dead)
						result = false;
					break;

				case Enums::_unit_animation_state_turn_left:
				case Enums::_unit_animation_state_turn_right:
				case Enums::_unit_animation_state_surprise_front:
				case Enums::_unit_animation_state_surprise_back:
					if (next_animation_state == Enums::_unit_animation_state_idle)
						result = false;
					break;

				case Enums::_unit_animation_state_unknown23:
				case Enums::_unit_animation_state_seat_enter:
				case Enums::_unit_animation_state_seat_exit:
				case Enums::_unit_animation_state_custom_animation:
					result = false;
					break;

				// custom animation states
				case Enums::_unit_animation_state_yelo_seat_board:
				case Enums::_unit_animation_state_yelo_seat_ejection:
					result = false;
					break;

				default:
					break;
			}

			return result;
		}

		bool PLATFORM_API unit_animation_busy(const Objects::s_unit_animation_data& animation)
		{
			bool result = false;

			switch (animation.state)
			{
			case Enums::_unit_animation_state_unknown23:
			case Enums::_unit_animation_state_airborne_dead:
			case Enums::_unit_animation_state_landing_dead:
			case Enums::_unit_animation_state_seat_enter:
			case Enums::_unit_animation_state_seat_exit:
			case Enums::_unit_animation_state_impulse:
			case Enums::_unit_animation_state_melee:
			case Enums::_unit_animation_state_melee_airborne:
			case Enums::_unit_animation_state_melee_continuous:
			case Enums::_unit_animation_state_throw_grenade:
			case Enums::_unit_animation_state_resurrect_front:
			case Enums::_unit_animation_state_resurrect_back:
			case Enums::_unit_animation_state_leap_start:
			case Enums::_unit_animation_state_leap_melee:
				result = true;
				break;

				// custom animation states
			case Enums::_unit_animation_state_yelo_seat_board:
			case Enums::_unit_animation_state_yelo_seat_ejection:
				result = true;
				break;

			default:
				break;
			}

			return result;
		}

		bool PLATFORM_API unit_animation_state_loops(const Objects::s_unit_animation_data& animation)
		{
			bool result = true;

			switch (animation.state)
			{
			case Enums::_unit_animation_state_gesture:
			case Enums::_unit_animation_state_turn_left:
			case Enums::_unit_animation_state_turn_right:
			case Enums::_unit_animation_state_unknown23:
			case Enums::_unit_animation_state_seat_enter:
			case Enums::_unit_animation_state_seat_exit:
			case Enums::_unit_animation_state_custom_animation:
			case Enums::_unit_animation_state_impulse:
			case Enums::_unit_animation_state_melee:
			case Enums::_unit_animation_state_melee_airborne:
			case Enums::_unit_animation_state_throw_grenade:
			case Enums::_unit_animation_state_resurrect_front:
			case Enums::_unit_animation_state_resurrect_back:
			case Enums::_unit_animation_state_leap_start:
			case Enums::_unit_animation_state_leap_melee:
				result = false;
				break;

				// custom animation states
			case Enums::_unit_animation_state_yelo_seat_board:
			case Enums::_unit_animation_state_yelo_seat_ejection:
				result = false;
				break;

			default:
				break;
			}

			return result;
		}

		bool PLATFORM_API unit_animation_weapon_ik(const Objects::s_unit_animation_data& animation)
		{
			bool result = animation.weapon_ik.animation_index == NONE;

			if (animation.replacement_animation_state)
				result = false;

			switch (animation.state)
			{
			case Enums::_unit_animation_state_ready:
			case Enums::_unit_animation_state_put_away:
			case Enums::_unit_animation_state_aim_still:
			case Enums::_unit_animation_state_aim_move:
			case Enums::_unit_animation_state_unknown23:
			case Enums::_unit_animation_state_airborne_dead:
			case Enums::_unit_animation_state_landing_dead:
			case Enums::_unit_animation_state_seat_enter:
			case Enums::_unit_animation_state_seat_exit:
			case Enums::_unit_animation_state_custom_animation:
			case Enums::_unit_animation_state_impulse:
			case Enums::_unit_animation_state_melee:
			case Enums::_unit_animation_state_melee_airborne:
			case Enums::_unit_animation_state_melee_continuous:
			case Enums::_unit_animation_state_throw_grenade:
			case Enums::_unit_animation_state_resurrect_front:
			case Enums::_unit_animation_state_resurrect_back:
			case Enums::_unit_animation_state_leap_start:
			case Enums::_unit_animation_state_leap_airborne:
			case Enums::_unit_animation_state_leap_melee:
				result = false;
				break;

				// custom animation states
			case Enums::_unit_animation_state_yelo_seat_board:
			case Enums::_unit_animation_state_yelo_seat_ejection:
				result = false;
				break;

			default:
				break;
			}

			return result;
		}

		bool PLATFORM_API unit_animation_vehicle_ik(const Objects::s_unit_animation_data& animation)
		{
			bool result = true;

			switch (animation.state)
			{
			case Enums::_unit_animation_state_unknown23:
			case Enums::_unit_animation_state_airborne_dead:
			case Enums::_unit_animation_state_landing_dead:
			case Enums::_unit_animation_state_seat_enter:
			case Enums::_unit_animation_state_seat_exit:
			case Enums::_unit_animation_state_impulse:
			case Enums::_unit_animation_state_resurrect_front:
			case Enums::_unit_animation_state_resurrect_back:
				result = false;
				break;

				// custom animation states
			case Enums::_unit_animation_state_yelo_seat_board:
			case Enums::_unit_animation_state_yelo_seat_ejection:
				result = false;
				break;

			default:
				break;
			}

			return result;
		}
	};
};