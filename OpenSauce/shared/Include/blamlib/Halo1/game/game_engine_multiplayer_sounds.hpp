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
		enum multiplayer_sound : _enum
		{
			_multiplayer_sound_playball,
			_multiplayer_sound_game_over,
			_multiplayer_sound_one_min_to_win,
			_multiplayer_sound_30_secs_to_win,
			_multiplayer_sound_red_team_one_min_to_win,
			_multiplayer_sound_red_team_30_secs_to_win,
			_multiplayer_sound_blue_team_one_min_to_win,
			_multiplayer_sound_blue_team_30_secs_to_win,
			_multiplayer_sound_red_team_flag_taken,
			_multiplayer_sound_red_team_flag_returned,
			_multiplayer_sound_red_team_flag_captured,
			_multiplayer_sound_blue_team_flag_taken,
			_multiplayer_sound_blue_team_flag_returned,
			_multiplayer_sound_blue_team_flag_captured,
			_multiplayer_sound_double_kill,
			_multiplayer_sound_tripple_kill,
			_multiplayer_sound_killtacular,
			_multiplayer_sound_running_riot,
			_multiplayer_sound_killing_spree,
			_multiplayer_sound_oddball,
			_multiplayer_sound_race,
			_multiplayer_sound_slayer,
			_multiplayer_sound_ctf,
			_multiplayer_sound_warthog,
			_multiplayer_sound_ghost,
			_multiplayer_sound_scorpion,
			_multiplayer_sound_countdown_timer,
			_multiplayer_sound_teleporter_activate,
			_multiplayer_sound_flag_failure,
			_multiplayer_sound_countdown_for_respawn,
			_multiplayer_sound_hill_move,
			_multiplayer_sound_player_respawn,
			_multiplayer_sound_koth,
			_multiplayer_sound_odd_ball,
			_multiplayer_sound_team_race,
			_multiplayer_sound_team_slayer,
			_multiplayer_sound_team_koth,
			_multiplayer_sound_blue_team_ctf,
			_multiplayer_sound_red_team_ctf,
			_multiplayer_sound_contest,
			_multiplayer_sound_control,
			_multiplayer_sound_hill_occupied,
			_multiplayer_sound_countdown_timer_end,
			_multiplayer_sound_ting,
			_multiplayer_sound,

			// TODO: engine code needs fixing up in order to make use of these later indexes

			_multiplayer_sound_custom1 = _multiplayer_sound,
			_multiplayer_sound_custom2,
			_multiplayer_sound_custom3,
			_multiplayer_sound_custom4,
			_multiplayer_sound_custom5,
			_multiplayer_sound_custom6,
			_multiplayer_sound_custom7,
			_multiplayer_sound_custom8,
			_multiplayer_sound_custom9,
			_multiplayer_sound_custom10,
			_multiplayer_sound_custom11,
			_multiplayer_sound_custom12,
			_multiplayer_sound_custom13,
			_multiplayer_sound_custom14,
			_multiplayer_sound_custom15,
			_multiplayer_sound_custom16, // default max s_game_globals_multiplayer_information::sounds instances = 60

			_multiplayer_sound_custom,
		};
	};

	namespace blam
	{
		// Overload should only be used in network_server connections
		void PLATFORM_API game_engine_play_multiplayer_sound(datum_index player_index, _enum multiplayer_sound_index, bool should_replicate);

		// Locally play [multiplayer_sound_index]
		inline void game_engine_play_multiplayer_sound(_enum multiplayer_sound_index)
		{
			game_engine_play_multiplayer_sound(datum_index::null, multiplayer_sound_index, false);
		}
	};
};