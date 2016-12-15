/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum playback_v1_event {
			_playback_v1_none,					// no apply func
			_playback_v1_end,					// no apply func
			_playback_v1_animation_state_set,
			_playback_v1_aiming_speed_set,
			_playback_v1_control_flags_set,
			_playback_v1_weapon_index_set,
			_playback_v1_throttle_set,
			_playback_v1_vectors_synchronize,	// no apply func
			_playback_v1_vectors_desynchronize, // no apply func

			_playback_v1_facing_vector_set,
			_playback_v1_aiming_vector_set,
			_playback_v1_looking_vector_set,
			//////////////////////////////////////////////////////////////////////////
			// apply multi vector
			_playback_v1_facing_aiming_vector_set,
			_playback_v1_facing_looking_vector_set,
			_playback_v1_aiming_looking_vector_set,
			_playback_v1_facing_aiming_looking_vector_set,
			//////////////////////////////////////////////////////////////////////////
			// apply angle vector
			_playback_v1_facing_angles_set,
			_playback_v1_aiming_angles_set,
			_playback_v1_looking_angles_set,
			_playback_v1_facing_aiming_angles_set,
			_playback_v1_facing_looking_angles_set,
			_playback_v1_aiming_looking_angles_set,
			_playback_v1_facing_aiming_looking_angles_set,

			k_number_of_playback_v1_events,
		};
	};

	namespace Scripting
	{
		//4 animation_event_v1				: short, short
			//6 animation_state_set_event_v1	: byte
			//6 aiming_speed_set_event_v1		: byte
			//6 control_flags_set_event_v1		: short
			//6 weapon_index_set_event_v1		: short
			//C throttle_set_event_v1			: long, long
			//10 multi_vector_set_event_v1		: long, long, long
			//C angle_vector_set_event_v1		: long, long
	};
};