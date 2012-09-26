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
	};

	namespace GameEngine
	{
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
			s_vehicle_team_remapper team_mappings[2];
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_game_type_remapper) == 0xA0 );
		struct s_vehicle_remapper
		{
			s_vehicle_game_type_remapper game_type_mappings[Enums::_game_engine-1];

			bool processed;
			bool is_teamplay;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_remapper) == 0x3C2 );
	};
};