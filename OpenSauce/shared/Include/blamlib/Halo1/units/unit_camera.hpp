/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace TagGroups
	{
		struct s_unit_camera_track
		{
			TAG_FIELD(tag_reference, track, 'trak');
			PAD32; PAD32; PAD32;
		}; BOOST_STATIC_ASSERT(sizeof(s_unit_camera_track) == 0x1C);

		struct s_unit_camera
		{
			TAG_FIELD(tag_string, camera_marker_name);
			TAG_FIELD(tag_string, camera_submerged_marker_name);
			TAG_FIELD(real, pitch_autolevel, "degrees");
			TAG_FIELD(real_bounds, pitch_range, "degrees");
			TAG_TBLOCK(camera_tracks, s_unit_camera_track);
		}; BOOST_STATIC_ASSERT(sizeof(s_unit_camera) == 0x58);
	};
};