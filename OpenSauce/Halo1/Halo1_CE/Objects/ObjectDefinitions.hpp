/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/items/projectiles.hpp>
#include <blamlib/Halo1/objects/object_definitions.hpp>
#include <blamlib/Halo1/objects/object_structures.hpp>

namespace Yelo
{
	namespace Objects
	{
		struct s_scenery_data
		{
			long_flags flags; // FLAG(0) - playing animation
		}; BOOST_STATIC_ASSERT( sizeof(s_scenery_data) == (Enums::k_object_size_scenery - Enums::k_object_size_object) );


		struct s_sound_scenery_data
		{
			UNUSED_TYPE(int32); // probably a long_flags field
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_scenery_data) == (Enums::k_object_size_sound_scenery - Enums::k_object_size_object) );


		struct s_placeholder_data
		{
			UNUSED_TYPE(int32);
			UNUSED_TYPE(int32);
		}; BOOST_STATIC_ASSERT( sizeof(s_placeholder_data) == (Enums::k_object_size_placeholder - Enums::k_object_size_object) );




		struct s_scenery_datum
		{
			s_object_data object;
			s_scenery_data scenery;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenery_datum) == Enums::k_object_size_scenery );

		struct s_sound_scenery_datum
		{
			s_object_data object;
			s_sound_scenery_data sound_scenery;
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_scenery_datum) == Enums::k_object_size_sound_scenery );

		struct s_placeholder_datum
		{
			s_object_data object;
			s_placeholder_data placeholder;
		}; BOOST_STATIC_ASSERT( sizeof(s_placeholder_datum) == Enums::k_object_size_placeholder );
	};
};