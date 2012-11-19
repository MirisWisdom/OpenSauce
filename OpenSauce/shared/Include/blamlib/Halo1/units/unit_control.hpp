/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Flags
	{
		//NUMBER_OF_UNIT_ANIMATION_STATES = 7
		//NUMBER_OF_UNIT_AIMING_SPEEDS = 2

		enum unit_control_flags : word_flags
		{
			_unit_control_crouch_bit,
			_unit_control_jump_bit,
			_unit_control_user1_bit,
			_unit_control_user2_bit,
			_unit_control_light_bit,
			_unit_control_exact_facing_bit,
			_unit_control_action_bit,
			_unit_control_equipment_bit,

			_unit_control_look_dont_turn_bit,
			_unit_control_force_alert_bit,
			_unit_control_reload_bit,
			_unit_control_primary_trigger_bit,
			_unit_control_secondary_trigger_bit,
			_unit_control_grenade_bit,
			_unit_control_swap_weapon_bit,

			k_number_of_unit_control_flags, // NUMBER_OF_UNIT_CONTROL_FLAGS
		};
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