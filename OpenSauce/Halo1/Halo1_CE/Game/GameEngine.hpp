/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

// just for our game engine definition's networking delegate code
//#include "Networking/MessageDeltas.hpp"

namespace Yelo // forward declare, just for our game engine definition's networking delegate code
{
	namespace MessageDeltas
	{
		struct message_dependant_header;
	};
	namespace Networking
	{
		struct s_network_game_client;
	};
};


namespace Yelo
{
	namespace Enums
	{
		enum game_engine : long_enum
		{
			_game_engine_none,
			_game_engine_ctf,
			_game_engine_slayer,
			_game_engine_oddball,
			_game_engine_king,
			_game_engine_race,
			_game_engine_stub,
			_game_engine,

			_game_engine_custom1 = _game_engine,
			_game_engine_custom2,
			_game_engine_custom3,
			_game_engine_custom4,
			_game_engine_custom5,
			_game_engine_custom,
		};

		enum game_trait : long_enum
		{
			_game_trait_none,
			_game_trait_invisible,
			_game_trait_extra_damage,
			_game_trait_damage_resistant,
			_game_trait,

			_game_custom_trait1 /*= _game_trait*/,
			_game_custom_trait2,
			_game_custom_trait3,
			_game_custom_trait4,
			_game_custom_trait5,
			_game_custom_trait,
		};

		enum game_variant_goal_radar : long_enum
		{
			_game_variant_goal_radar_motion_tracker,
			_game_variant_goal_radar_navpoints,
			_game_variant_goal_radar_none,

			_game_variant_goal_radar,
		};

		enum game_variant_weapon_set : long_enum
		{
			_game_variant_weapon_set_normal,
			_game_variant_weapon_set_pistols,
			_game_variant_weapon_set_rifles,
			_game_variant_weapon_set_plasma,
			_game_variant_weapon_set_sniper,
			_game_variant_weapon_set_sans_snipers,
			_game_variant_weapon_set_rockets,
			_game_variant_weapon_set_shotguns,
			_game_variant_weapon_set_short_range,
			_game_variant_weapon_set_human,
			_game_variant_weapon_set_covenant,
			_game_variant_weapon_set_classic,
			_game_variant_weapon_set_heavies,

			_game_variant_weapon_set,
		};

		enum oddball_carrier_speed : long_enum
		{
			_oddball_carrier_speed_normal,
			_oddball_carrier_speed_faster,
			_oddball_carrier_speed_slower,

			_oddball_carrier_speed,
		};

		enum oddball_ball_type : long_enum
		{
			_oddball_ball_type_normal,
			_oddball_ball_type_reverse_tag,
			_oddball_ball_type_juggernaut,

			_oddball_ball_type,
		};
	};

	namespace Flags
	{
		enum game_variant_flags : long_flags
		{
			_game_variant_ball_indicator_bit,
			_game_variant_friends_on_radar_bit,
			_game_variant_use_custom_start_equipment_bit,
			_game_variant_invisible_players_bit,
			_game_variant_players_have_shields_bit,
			_game_variant_infinite_grenades_bit,
			_game_variant_show_homies_bit,
			_game_variant_show_players_on_radar_bit,
		};
	};

	namespace GameEngine
	{
		struct s_ctf_globals
		{
			real_point3d* team_flag_positions[2];
			datum_index team_flag_object_indexes[2]; // weapon objects
			int32 team_score[2];
			int32 score_limit;
			bool team_flag_is_taken[2];
			PAD16;
			uint32 team_flag_is_taken_sound_timer[2];
			uint32 flag_swap_timer;
			uint32 flag_failure_sound_timer;
			byte team_with_flag;
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_ctf_globals) == 0x34 );
		s_ctf_globals* CtfGlobals();

		struct s_slayer_globals
		{
		}; //BOOST_STATIC_ASSERT( sizeof(s_slayer_globals) == 0x );
		s_slayer_globals* SlayerGlobals();

		struct s_oddball_globals
		{
			int32 score_to_win;
			int32 team_score[16];
			int32 individual_score[16];
			int32 undocumented1[16];
			datum_index current_ball_owners[16]; // player_index
			uint32 current_ball_held_times[16]; // game-ticks
		};
		s_oddball_globals* OddballGlobals();

		struct s_king_globals
		{
		}; //BOOST_STATIC_ASSERT( sizeof(s_king_globals) == 0x1B0 );
		s_king_globals* KingGlobals();

		struct s_race_globals
		{
		}; //BOOST_STATIC_ASSERT( sizeof(s_race_globals) == 0x );
		s_race_globals* RaceGlobals();



		struct game_engine_definition
		{
			cstring name;											// 0x0

			_enum type;												// 0x4
			PAD16;													// 0x6

			typedef void (PLATFORM_API* _dispose)();
			_dispose dispose;										// 0x8

			typedef bool (PLATFORM_API* _initialize_for_new_map)();
			_initialize_for_new_map initialize_for_new_map;			// 0xC

			typedef void (PLATFORM_API* _dispose_from_old_map)();
			_dispose_from_old_map dispose_from_old_map;				// 0x10

