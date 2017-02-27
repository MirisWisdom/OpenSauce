/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/players.hpp>
#include <blamlib/Halo1/sound/sound_environment_definitions.hpp>

namespace Yelo
{
	namespace Scenario
	{
		struct s_scenario_player_atmospheric_fog // made up name
		{
			bool is_inside; PAD24;
			UNKNOWN_TYPE(real_vector3d);
			UNKNOWN_TYPE(real_vector3d);
			UNKNOWN_TYPE(real_rgb_color);
			UNKNOWN_TYPE(real);
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_player_atmospheric_fog) == 0x2C );

		struct s_scenario_globals
		{
			int16 current_structure_bsp_index;
			PAD16;
			s_scenario_player_atmospheric_fog player_fog[Enums::k_maximum_number_of_local_players];
			struct {
				bool copy_environment_tag; PAD24; // never seen this set to true
				TagGroups::sound_environment environment;
			}sound;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_globals) == 0x7C );
	};
};