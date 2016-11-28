/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

// why did I put this file in Networking?
// Because Game\ will be bloated in the end, that's why -_-

namespace Yelo
{
	namespace Enums
	{
		enum game_results_statistic
		{
			_game_results_statistic_games_played,
			_game_results_statistic_games_quit,
			_game_results_statistic_games_disconnected,
			_game_results_statistic_games_completed,
			_game_results_statistic_games_won,
			_game_results_statistic_games_tied,
			_game_results_statistic_rounds_won,

			_game_results_statistic_kills, // 0x7
			_game_results_statistic_assists,
			_game_results_statistic_deaths,
			_game_results_statistic_betrayals,
			_game_results_statistic_suicides,
			_game_results_statistic_most_kills_in_a_row,

			_game_results_statistic_seconds_alive, // 0xD
			_game_results_statistic_ctf_flag_scores, // 0xE
			_game_results_statistic_ctf_flag_grabs, // 0xF
			_game_results_statistic_ctf_flag_carrier_kills, // 0x10
			_game_results_statistic_ctf_flag_returns, // 0x11
			_game_results_statistic_ctf_bomb_scores, // 0x12
			_game_results_statistic_ctf_bomb_plants, // 0x13
			_game_results_statistic_ctf_bomb_carrier_kills, // 0x14
			_game_results_statistic_ctf_bomb_grabs, // 0x15
			_game_results_statistic_ctf_bomb_returns, // 0x16
			_game_results_statistic_oddball_time_with_ball, // 0x17
			_game_results_statistic_oddball_unused, // 0x18
			_game_results_statistic_oddball_kills_as_carrier, // 0x19
			_game_results_statistic_oddball_ball_carrier_kills, // 0x1A
			_game_results_statistic_king_time_on_hill, // 0x1B
			_game_results_statistic_king_total_control_time, // 0x1C
			_game_results_statistic_king_number_of_controls, // unused
			_game_results_statistic_king_longest_control_time, // unused
			_game_results_statistic_race_laps, // unused
			_game_results_statistic_race_total_lap_time, // unused
			_game_results_statistic_race_fastest_lap_time, // unused
			_game_results_statistic_headhunter_heads_picked_up, // unused
			_game_results_statistic_headhunter_heads_deposited, // unused
			_game_results_statistic_headhunter_number_of_deposits, // unused
			_game_results_statistic_headhunter_largest_deposit, // unused
			_game_results_statistic_juggernaut_kills, // 0x26
			_game_results_statistic_juggernaut_kills_as_juggernaut, // 0x27
			_game_results_statistic_juggernaut_total_control_time, // 0x28
			_game_results_statistic_juggernaut_number_of_controls, // unused
			_game_results_statistic_juggernaut_longest_control_time, // unused
			_game_results_statistic_territories_taken, // 0x2B
			_game_results_statistic_territories_lost, // 0x2C

			_game_results_statistic,
		};

		enum game_results_damage
		{
			_game_results_damage_kills,
			_game_results_damage_deaths,
			_game_results_damage_betrayals,
			_game_results_damage_suicides,
			_game_results_damage_shots_fire,
			_game_results_damage_shots_hit,
			_game_results_damage_headshots,

			_game_results_damage,
		};

		enum game_results_player_vs_player
		{
			_game_results_player_vs_player_kills,
			_game_results_player_vs_player_deaths,

			_game_results_player_vs_player,
		};

		enum game_results_medeal
		{
			_game_results_medeal_multiple_kill_2,
			_game_results_medeal_multiple_kill_3,
			_game_results_medeal_multiple_kill_4,
			_game_results_medeal_multiple_kill_5,
			_game_results_medeal_multiple_kill_6,
			_game_results_medeal_multiple_kill_7_or_more,

			_game_results_medeal_sniper_kill, // 0x6
			_game_results_medeal_collision_kill,
			_game_results_medeal_bash_kill,
			_game_results_medeal_stealth_kill,
			_game_results_medeal_killed_vehicle,
			_game_results_medeal_boarded_vehicle,
			_game_results_medeal_grenade_stick,

			_game_results_medeal_5_kills_in_a_row, // 0xD
			_game_results_medeal_10_kills_in_a_row,
			_game_results_medeal_15_kills_in_a_row,
			_game_results_medeal_20_kills_in_a_row,
			_game_results_medeal_25_kills_in_a_row,

			_game_results_medeal_ctf_flag_grab, // 0x12
			_game_results_medeal_ctf_flag_carrier_kill,
			_game_results_medeal_ctf_flag_returned,
			_game_results_medeal_ctf_bomb_planted,
			_game_results_medeal_ctf_bomb_carrier_kill,
			_game_results_medeal_ctf_bomb_defused,

			_game_results_medeal_unused1,
			_game_results_medeal_unused2,
			_game_results_medeal_unused3,
			_game_results_medeal_unused4,
			_game_results_medeal_unused5,
			_game_results_medeal_unused6,
			_game_results_medeal_unused7,
			_game_results_medeal_unused8,

			_game_results_medeal,
		};
	};

	namespace Statistics
	{
		struct pcr_stat_player
		{
			wchar_t PlayerName[16]; // 0x0
			wchar_t DisplayName[16]; // 0x20
			wchar_t ScoreString[16]; // 0x40

			int32 Kills; // 0x60
			int32 Deaths; // 0x64
			int32 Assists; // 0x68
			int32 Suicides; // 0x6C

			int16 Place; // 0x70
			UNKNOWN_TYPE(int16); // 0x72
			UNKNOWN_TYPE(byte); PAD24; // 0x74
			UNKNOWN_TYPE(int32); // 0x78

			int32 MedealsEarned; // 0x7C
			long_flags MedealsEarnedByType; // 0x80

			int32 TotalShots; // 0x84
			int32 ShotsHit; // 0x88
			int32 HeadShots; // 0x8C

			int32 Killed[16]; // 0x90

			// 16 byte structure
			UNKNOWN_TYPE(int32); // 0xD0
			UNKNOWN_TYPE(int32); // 0xD4
			UNKNOWN_TYPE(int32); // 0xD8
			UNKNOWN_TYPE(int32); // 0xDC

			wchar_t PlaceString[16]; // 0xE0

			// 12 byte structure
			UNKNOWN_TYPE(int32); // 0x100
			UNKNOWN_TYPE(int32); // 0x104
			UNKNOWN_TYPE(byte); PAD24; // 0x108

			union { // 0x10C, 0x110
				struct _Ctf {
					int32 FlagCarrierKills;
					int32 FlagGrabs;
				}Ctf;
				struct _Slayer {
					int32 AverageLife;
					int32 MostKillsInARow; // Best Spree
				}Slayer;
				struct _Oddball {
					int32 BallCarrierKills;
					int32 KillsAsCarrier;
				}Oddball;
				struct _Koth {
					int32 TotalControlTime;
					int32 TimeOnHill;
				}Koth;
				struct _Juggernaut {
					int32 JuggernautKills;
					int32 KillsAsJuggernaut;
				}Juggernaut;
				struct _Territories {
					long TerritoriesTaken;
					long TerritoriesLost;
				}Territories;
				struct _Assault {
					int32 BombGrabs;
					int32 BombCarrierKills;
				}Assault;
			};
		}; BOOST_STATIC_ASSERT( sizeof(pcr_stat_player) == 0x114 );

		void Initialize();
		void Dispose();

		void PersistPcr(char drive, const char file[24]);
		void OnBuildPcr();
	};

	namespace GameState
	{
		Statistics::pcr_stat_player* _PcrPlayerStats();
	};
};