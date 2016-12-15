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
		struct s_camera_track_control_point
		{
			TAG_FIELD(real_vector3d, position);
			TAG_FIELD(real_quaternion, orientation);
			TAG_PAD(int32, 8);
		}; BOOST_STATIC_ASSERT( sizeof(s_camera_track_control_point) == 0x3C ); // max count: 16

		struct s_camera_track_definition
		{
			enum { k_group_tag = 'trak' };

			struct __flags
			{
				TAG_FLAG(unused);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_TBLOCK(control_points, s_camera_track_control_point);
			TAG_PAD(int32, 8);
		}; BOOST_STATIC_ASSERT( sizeof(s_camera_track_definition) == 0x30 ); // max count: 1
	};
};