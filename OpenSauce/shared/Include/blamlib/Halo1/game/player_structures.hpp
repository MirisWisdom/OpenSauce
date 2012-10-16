/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/player_action.hpp>
#include <blamlib/Halo1/game/players.hpp>
#include <blamlib/Halo1/networking/network_game_manager.hpp>
#include <blamlib/Halo1/networking/player_update_client.hpp>
#include <blamlib/Halo1/networking/player_update_server.hpp>

namespace Yelo
{
	namespace Objects
	{
		struct s_unit_datum;
	};
};

namespace Yelo
{
	namespace Players
	{
		union u_player_multiplayer_stats
		{
			PAD64;

			struct s_ctf {
				int16 flag_grabs;
				int16 flag_returns;
				int16 flag_scores;
			}ctf_statistics;

			struct s_slayer {
			}slayer;

			struct s_oddball {
				UNKNOWN_TYPE(int16);
				int16 target_kills;
				int16 kills;
			}oddball;

			struct s_king {
				int16 hill_score;
			}king;

			struct s_race {
				int16 time;
				int16 laps;
				int16 best_time;
			}race;
		}; BOOST_STATIC_ASSERT( sizeof(u_player_multiplayer_stats) == 8 );

		// Special yelo data used for player datums when running as a server
		struct s_player_yelo_server_data
		{
			enum { k_max_struct_size = 0x38 };

			struct _rcon_access {
				int32 last_bad_password_time;		// Game tick of when the last failure took place
				int16 bad_password_cooldown;		// Ticks until it's kosher to decrement [number_of_bad_passwords]
				int16 number_of_bad_passwords;		// Number of times the player has given a bad password in a given threshold of time
			}rcon_access;

			struct _voting {
				bool voted;		// Has this player voted yet?
				sbyte option;	// Option, in enumerated form. Or NONE.
				PAD16;
			}voting;

		}; BOOST_STATIC_ASSERT( sizeof(s_player_yelo_server_data) <= s_player_yelo_server_data::k_max_struct_size );

		struct s_player_datum : Memory::s_datum_base
		{
			struct s_game_engine_state_message
			{
				int32 message_index;
				datum_index message_player_index; // player to use in the message?

				// values used for rendering a target player's name
				datum_index target_player;
				int32 target_time; // timer used to fade in the target player name
			};

			int16 local_player_index;							// 0x2
			wchar_t display_name[Enums::k_player_name_length+1];// 0x4
			UNUSED_TYPE(int32);									// 0x1C
			union {												// 0x20
				int32 team_index;
				argb_color color;
			};
			s_player_action_result nearest_object_action_result;// 0x24
			int32 respawn_time;									// 0x2C
			UNKNOWN_TYPE(int32);								// 0x30 respawn time growth related
			datum_index slave_unit_index;						// 0x34
			datum_index last_slave_unit_index;					// 0x38
			int16 cluster_index;								// 0x3C
			bool weapon_swap_result;							// 0x3E
			PAD8;
			datum_index auto_aim_target;						// 0x40 biped_index
			int32 auto_aim_update_time;							// 0x44
			Networking::s_network_game_player network_player;	// 0x48
			int16 powerup_times[Enums::k_number_of_of_player_powerups];	// 0x68
			real speed;											// 0x6C
			int32 source_teleporter_netgame_index;				// 0x70 index to a netgame flag in the scenario, or NONE
			s_game_engine_state_message engine_state_messsage;	// 0x74
			int32 last_death_time;								// 0x84 game tick of the last time this player was killed
			datum_index target_player_index;					// 0x88 player index of the slayer target for this player
			bool odd_man_out;									// 0x8C
			PAD24;
			UNUSED_TYPE(int32);									// 0x90
			UNUSED_TYPE(int16);									// 0x94
			int16 kills_this_lifetime;							// 0x96 how many kills we've had in this lifetime (since the last spawn)
			int16 current_spree_count;							// 0x98 how many kills we've had in our 'spree time' (4 second intervals max)
			int16 last_kill_time;								// 0x9A set from game_time_globals::local_time
			union {												// 0x9C
				byte player_score_buffer[0x10];
				struct {
					int16 kills[4];								// 0x9C
					int16 assists[4];							// 0xA4
				}player_score;
			};
			int16 friendly_fire_kills;							// 0xAC
			int16 deaths;										// 0xAE
			int16 suicides;										// 0xB0
			PAD16;
			UNUSED_TYPE(int32);									// 0xB4
			UNUSED_TYPE(int32);									// 0xB8
			UNUSED_TYPE(int32);									// 0xBC
			int16 team_kills;									// 0xC0
			PAD16;
			u_player_multiplayer_stats multiplayer_stats;		// 0xC4
			int32 telefrag_counter;								// 0xCC # of ticks spent blocking teleporter
			int32 quit_game_tick;								// 0xD0 game tick the player quick at
			bool telefrag_enabled;								// 0xD4 if we're blocking a teleporter, this will be true
			bool quit_out_of_game;								// 0xD5
			PAD16;
			UNUSED_TYPE(int32);									// 0xD8
			int32 ping;											// 0xDC
			int32 team_kill_number;								// 0xE0
			int32 team_kill_timer;								// 0xE4

			union {												// 0xE8
				byte _unused_networking_buffer[0x200 - 0xE8];

				struct {
					Networking::s_player_client_update update_data;
					PAD32;										// 0x1FC
				}client;
				struct {
					Networking::s_player_server_update update_data;
					// !!! Only use this when the local machine is the host (ie, Networking::IsServer() == true) !!!
					s_player_yelo_server_data yelo_data;		// 0x1C8
				}server;
			};


#if !PLATFORM_IS_EDITOR
			Objects::s_unit_datum* GetPlayerUnit();
			datum_index GetVehicleIndex();
#endif
		}; BOOST_STATIC_ASSERT( sizeof(s_player_datum) == 0x200 );


		struct s_team_datum : TStructImpl(0x40)
		{
			// Based on the size of this, I'm *guessing* this is what the struct would contain
			datum_index players[Enums::k_multiplayer_maximum_players];

			// nothing even uses this...this structure 
			// could have no real fields...maybe use it 
			// for our own evil deeds?
		};


		struct s_players_globals_data
		{
			UNUSED_TYPE(int32);																// 0x0, initialized to NONE but that's all I can tell
			// the player_index for each local player
			datum_index local_player_players[Enums::k_maximum_number_of_local_players];		// 0x4
			// the object_index of each local player's dead unit (their old body)
			datum_index local_player_dead_units[Enums::k_maximum_number_of_local_players];	// 0x8
			int16 local_player_count;														// 0xC
			int16 double_speed_ticks_remaining;												// 0xE
			bool are_all_dead;																// 0x10
			bool input_disabled;															// 0x11
			UNKNOWN_TYPE(int16);															// 0x12, bsp switch trigger index
			_enum respawn_failure;															// 0x14
			bool was_teleported;															// 0x16, or respawned
			PAD8;
			long_flags pvs[BIT_VECTOR_SIZE_IN_DWORDS(512)];								// 0x18
			long_flags pvs2[BIT_VECTOR_SIZE_IN_DWORDS(512)];								// 0x58
		}; BOOST_STATIC_ASSERT( sizeof(s_players_globals_data) == 0x98 );
	};
};