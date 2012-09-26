/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum global_game_team : _enum
		{
			_global_game_team_default,
			_global_game_team_player,
			_global_game_team_human,
			_global_game_team_covenant,
			_global_game_team_flood,
			_global_game_team_sentinel,
			_global_game_team_unused6,
			_global_game_team_unused7,
			_global_game_team_unused8,
			_global_game_team_unused9,

			_global_game_team
		};
	};

	namespace GameState
	{
		struct s_game_options
		{
			UNKNOWN_TYPE(int16);
			UNKNOWN_TYPE(int16); // ?, not sure about this field
			UNKNOWN_TYPE(int16); // ?, not sure about this field
			_enum difficulty_level;
			int32 random_seed;
			char map_name[0x7F+1];
			byte unknown[0x7F+1]; // pretty sure this is just an unused string
		}; BOOST_STATIC_ASSERT( sizeof(s_game_options) == 0x10C );

		struct s_game_globals
		{
			bool map_loaded;
			bool active;
			bool players_are_double_speed;
			bool map_loading_in_progress;
			real map_loading_precentage;
			s_game_options options;
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals) == 0x114 );
	};
};