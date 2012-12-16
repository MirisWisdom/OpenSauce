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
			datum_index effect_definition_index;
			long_flags flags;
			datum_index responsible_player_index;
			datum_index responsible_unit_index;
			Enums::game_team responsible_units_team;
			PAD16;
			s_scenario_location location;
			real_vector3d damage_position;
			UNKNOWN_TYPE(real_vector3d); // position
			UNKNOWN_TYPE(real_vector3d); // direction?
			UNKNOWN_TYPE(real); // k
			UNKNOWN_TYPE(real);
			UNKNOWN_TYPE(real);
			Enums::material_type material;
			PAD16;
			UNKNOWN_TYPE(int32); // only seen this written to, and when it was it was set to a s_projectile_material_response_definition*
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_data) == 0x54 );
	};
};