/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Objects
	{
		struct s_object_lighting
		{
			real_rgb_color ambient_color;
			int16 distant_light_count;
			PAD16;
			struct {
				real_rgb_color color;
				real_vector3d color_direction;
			}distant_light[2];

			//////////////////////////////////////////////////////////////////////////
			// not tag based, used by the game state
			int16 point_light_count;
			PAD16;
			datum_index point_light_indices[2]; // light_data
			//////////////////////////////////////////////////////////////////////////

			real_argb_color reflection_tint;
			real_vector3d shadow_vector;
			real_rgb_color shadow_color;
		}; BOOST_STATIC_ASSERT( sizeof(s_object_lighting) == 0x74 );
	};
};