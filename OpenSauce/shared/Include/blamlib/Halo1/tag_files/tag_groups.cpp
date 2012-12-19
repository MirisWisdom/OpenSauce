/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace TagGroups
	{
#if PLATFORM_IS_EDITOR
		s_tag_field_definition k_tag_field_definitions[] = {
			{ sizeof(tag_string) },
			{ sizeof(sbyte) },
			{ sizeof(int16) },
			{ sizeof(int32) },
			{ sizeof(angle) },
			{ sizeof(tag) },
			{ sizeof(_enum) },
			{ sizeof(long_flags) },
			{ sizeof(word_flags) },
			{ sizeof(byte_flags) },
			{ sizeof(point2d) },
			{ sizeof(rectangle2d) },
			{ sizeof(rgb_color) },
			{ sizeof(argb_color) },

			{ sizeof(real) },
			{ sizeof(real_fraction) },
			{ sizeof(real_point2d) },
			{ sizeof(real_point3d) },
			{ sizeof(real_vector2d) },
			{ sizeof(real_vector3d) },
			{ sizeof(real_quaternion) },
			{ sizeof(real_euler_angles2d) },
			{ sizeof(real_euler_angles3d) },
			{ sizeof(real_plane2d) },
			{ sizeof(real_plane3d) },
			{ sizeof(real_rgb_color) },
			{ sizeof(real_argb_color) },

			{ sizeof(real_rgb_color) },
			{ sizeof(real_argb_color) },

			{ sizeof(short_bounds) },
			{ sizeof(angle_bounds) },
			{ sizeof(real_bounds) },
			{ sizeof(real_fraction_bounds) },

			{ sizeof(tag_reference) },
			{ sizeof(tag_block) },
			{ sizeof(int16) },
			{ sizeof(int32) },
			{ sizeof(tag_data) },

			{ 0 },
			{ 0 },
			{ 0 },
			{ 0 },
			{ 0 },
			{ 0 },
			{ 0 },
		};
		BOOST_STATIC_ASSERT( NUMBEROF(k_tag_field_definitions) == Enums::k_number_of_tag_field_types );
#endif
	};
};