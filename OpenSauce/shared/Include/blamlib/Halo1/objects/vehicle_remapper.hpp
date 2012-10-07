/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_engine.hpp>
#include <blamlib/Halo1/game/players.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum e_vehicle_set
		{
			_vehicle_set_default,
			_vehicle_set_none,
			_vehicle_set_warthogs,
			_vehicle_set_ghosts,
			_vehicle_set_scorpions,
			_vehicle_set_rwarthogs,
			_vehicle_set_banshees,
			_vehicle_set_cg_turrets,

			k_number_of_vehicle_sets,

			_vehicle_set_yelo8 = k_number_of_vehicle_sets,
			_vehicle_set_yelo9,
			_vehicle_set_yelo10,

			k_number_of_vehicle_sets_yelo,
		};
	};
	namespace Objects
	{
		struct s_vehicle_set
		{
			unsigned vehicle_set : 4; // e_vehicle_set
			unsigned warthogs : 3;
			unsigned ghosts : 3;
			unsigned scorpions : 3;
			unsigned rwarthogs : 3;
			unsigned banshees : 3;
			unsigned cg_turrets : 3;
			unsigned yelo8 : 3;
			unsigned yelo9 : 3;
			unsigned yelo10 : 3;
			unsigned pad : 1;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_set) == sizeof(int32) );

		struct s_vehicle_team_remapper
		{
			int32 team_index;
			int32 scenario_datum_index; // vehicle block index
			word_flags multiplayer_spawn_flags;
			PAD16;
			datum_index vehicle_definition_index;

			struct s_datum {
				int32 scenario_datum_index;
				bool spawn; // will this vehicle be spawned?
				byte_flags spawn_flags;
				PAD16;
			}datums[8];
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_team_remapper) == 0x50 );
		struct s_vehicle_game_type_remapper
		{
			s_vehicle_team_remapper team_mappings[Enums::k_number_of_multiplayer_teams];
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_game_type_remapper) == 0xA0 );
		struct s_vehicle_remapper
		{
			s_vehicle_game_type_remapper game_type_mappings[Enums::_game_engine-1];

			bool processed;
			bool is_teamplay;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_remapper) == 0x3C2 );
	};
};