			typedef void (PLATFORM_API* _player_added)(datum_index player_index);
			_player_added player_added;								// 0x14

			void* unused0;											// 0x18

			typedef void (PLATFORM_API* _game_ending)();
			_game_ending game_ending;								// 0x1C

			typedef void (PLATFORM_API* _game_starting)();
			_game_starting game_starting;							// 0x20

			// --- DEPRICATED functions (used on xbox)
			PAD32;													// 0x24
			PAD32;													// 0x28
			PAD32;													// 0x2C
			PAD32;													// 0x30
			// --- DEPRICATED

			typedef void (PLATFORM_API* _post_rasterize)();
			_post_rasterize post_rasterize;							// 0x34

			typedef void (PLATFORM_API* _player_update)(datum_index player_index);
			_player_update player_update;							// 0x38

			//typedef void (PLATFORM_API* _weapon_update)(datum_index weapon_index, weapon_datum* weapon);
			PAD32;//_weapon_update weapon_update;							// 0x3C

			typedef bool (PLATFORM_API* _weapon_pickup)(datum_index weapon_index, datum_index player_index);
			_weapon_pickup weapon_pickup;							// 0x40

			typedef void (PLATFORM_API* _weapon_drop)(datum_index weapon_index);
			void* weapon_drop;										// 0x44

			typedef void (PLATFORM_API* _update)();
			_update update;											// 0x48

			typedef int32 (PLATFORM_API* _get_score)(datum_index player_index, int32 use_team_score);
			_get_score get_score;									// 0x4C

			typedef int32 (PLATFORM_API* _get_team_score)(int32 team_index);
			_get_team_score get_team_score;							// 0x50

			// returns out buffer
			typedef wchar_t* (PLATFORM_API* _get_score_string)(wchar_t* out_buffer, datum_index player_index);
			_get_score_string get_score_string;						// 0x54

			// returns out buffer
			typedef wchar_t* (PLATFORM_API* _get_score_header_string)(wchar_t* out_buffer);
			_get_score_header_string get_score_header_string;		// 0x58

			// returns out buffer
			typedef wchar_t* (PLATFORM_API* _get_team_score_string)(int32 team_index, wchar_t* out_buffer);
			_get_team_score_string get_team_score_string;			// 0x5C

			typedef bool (PLATFORM_API* _allow_pick_up)(datum_index unit_index, datum_index weapon_index);
			_allow_pick_up allow_pick_up;							// 0x60

			typedef void (PLATFORM_API* _player_damaged_player)(datum_index player_index, datum_index damaged_player, bool /*update_speed?*/);
			_player_damaged_player* player_damaged_player;			// 0x64

			typedef void (PLATFORM_API* _player_killed_player)(datum_index player_index, datum_index unit_index, datum_index dead_player, bool /*update_speed?*/);
			_player_killed_player player_killed_player;				// 0x68

			typedef bool (PLATFORM_API* _display_score)(datum_index player_index, int32 format_type, wchar_t* out_buffer, int32 max_length);
			_display_score display_score;							// 0x6C

			typedef real (PLATFORM_API* _starting_location_rating)(datum_index player_index, real_point3d* location);
			_starting_location_rating starting_location_rating;		// 0x70

			typedef void (PLATFORM_API* _prespawn_player_update)(datum_index player_index);
			_prespawn_player_update prespawn_player_update;			// 0x74

			typedef bool (PLATFORM_API* _postspawn_player_update)(datum_index player_index);
			_postspawn_player_update postspawn_player_update;		// 0x78

			void* unused1;											// 0x7C

			// returns [true] if the engine's goal is [player_index]
			typedef bool (PLATFORM_API* _goal_matches_player)(datum_index player_index);
			_goal_matches_player goal_matches_player;				// 0x80

			typedef bool (PLATFORM_API* _test_flag)(uint32 flags);
			_test_flag test_flag;									// 0x84

			typedef bool (PLATFORM_API* _test_trait)(datum_index player_index, Enums::game_trait trait);
			_test_trait test_trait;									// 0x88

			// returns [true] if [player_index] was the winner of the game
			typedef bool (PLATFORM_API* _test_player_won)(datum_index player_index);
			_test_player_won test_player_won;						// 0x8C

			// new to pc

			// -1 for machine sends to all.
			typedef void (PLATFORM_API* _to_network)(void* baselines, int32 specific_machine /* = NONE*/ );
			_to_network to_network;									// 0x90

			typedef void (PLATFORM_API* _from_network)(MessageDeltas::message_dependant_header* header, Networking::s_network_game_client* client);
			_from_network from_network;								// 0x94

			typedef void (PLATFORM_API* _player_reset_score)(datum_index player_index);
			_player_reset_score player_reset_score;					// 0x98

			// used by gamespy api
			typedef bool (PLATFORM_API* _get_gamespy_packet_field_string)(int32 field /*gamespy_field*/, char* buffer);
			_get_gamespy_packet_field_string get_gamespy_packet_field_string;	// 0x9C

