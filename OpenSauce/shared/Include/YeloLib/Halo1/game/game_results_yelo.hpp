/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

/*
k_maximum_player_skill_level	127
k_maximum_player_experience		0x3FFFFFFF

k_maximum_teams					16
*/

namespace Yelo
{
	namespace Game
	{
		struct s_game_results_player_statistics
		{
		};
		struct s_game_results_team_statistics
		{
		};
		struct s_game_results_statistics
		{
		};
		struct s_game_results
		{
			bool finalized; PAD24;
		};
		struct s_game_result_globals
		{
			bool recording;
			bool pause;
			bool updating;
			PAD8;
		};
	};
};