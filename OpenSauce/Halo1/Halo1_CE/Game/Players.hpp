/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Memory/Data.hpp"

namespace Yelo
{
	namespace Networking
	{
		struct s_network_game_player;
	};

	namespace Objects
	{
		struct s_unit_datum;
	};
};

namespace Yelo
{
	namespace Enums
	{
		enum player_powerup
		{
			_player_powerup_active_camo,
			_player_powerup_full_spectrum_vision,
			_player_powerup,
		};

		enum player_action_result
		{
			_player_action_result_none,
			_player_action_result_pickup1,
			_player_action_result_pickup2,
			_player_action_result_exit_seat,
			_player_action_result_4, // game engine related

			_player_action_result_swap_equipment,
			_player_action_result_swap_weapon,
			_player_action_result_pickup_weapon,

			_player_action_result_enter_seat,
			_player_action_result_force_ai_to_exit_seat,
			_player_action_result_touch_device,
			_player_action_result_flip_vehicle,

			_player_action_result,
		};
	};

	namespace Players
	{
		struct s_player_set_action_result_network_data
		{
			datum_index player_index;
			int32 action_result_type;
			datum_index action_object_index;
			int16 action_result;
			int16 action_seat_index;
			datum_index weapon_to_drop_as_result_of_swap;
		}; BOOST_STATIC_ASSERT( sizeof(s_player_set_action_result_network_data) == 0x14 );

		struct s_action_update_data
		{
			long_flags control_flags;
			PAD64;
			struct {
				real r0, r1;
			}throttle;
			real primary_trigger;
			int16 desired_weapon_index;
			int16 desired_grenade_index;
		}; BOOST_STATIC_ASSERT( sizeof(s_action_update_data) == 0x1C );

		struct s_vehicle_update_data
		{
			datum_index vehicle_index;
			real_point3d position;
			real_vector3d translational_velocity, angular_velocity;
			real_vector3d forward, up;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_update_data) == 0x40 );

		struct s_remote_player_position_update_network_data
		{
			real_point3d position;
		}; BOOST_STATIC_ASSERT( sizeof(s_remote_player_position_update_network_data) == 0xC );
		struct s_remote_player_action_update_network_data
		{
			int32 ticks_to_apply_update_to;
			s_action_update_data action;
			PAD32;
			real_vector3d facing_vector;
		}; BOOST_STATIC_ASSERT( sizeof(s_remote_player_action_update_network_data) == 0x30 );


