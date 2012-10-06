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
		enum playback_event_type {
			_playback_event_type,

			//NUMBER_OF_UNIT_ANIMATION_STATES = 7
			//NUMBER_OF_UNIT_AIMING_SPEEDS = 2
			//MAXIMUM_WEAPONS_PER_UNIT = 4
			//NUMBER_OF_UNIT_GRENADE_TYPES = 2

			//1 animation_state_event_data		: byte
			//1 aiming_speed_event_data			: byte
			//2 control_flags_event_data		: short
			//2 weapon_index_event_data			: short
			//8 throttle_event_data				: long, long
			//2 vector_char_difference_data		: byte, byte
			//4 vector_short_difference_data	: short, short

			//4 animation_event_v1				: short, short
				//6 animation_state_set_event_v1	: byte
				//6 aiming_speed_set_event_v1		: byte
				//6 control_flags_set_event_v1		: short
				//6 weapon_index_set_event_v1		: short
				//C throttle_set_event_v1			: long, long
				//10 multi_vector_set_event_v1		: long, long, long
				//C angle_vector_set_event_v1		: long, long

			//unit_control_v1
				//0  byte
				//1  byte
				//2  word
				//4  word
				//-1 word
				//C  real_vector2d
				//1C real_vector3d
				//28 real_vector3d
				//34 real_vector3d
			//unit_control_v2
				//18 long 
			//unit_control_v3
				//6  word
			//unit_control_v4
				//8  word

			// limited by 'NUMBEROF(apply_funcs)' (0x5C)
			//_playback_animation_state_set,8
			//_playback_aiming_speed_set,C
			//_playback_control_flags_set,10
			//_playback_weapon_index_set,14
			//_playback_throttle_set,18

			//NUMBER_OF_CONTROL_VECTORS = 8
			//_playback_vector_char_difference_set,1C
			//_playback_vector_short_difference_set,3C


			//,0
			//,1
			//_playback_v1_animation_state_set,2
			//_playback_v1_aiming_speed_set,3
			//_playback_v1_control_flags_set,4
			//_playback_v1_weapon_index_set,5
			//_playback_v1_throttle_set,6
			//,7
			//,8
			//_playback_v1_facing_vector_set,9
			//_playback_v1_aiming_vector_set,A
			//_playback_v1_looking_vector_set,B
			//_playback_v1_facing_aiming_vector_set,C
			//_playback_v1_facing_aiming_looking_vector_set,F
			//_playback_v1_facing_angles_set,10
			//_playback_v1_facing_aiming_looking_angles_set,16
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