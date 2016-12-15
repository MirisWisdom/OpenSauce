/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

#include <blamlib/Halo1/cutscene/recorded_animation_playback_v1.hpp>
#include <blamlib/Halo1/cutscene/recorded_animation_playback.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct recorded_animation_definition
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(byte, version); // a Enums::e_recorded_animation_version, with +1  (to support NONE)
			TAG_FIELD(sbyte, raw_animation_data);
			TAG_FIELD(sbyte, unit_control_data_version);
			PAD8;
			TAG_FIELD(int16, length_of_animation);
			PAD16;
			PAD32;
			TAG_FIELD(tag_data, event_stream);

			inline byte_enum ToRecordedAnimationVersion() const { return CAST(byte_enum, version) - 1; }
		}; BOOST_STATIC_ASSERT( sizeof(recorded_animation_definition) == 0x40 );
	};

	namespace Scripting
	{
		//unit_control_v1
			//0  byte, animation_state
			//1  byte, aiming_speed
			//2  word, control_flags
			//4  word, weapon_index
			//-1 word
			//C  real_vector2d, throttle
			//1C real_vector3d, facing_vector
			//28 real_vector3d, aiming_vector
			//34 real_vector3d, looking_vector
		//unit_control_v2
			//18 long, primary_trigger
		//unit_control_v3
			//6  word, grenade_index
		//unit_control_v4
			//8  word, zoom_index
	};
};