/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>

#include <blamlib/Halo1/effects/effects_constants.hpp>
#include <blamlib/Halo1/effects/effect_definitions.hpp>
#include <YeloLib/Halo1/effects/effects_constants_yelo.hpp>

namespace Yelo
{
	namespace Effects
	{
		//////////////////////////////////////////////////////////////////////////
		// game-state: effects
		struct s_effect_datum : Memory::s_datum_base
		{
			// BIT(0) - ?
			// BIT(1) - loops
			// BIT(2) - loop stopped with event
			// BIT(3) - loop stopped without event
			// BIT(4) - ?
			// BIT(5) - ?
			// BIT(6) - nonviolent
			word_flags flags;
			datum_index definition_index;
			struct { // Enums::object_function_reference - 1
				int16 primary_scale;	// a
				int16 secondary_scale;	// b
				int16 change_color;
			}object_function_indexes;
			PAD16;
			s_scenario_location location;
			real_rgb_color color;				// defaults to white
			real_vector3d transitional_velocity;
			//////////////////////////////////////////////////////////////////////////
			// no code which creates effects seems to actually set this struct up
			PAD(0, 12);
			// 0x30 UNKNOWN_TYPE(int32)
			// 0x34 void (PLATFORM_API* )(__out real_vector3d&, __out real_point3d&, ?) // last arg is the value at 0x30
			// 0x38 ?
			//////////////////////////////////////////////////////////////////////////
			datum_index object_index;			// object this effect was created on
			datum_index attached_object_index;	// object attached to this effect
			real scale_a, scale_b;
			int16 local_player_index;
			int16 event_index;
			UNKNOWN_TYPE(real);
			real event_delay_time;
			UNKNOWN_TYPE(real);
			datum_index location_datum_indices[Enums::k_maximum_effect_locations_count];
			byte particles_counts[Enums::k_maximum_effect_particles_per_event_count];
		}; BOOST_STATIC_ASSERT( sizeof(s_effect_datum) == 0xFC );
		typedef Memory::DataArray<	s_effect_datum, 
									Enums::k_maximum_number_of_effects_per_map,
									Enums::k_maximum_number_of_effects_per_map_upgrade> 
			effect_data_t;


		//////////////////////////////////////////////////////////////////////////
		// game-state: effect locations
		struct s_effect_location_datum : Memory::s_datum_base
		{
			// if != NONE and the sign bit is set, it's a first person weapon node
			int16 node_index;
			datum_index next_location_index;
			real_matrix4x3 node_matrix;
		}; BOOST_STATIC_ASSERT( sizeof(s_effect_location_datum) == 0x3C );
		typedef Memory::DataArray<	s_effect_location_datum, 
									Enums::k_maximum_number_of_effect_locations_per_map,
									Enums::k_maximum_number_of_effect_locations_per_map_upgrade> 
			effect_location_data_t;
	};
};