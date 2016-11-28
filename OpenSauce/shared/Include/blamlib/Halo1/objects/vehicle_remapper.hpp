/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/players.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum e_vehicle_remapper_vehicle
		{
			_vehicle_remapper_vehicle_warthog,
			_vehicle_remapper_vehicle_ghost,
			_vehicle_remapper_vehicle_scorpion,
			_vehicle_remapper_vehicle_rwarthog,
			_vehicle_remapper_vehicle_banshee,
			_vehicle_remapper_vehicle_cg_turret,

			k_number_of_vehicle_remapper_vehicles,

			_vehicle_remapper_vehicle_custom1 = k_number_of_vehicle_remapper_vehicles,
			_vehicle_remapper_vehicle_custom2,
			_vehicle_remapper_vehicle_custom3,

			k_number_of_vehicle_remapper_vehicles_yelo,
		};

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
			_vehicle_set_custom,

			k_number_of_vehicle_sets,

			_vehicle_set_custom1 = k_number_of_vehicle_sets,
			_vehicle_set_custom2,
			_vehicle_set_custom3,

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
			unsigned custom1 : 3;
			unsigned custom2 : 3;
			unsigned custom3 : 3;
			unsigned pad : 1;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_set) == sizeof(int32) );

		//////////////////////////////////////////////////////////////////////////
		// This is still a WIP
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
		//////////////////////////////////////////////////////////////////////////
		struct s_vehicle_remapper
		{
			s_vehicle_team_remapper team_mappings[Enums::k_number_of_multiplayer_teams];
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_remapper) == 0xA0 );
		struct s_vehicle_remapper_globals
		{
			s_vehicle_remapper vehicle_mappings[Enums::k_number_of_vehicle_remapper_vehicles];

			bool processed;
			bool is_teamplay;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_remapper_globals) == 0x3C2 );
	};
};