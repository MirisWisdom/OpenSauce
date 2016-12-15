/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_sky_atmospheric_fog
		{
			real_rgb_color color;
			PAD64;
			real_fraction maximum_density;
			real start_distance;
			real opaque_distance;
		}; BOOST_STATIC_ASSERT( sizeof(s_sky_atmospheric_fog) == 0x20 );

		struct s_sky_definition
		{
			enum { k_group_tag = 'sky ' };

			tag_reference model;
			tag_reference animation;
			TAG_PAD(tag_block, 2);
			struct s_ambient_radiosity {
				real_rgb_color color;
				real power;
			}indoor_ambience, outdoor_ambience;
			s_sky_atmospheric_fog outdoor_fog, indoor_fog;
			tag_reference indoor_fog_screen; // 'fog '
			PAD32; // this could be angle 'Global Sky Rotation' (a la Halo 2). TODO: try to find references to this field offset
			TAG_PAD(tag_block,
				1 + // s_sky_shader_function
				1 + // s_sky_animation
				1); // s_sky_light
		}; BOOST_STATIC_ASSERT( sizeof(s_sky_definition) == 0xD0 );
	};
};