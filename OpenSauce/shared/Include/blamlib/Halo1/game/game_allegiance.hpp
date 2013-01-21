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
		enum game_team : _enum
		{
#include <cseries/msvc_warning_4341_toggle.hpp>
			_game_team_none = _enum(NONE),
#include <cseries/msvc_warning_4341_toggle.hpp>

			_game_team_default = 0,
			_game_team_player,
			_game_team_human,
			_game_team_covenant,
			_game_team_flood,
			_game_team_sentinel,
			_game_team_unused6,
			_game_team_unused7,
			_game_team_unused8,
			_game_team_unused9,

			k_number_of_game_teams
		};
	};

	namespace GameState
	{
		struct s_game_allegiance_globals
		{
			struct s_allegiance
			{
				Enums::game_team this_team;
				Enums::game_team other_team;
				int16 threshold;
				UNKNOWN_TYPE(int16);	// 0x6
				UNKNOWN_TYPE(bool);		// 0x8
				UNKNOWN_TYPE(bool);		// 0x9
				bool is_broken;			// 0xA
				UNKNOWN_TYPE(bool);		// 0xB
				UNKNOWN_TYPE(bool);		// 0xC
				PAD8;
				int16 incidents_count;	// 0xE
				UNKNOWN_TYPE(int16);	// 0x10
			}; BOOST_STATIC_ASSERT( sizeof(s_allegiance) == 0x12 );

			int16 current_incidents;
			s_allegiance allegiances[8];
			PAD16;
			long_flags ally_mapping_flags[ BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_number_of_game_teams * Enums::k_number_of_game_teams) ]; // 0x94
			long_flags enemy_mapping_flags[ BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_number_of_game_teams * Enums::k_number_of_game_teams) ]; // 0xA4
		}; BOOST_STATIC_ASSERT( sizeof(s_game_allegiance_globals) == 0xB4 );
	};
};