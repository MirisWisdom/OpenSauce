/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/players.hpp>

namespace Yelo
{
	namespace GameEngine
	{
		struct s_ctf_globals
		{
			real_point3d* team_flag_positions[Enums::k_number_of_multiplayer_teams];
			datum_index team_flag_object_indexes[Enums::k_number_of_multiplayer_teams]; // weapon objects
			int32 team_score[Enums::k_number_of_multiplayer_teams];
			int32 score_limit;
			bool team_flag_is_taken[Enums::k_number_of_multiplayer_teams];
			PAD16;
			uint32 team_flag_is_taken_sound_timer[Enums::k_number_of_multiplayer_teams];
			uint32 flag_swap_timer;
			uint32 flag_failure_sound_timer;
			byte team_with_flag;
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_ctf_globals) == 0x34 );
	};
};