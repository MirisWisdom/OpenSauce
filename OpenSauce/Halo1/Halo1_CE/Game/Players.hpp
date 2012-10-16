/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Memory/Data.hpp"

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
			// PAD16
			// 0xB4 UNUSED_TYPE(int32)
			// 0xB8 UNUSED_TYPE(int32)
			// 0xBC UNUSED_TYPE(int32)
			TStructGetPtrImpl(int16, TKs, 0xC0);
			// PAD16
			TStructGetPtrImpl(u_player_datum_scores, Scores, 0xC4);
			TStructGetPtrImpl(uint32, TelefragCounter, 0xCC); // # of ticks spent blocking teleporter
			TStructGetPtrImpl(int32, QuickGameTick, 0xD0); // game tick the player quick at
			TStructGetPtrImpl(bool, TelefragEnabled, 0xD4); // if we're blocking a teleporter, this will be true
			TStructGetPtrImpl(bool, QuitOutOfGame, 0xD5);
			// PAD16
			// 0xD8 UNUSED_TYPE(int32)
			TStructGetPtrImpl(int32, Ping, 0xDC);
			TStructGetPtrImpl(int32, TKNum, 0xE0);
			TStructGetPtrImpl(int32, TKTimer, 0xE4);
			TStructGetPtrImpl(Networking::s_player_client_update, ClientUpdateData, 0xE8);
			TStructGetPtrImpl(Networking::s_player_server_update, ServerUpdateData, 0xE8);
			// !!! Only use this when the local machine is the host (ie, Networking::IsServer() == true) !!!
			TStructGetPtrImpl(s_player_yelo_server_data, ServerYeloData, 0x1C8);

#if FALSE
			union {
				byte _unused_networking_buffer[0x200 - 0xE8];

				Networking::s_player_client_update client_update_data;
				struct {
					Networking::s_player_server_update update_data;
					// !!! Only use this when the local machine is the host (ie, Networking::IsServer() == true) !!!
					s_player_yelo_server_data yelo_data;
				}server;
			};
#endif


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
		s_players_globals_data*			PlayersGlobals();

		struct s_player_control : TStructImpl(0x40)
		{
			TStructGetPtrImpl(datum_index, UnitIndex, 0x0);

			// 0x8, word
			// 0xA, word
			TStructGetPtrImpl(real_euler_angles2d, DesiredAngles, 0xC); // facing
			TStructGetPtrImpl(real_vector2d, Throttle, 0x14);
			TStructGetPtrImpl(real, PrimaryTrigger, 0x1C);
			TStructGetPtrImpl(int16, CurrentWeaponIndex, 0x20);
			TStructGetPtrImpl(int16, CurrentGrenadeIndex, 0x22);
			TStructGetPtrImpl(int16, ZoomLevel, 0x24);
			TStructGetPtrImpl(sbyte, WeaponSwapTicks, 0x26);
			// 0x27, sbyte, unused ticks counter
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


		datum_index*					MachineToPlayerTable(); // [k_multiplayer_maximum_players][k_maximum_number_of_local_players]

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