/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include <blamlib/Halo1/cutscene/recorded_animation_playback_v1.hpp>
#include <blamlib/Halo1/cutscene/recorded_animation_playback.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum e_recorded_animation_version : byte_enum {
			_recorded_animation_version_v1, // see recorded_animation_playback_v1.hpp
			_recorded_animation_version_v2, // see recorded_animation_playback_v1.hpp
			_recorded_animation_version_v3, // see recorded_animation_playback_v1.hpp
			_recorded_animation_version_v4,

			k_recorded_animation_version, // RECORDED_ANIMATION_VERSION
		};
	};

	namespace TagGroups
	{
		struct recorded_animation_definition
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(sbyte, version); // see Enums::e_recorded_animation_version.
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