		struct s_action_queue_entry : TStructImpl(44)
		{
			// 0x4 int32 remaining_ticks_to_apply_action_to
		};
		struct s_action_queue
		{
			Memory::s_simple_circular_queue queue;
			s_action_queue_entry* entries;			// 0x14, #120, user_data
			UNKNOWN_TYPE(bool);						// 0x18
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_action_queue) == 0x1C );

		struct s_position_queue_entry : TStructImpl(20)
		{
		};
		struct s_position_queue
		{
			Memory::s_simple_circular_queue queue;
			s_position_queue_entry* entries;		// 0x14, #30, user_data
		}; BOOST_STATIC_ASSERT( sizeof(s_position_queue) == 0x18 );

		struct s_vehicle_update_queue_entry : TStructImpl(72)
		{
		};
		struct s_vehicle_update_queue
		{
			Memory::s_simple_circular_queue queue;
			s_vehicle_update_queue_entry* entries;	// 0x14, #30, user_data
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_update_queue) == 0x18 );


		union u_player_datum_scores
		{
			byte data[sizeof(int32)*2];

			struct s_ctf {
				int16 flag_grabs;
				int16 flag_returns;
				int16 flag_scores;
			}ctf;

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
		}; BOOST_STATIC_ASSERT( sizeof(u_player_datum_scores) == 8 );
		// Special yelo data used for player datums when running as a server
		struct s_player_yelo_server_data
		{
			enum { k_max_struct_size = 0x38 };

			struct _rcon_access {
				uint32 last_bad_password_time;		// Game tick of when the last failure took place
				int16 bad_password_cooldown;		// Ticks until it's kosher to decrement [number_of_bad_passwords]
				int16 number_of_bad_passwords;		// Number of times the player has given a bad password in a given threshold of time
			}rcon_access;

			struct _voting {
				bool voted;		// Has this player voted yet?
				sbyte option;	// Option, in enumerated form. Or NONE.
				PAD16;
			}voting;

		}; BOOST_STATIC_ASSERT( sizeof(s_player_yelo_server_data) <= s_player_yelo_server_data::k_max_struct_size );
		struct s_player_datum : TStructImpl(0x200)
		{
			TStructGetPtrImpl(Memory::s_datum_base, DatumHeader, 0x0);
			TStructGetPtrImpl(int16, LocalIndex, 0x2);
			TStructGetPtrImpl(wchar_t, DisplayName, 0x4);  // [12]
			// 0x1C UNUSED_TYPE(int32)
			TStructGetPtrImpl(int32, TeamIndex, 0x20);
			TStructGetPtrImpl(argb_color, Color, 0x20);
			struct s_action_result
			{
				datum_index action_object_index;
				int16 action_result;			// enum object_type
				int16 action_seat_index;
			};
			TStructGetPtrImpl(s_action_result, NearestObjectActionResult, 0x24);
			TStructGetPtrImpl(uint32, RespawnTime, 0x2C);
			// 0x30 uint32 respawn time growth related
			TStructGetPtrImpl(datum_index, SlaveUnitIndex, 0x34);
			TStructGetPtrImpl(datum_index, LastSlaveUnitIndex, 0x38);
			TStructGetPtrImpl(int16, ClusterIndex, 0x3C);
			TStructGetPtrImpl(bool, WeaponSwapResult, 0x3E);
			// PAD8
			TStructGetPtrImpl(datum_index, AutoAimTarget, 0x40); // biped_index
			TStructGetPtrImpl(uint32, AutoAimUpdateTime, 0x44);
			TStructGetPtrImpl(Networking::s_network_game_player, NetworkPlayer, 0x48);
			TStructGetPtrImpl(int16, PowerupTimes, 0x68); // int16[Enums::_player_powerup]
			TStructGetPtrImpl(real, Speed, 0x6C);
			TStructGetPtrImpl(int32, SourceTeleporterNetgameIndex, 0x70); // index to a netgame flag in the scenario, or NONE
			struct s_game_engine_state_message
			{
				int32 message_index;
				datum_index message_player_index; // player to use in the message?

				// values used for rendering a target player's name
				datum_index target_player;
				uint32 target_time; // timer used to fade in the target player name
			};
			TStructGetPtrImpl(s_game_engine_state_message, EngineStateMessage, 0x74);
			TStructGetPtrImpl(uint32, LastDeathTime, 0x84); // game tick of the last time this player was killed
			TStructGetPtrImpl(datum_index, TargetPlayerIndex, 0x88); // player index of the slayer target for this player
			TStructGetPtrImpl(bool, OddManOut, 0x8C);
			// PAD24
			// 0x90 UNUSED_TYPE(int32)
			// 0x94 UNUSED_TYPE(int16)
			TStructGetPtrImpl(int16, KillsThisLifetime, 0x96); // how many kills we've had in this lifetime (since the last spawn)
			TStructGetPtrImpl(int16, CurrentSpreeCount, 0x98); // how many kills we've had in our 'spree time' (4 second intervals max)
			TStructGetPtrImpl(int16, LastKillTime, 0x9A); // set from game_time_globals::local_time

			TStructGetPtrImpl(byte, StatsBuffer, 0x9C);
				TStructGetPtrImpl(int16, KillsArray, 0x9C);		// [4]
				TStructGetPtrImpl(int16, AssistsArray, 0xA4);	// [4]

			TStructGetPtrImpl(int16, FriendlyFireKills, 0xAC);
			TStructGetPtrImpl(int16, Deaths, 0xAE);
			TStructGetPtrImpl(int16, Suicides, 0xB0);
			// 0xB2? 0xB4? 0xB8? 0xBC?
			TStructGetPtrImpl(int16, TKs, 0xC0);
			// 0xC2?
			TStructGetPtrImpl(u_player_datum_scores, Scores, 0xC4);
			TStructGetPtrImpl(uint32, TelefragCounter, 0xCC); // # of ticks spent blocking teleporter
			TStructGetPtrImpl(int32, QuickGameTick, 0xD0); // game tick the player quick at
			TStructGetPtrImpl(bool, TelefragEnabled, 0xD4); // if we're blocking a teleporter, this will be true
			TStructGetPtrImpl(bool, QuitOutOfGame, 0xD5);
			// 0xD6?
			// 0xD8?
			TStructGetPtrImpl(int32, Ping, 0xDC);
			TStructGetPtrImpl(int32, TKNum, 0xE0);
			TStructGetPtrImpl(int32, TKTimer, 0xE4);

			struct s_client_update_data
			{
				// 0xE8 int32
				// 0xEC int32
				// 0xF0, s_remote_player_action_update_network_data
				// 0x120 s_action_queue
				// 0x13C, 0x20 byte structure
				// 0x15C int32
				// 0x160 int32
				// 0x164, s_remote_player_position_update_network_data
				// 0x170 s_position_queue
				// 0x188 uint32
				// 0x18C int32
				// 0x190, s_vehicle_update_data
				// 0x1D0 s_vehicle_update_queue
				// 0x1E8 uint32
				// 0x1EC uint32
				// 0x1F0 uint32
				// 0x1F4 uint32
				// 0x1F8 uint32

				// 0x1FC?
			};
			struct s_server_update_data
			{
				UNKNOWN_TYPE(uint32); // 0xE8
				UNKNOWN_TYPE(int32);  // 0xEC
				UNKNOWN_TYPE(int32);  // 0xF0
				int32 last_completed_update_id;
				real_point3d position;

				// 0x104 datum_index or int32
				// 0x108 boolean, PAD24
				// 0x10C uint32
				// 0x110 uint32
				// 0x114 uint32
				// 0x118 uint32
				// 0x11C uint32
				// 0x120 int32
				// 0x124 int32
				// 0x128 int32
				// 0x12C int32 server_update_data.action_baseline_id, NUMBER_OF_REMOTE_PLAYER_ACTION_UPDATE_BASELINE_IDS = 2
				// 0x130, s_remote_player_action_update_network_data
				// 0x160 uint32
				// 0x164 datum_index or int32
				// 0x168 datum_index or int32
				// 0x16C datum_index or int32
				// 0x170 real_point3d predict_position
				// 0x17C datum_index or int32
				// 0x180 datum_index or int32
				// 0x184 datum_index or int32
				// 0x188, 0x40 byte structure

				// 0x1C8?
			}; //BOOST_STATIC_ASSERT( sizeof(s_server_update_data) == 0xE0 );
			TStructGetPtrImpl(s_client_update_data, ClientUpdateData, 0xE8);
			TStructGetPtrImpl(s_server_update_data, ServerUpdateData, 0xE8);

			// !!! Only use this when the local machine is the host (ie, Networking::IsServer() == true) !!!
			TStructGetPtrImpl(s_player_yelo_server_data, ServerYeloData, 0x1C8);


			Objects::s_unit_datum* GetPlayerUnit();
			datum_index GetVehicleIndex();
		};
		typedef Memory::DataArray<s_player_datum, Enums::k_multiplayer_maximum_players_upgrade> t_players_data;
		t_players_data*					Players();


		struct s_team_datum : TStructImpl(0x40)
		{
			// nothing even uses this...this structure 
			// could have no real fields...maybe use it 
			// for our own evil deeds?
		};
		typedef Memory::DataArray<s_team_datum, Enums::k_multiplayer_maximum_teams> t_teams_data;
		t_teams_data*					Teams();


		struct s_players_globals_data
		{
			struct _local_player {
				int32 count1;
				uint32 players[8];
				int32 count2;
				bool are_all_dead;
				bool input_enabled;
				bool teleport_on_bsp_switch;
				PAD24;
				bool was_teleported; // or respawned
				PAD8;
			}local_player;

			byte pad[104];
		};
		s_players_globals_data*			PlayersGlobals();

		struct s_player_control : TStructImpl(0x40)
		{
			TStructGetPtrImpl(datum_index, UnitIndex, 0x0);

			// 0x8, word
			// 0xA, word
			TStructGetPtrImpl(real_euler_angles2d, DesiredAngles, 0xC); // facing
			// 0x14, real

			TStructGetPtrImpl(int16, CurrentWeaponIndex, 0x20);
			TStructGetPtrImpl(int16, CurrentGrenadeIndex, 0x22);
			TStructGetPtrImpl(int16, ZoomLevel, 0x24);
			// 0x26, sbyte
			// 0x27, sbyte
			TStructGetPtrImpl(datum_index, TargetUnitIndex, 0x28);
			TStructGetPtrImpl(real, AutoaimLevel, 0x2C);
			// 0x30, real

			// 0x38, real
			// 0x3C, real
		};
		struct s_player_control_globals_data
		{
			long_flags action_flags[2]; // see "action_test" script functions
			PAD32; // TODO: document the fields in the first 12 bytes of this struct
			long_flags flags; // FLAG(0) = camera control

			s_player_control local_players[Enums::k_maximum_number_of_local_players];
		}; BOOST_STATIC_ASSERT( sizeof(s_player_control_globals_data) == 0x50 );
		s_player_control_globals_data*	PlayerControlGlobals();

		struct s_player_effects_data : TStructImpl(0x128)
		{
		};
		s_player_effects_data*			PlayerEffects();


		datum_index*					MachineToPlayerTable();

		void Initialize();
		void Dispose();

		void PLATFORM_API UpdateServer();	// server & client updates are called before core game update functions
		void PLATFORM_API UpdateClient();
		void PLATFORM_API Update();			// called after core game update functions

		// Get the handle to the currently active local player
		// Returns a null handle if there is none.
		datum_index LocalPlayerIndex();
		// Get the player data of the currently active local player
		// Returns NULL if there is none.
		s_player_datum* LocalPlayer();
		// Get the player data from a player number, and optionally their player_index
		// Returns NULL if player number is invalid or unused
		s_player_datum* GetPlayerFromNumber(byte player_number, __out datum_index* player_index_reference = NULL);


		// Use the handle to a player to get the active unit object which it controls
		// Returns NULL if player handle or the unit handle for the player is NONE
		Objects::s_unit_datum* GetPlayerUnit(datum_index player_index);

		// Use the handle to a player to get the weapons held by it's active unit which 
		// they are in control of.
		// Returns NULL if player handle or the unit handle for the player is NONE
		datum_index* GetWeapons(datum_index player_index, int16* current_weapon_index = NULL);

		// Use the handle to a player to get the active unit object of the vehicle 
		// which the unit the player is controlling is currently in.
		// Returns NULL if player handle or the unit handle for the player is NONE
		// Also returns NULL if player isn't in a vehicle
		// NOTE: "vehicle" is a loose term here, could be another unit derived type, 
		// possibly even a biped, who knows!
		Objects::s_unit_datum* GetVehicle(datum_index player_index, int16* current_seat_index = NULL);
	};
};