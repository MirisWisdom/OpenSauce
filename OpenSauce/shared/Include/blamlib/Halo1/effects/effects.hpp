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
			// BIT(1) - attached to object or loops ?
			// BIT(2) - loop stopped with event
			// BIT(3) - loop stopped without event
			// BIT(4) - ?
			// BIT(5) - ?
			// BIT(6) - nonviolent
			word_flags flags;						// 0x2
			datum_index definition_index;			// 0x4
			struct { // Enums::object_function_reference - 1
				int16 primary_scale;				// 0x8 a
				int16 secondary_scale;				// 0xA b
				int16 change_color;					// 0xC
			}object_function_indexes;
			PAD16;
			s_scenario_location location;			// 0x10
			real_rgb_color color;					// 0x18 defaults to white
			real_vector3d transitional_velocity;	// 0x24
			//////////////////////////////////////////////////////////////////////////
			// no code which creates effects seems to actually set this struct up
			// seems to be related to particle "radius animation", calculates scale factor?
			PAD(0, 12);
			// 0x30 UNKNOWN_TYPE(int32)
			// 0x34 void (PLATFORM_API* )(__out real_vector3d&, __out real_point3d&, ?) // last arg is the value at 0x30
			// 0x38 ?
			//////////////////////////////////////////////////////////////////////////
			datum_index object_index;				// 0x3C object this effect was created on
			datum_index attached_object_index;		// 0x40 object attached to this effect
			real scale_a;							// 0x44
			real scale_b;							// 0x48
			int16 local_player_index;				// 0x4C
			int16 event_index;						// 0x4E
			real time;								// 0x50
			real event_delay_time;					// 0x54
			UNKNOWN_TYPE(real);						// 0x58 last percentage (time / event_delay_time)
			datum_index location_datum_indices[Enums::k_maximum_effect_locations_count];	// 0x5C
			byte particles_counts[Enums::k_maximum_effect_particles_per_event_count];		// 0xDC
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