			// 1st param must be 22 in order to return true
			typedef bool (PLATFORM_API* _create_player_score_strings)(uint32 flags, datum_index player_index, cstring out_buffer);
			_create_player_score_strings create_player_score_strings;	// 0xA0

			// 1st param must be 29 in order to return true
			typedef bool (PLATFORM_API* _create_team_score_strings)(uint32 flags, int32 team_index, cstring out_buffer);
			_create_team_score_strings create_team_score_strings;	// 0xA4

			// used by gamespy api, check out sub_57BDA0
			void* unused2;											// 0xA8

			typedef void (PLATFORM_API* _initialize_for_reset_map)();
			_initialize_for_reset_map initialize_for_reset_map;		// 0xAC
		}; BOOST_STATIC_ASSERT( sizeof(game_engine_definition) == 0xB0 );

		struct s_universal_variant
		{
			bool							teams;
			PAD24;
			Flags::game_variant_flags		flags;
			Enums::game_variant_goal_radar	goal_rader;
			bool odd_man_out;
			PAD24;

			int32							respawn_time_growth;
			int32							respawn_time;
			int32							suicide_penalty;
			int32							lives;

			real							health; // I thought this was damage_multiplier...
			int32							score_to_win;
			Enums::game_variant_weapon_set	weapon_set;

			int32							vehicle_set[2]; // red, blue
			int32							vehicles_respawn;

			byte							friendly_fire; // need an enum for this
			PAD24;
			int32							friendly_fire_penalty;
			bool							team_autobalance;
			PAD24;

			int32							time_limit;
		}; BOOST_STATIC_ASSERT( sizeof(s_universal_variant) == 0x48 );

		union s_game_engine_variant
		{
			PAD(0, sizeof(int32)*14);

			struct {
				bool assault;
				PAD8; // unused...

				struct {
					bool reset;
					bool at_home_to_score;
					int32 flag_type; // 1 = single
				}flag;
			}ctf;

			struct {
				bool death_handicap; // True: killed player's speed increases in magnitudes of one
				bool kill_handicap; // True: killer's speed decreases in magnitudes of two
				bool kill_in_order;
			}slayer;

			struct {
				UNKNOWN_TYPE(bool);
				PAD24;
				Enums::oddball_carrier_speed ball_speed;

				Enums::game_trait nonball_trait;
				Enums::game_trait ball_trait;
				Enums::oddball_ball_type type;
				int32 ball_count;
			}oddball;

			struct {
				UNKNOWN_TYPE(bool);
			}king;

			struct {
				int32 race_type;
				UNKNOWN_TYPE(int32); // enum...
			}race;
		}; BOOST_STATIC_ASSERT( sizeof(s_game_engine_variant) == 0x38 );

		struct s_game_variant
		{
			wchar_t name[24];							// 0x0
			Enums::game_engine game_engine_index;		// 0x30
			s_universal_variant universal_variant;		// 0x34
			PAD64 PAD64 PAD64 PAD64;					// 0x7C, unused...
			s_game_engine_variant game_engine_variant;	// 0x9C
			long_flags flags;							// 0xD4
			//uint32 crc;								// 0xD8, not actually part of the structure, only "used" when presisting to/from files
		}; BOOST_STATIC_ASSERT( sizeof(s_game_variant) == 0xD8/*0xDC*/ );
		s_game_variant* GlobalVariant();

		struct s_vehicle_team_remapper
		{
			int32 team_index;
			int32 scenario_datum_index; // vehicle block index
			word_flags multiplayer_spawn_flags;
			PAD16;
			datum_index vehicle_definition_index;

			struct s_datum {
				int32 scenario_datum_index;
				bool spawn; // will this vehicle be spawned?
				byte_flags spawn_flags;
				PAD16;
			}datums[8];
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_team_remapper) == 0x50 );
		struct s_vehicle_game_type_remapper
		{
			s_vehicle_team_remapper team_mappings[2];
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_game_type_remapper) == 0xA0 );
		struct s_vehicle_remapper
		{
			s_vehicle_game_type_remapper game_type_mappings[Enums::_game_engine-1];

			bool processed;
			bool is_teamplay;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_remapper) == 0x3C2 );
		// v1.08: 0x7FB300


		// List of old game engines which can be used
		const game_engine_definition* const* Definitions();
		// Current game engine being used
		const game_engine_definition* Current();

		// List of original and custom made game engines
		const game_engine_definition* const* NewDefinitions();
		// How many total game engine definitions there are (that are valid and not null)
		int32 NewDefinitionsCount();

		// Register a engine definition so that it may be used by the game code
		void RegisterNewGameEngine(game_engine_definition* engine);

		void Initialize();
		void Dispose();
		void PLATFORM_API InitializeForNewMap();
		// Game components updated before the game_enigne:
		//	* game allegiance
		//	* ai
		//	* players (if we're not the server)
		//	* effects
		//	* FP weapons
		// Everything else is updated after the game_engine (scripting, objects, etc).
		// Our update is performed AFTER the engine's update is done.
		void PLATFORM_API Update();
	};
};