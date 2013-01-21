/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			_control_vector_none, // the 'number_of' is 8, so I'm assuming there's a none

			_control_vector_facing,
			_control_vector_aiming,
			_control_vector_looking,
			_control_vector_facing_aiming,
			_control_vector_facing_looking,
			_control_vector_aiming_looking,
			_control_vector_facing_aiming_looking,

			k_number_of_control_vectors,
		};

		enum playback_event {
			_playback_none,					// no apply func
			_playback_end,					// no apply func
			_playback_animation_state_set,
			_playback_aiming_speed_set,
			_playback_control_flags_set,
			_playback_weapon_index_set,
			_playback_throttle_set,

			_playback_vector_char_difference_set,

			_playback_vector_short_difference_set = _playback_vector_char_difference_set + k_number_of_control_vectors,

			k_number_of_playback_events = _playback_vector_short_difference_set + k_number_of_control_vectors,
		};
	};

	namespace Scripting
	{
		//1 animation_state_event_data		: byte
		//1 aiming_speed_event_data			: byte
		//2 control_flags_event_data		: short
		//2 weapon_index_event_data			: short
		//8 throttle_event_data				: long, long
		//2 vector_char_difference_data		: byte, byte
		//4 vector_short_difference_data	: short, short

		struct recorded_animation_playback_header
		{
			byte_enum version : 2;
			byte_enum event_type : 6;
		}; BOOST_STATIC_ASSERT( sizeof(recorded_animation_playback_header) == sizeof(byte_enum) );
	};
};