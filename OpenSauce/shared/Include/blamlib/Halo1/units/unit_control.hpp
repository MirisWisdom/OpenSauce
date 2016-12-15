/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		//_unit_animation_state_asleep
		//_unit_animation_state_alert
		//_unit_animation_state_alert2
		//_unit_animation_state_stand
		//_unit_animation_state_stand2
		//_unit_animation_state_flee
		//_unit_animation_state_flaming
		//NUMBER_OF_UNIT_ANIMATION_STATES = 7

		enum {
			_unit_aiming_speed_alert,
			_unit_aiming_speed_casual, // infection forms use this

			k_number_of_unit_aiming_speeds // NUMBER_OF_UNIT_AIMING_SPEEDS
		};
	};

	namespace Flags
	{
		enum unit_control_flags : word_flags
		{
			_unit_control_crouch_bit,
			_unit_control_jump_bit,
			_unit_control_user1_bit,
			_unit_control_user2_bit,
			_unit_control_light_bit,
			_unit_control_exact_facing_bit,
			_unit_control_action_bit,
			_unit_control_melee_bit,//_unit_control_equipment_bit, // debug strings still list this as 'equipment', but usage is melee

			_unit_control_look_dont_turn_bit,
			_unit_control_force_alert_bit,
			_unit_control_reload_bit,
			_unit_control_primary_trigger_bit,
			_unit_control_secondary_trigger_bit,
			_unit_control_grenade_bit,
			_unit_control_swap_weapon_bit,

			k_number_of_unit_control_flags, // NUMBER_OF_UNIT_CONTROL_FLAGS

			// There's technically room left for one more control flag
			_unit_control_unused_bit = k_number_of_unit_control_flags,

			// If we in fact do use the above unused bit, editor code will need to have their VALID_FLAGS updated to test with this
			k_number_of_unit_control_flags_yelo,
		};
		BOOST_STATIC_ASSERT( k_number_of_unit_control_flags_yelo <= BIT_COUNT(word_flags) );
	};

	namespace Objects
	{
		struct s_unit_control_data
		{
			byte animation_state;
			byte aiming_speed;
			word_flags control_flags;
			int16 weapon_index, grenade_index;
			int16 zoom_index;
			PAD16;
			real_vector3d throttle;
			real primary_trigger;
			real_vector3d facing_vector, aiming_vector, looking_vector;
		}; BOOST_STATIC_ASSERT( sizeof(s_unit_control_data) == 0x40 );
	};
};