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
		enum game_difficulty_level : _enum
		{
			_game_difficulty_level_easy,
			_game_difficulty_level_normal,
			_game_difficulty_level_hard,
			_game_difficulty_level_impossible,

			k_number_of_game_difficulty_levels
		};
	};

	namespace GameState
	{
		struct s_game_options
		{
			// In Halo 2:
			// 0x0 long_enum game_mode; // where _game_mode_campaign = 1 (and default). k_number_of_game_modes = 6
			// 0x4 _enum game_simulation; // _game_simulation_local is default

			UNKNOWN_TYPE(int32); // never see this referenced or explicitly set. due to memset, this will always be set to zero
			UNKNOWN_TYPE(int16); // never see this referenced besides in game_options_new code where it's set to 0, even after a memset call. highly likely to be an enum field, albeit unused
			Enums::game_difficulty_level difficulty_level;
			int32 game_random_seed;
			// not always the actual path, sometimes just the name, go figure
			char map_name[/*Enums::k_max_tag_name_length*/255+1];
		}; BOOST_STATIC_ASSERT( sizeof(s_game_options) == 0x10C );
	};

	namespace blam
	{
		extern cstring g_game_difficulty_level_names[Enums::k_number_of_game_difficulty_levels];
	};
};