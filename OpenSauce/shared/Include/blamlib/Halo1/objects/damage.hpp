/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/effects/material_effect_definitions.hpp>
#include <blamlib/Halo1/game/game_allegiance.hpp>

namespace Yelo
{
	namespace Objects
	{
		struct s_damage_data
		{
			datum_index effect_definition_index;			// 0x0
			long_flags flags;								// 0x4
			datum_index responsible_player_index;			// 0x8
			datum_index responsible_unit_index;				// 0xC
			Enums::game_team responsible_units_team;		// 0x10
			PAD16;											// 0x12
			s_scenario_location location;					// 0x14
			real_point3d damage_position;					// 0x1C
			UNKNOWN_TYPE(real_vector3d); // position		// 0x28
			UNKNOWN_TYPE(real_vector3d); // direction?		// 0x34
			real screen_flash_intensity;					// 0x40
			real damage_multiplier;							// 0x44
			UNKNOWN_TYPE(real);								// 0x48
			Enums::material_type material;					// 0x4C
			PAD16;											// 0x4E
			UNKNOWN_TYPE(int32); // only seen this written to, and when it was it was set to a s_projectile_material_response_definition*
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_data) == 0x54 );
	};
};