/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/players.hpp>
#include <blamlib/Halo1/saved_games/game_variant.hpp>

namespace Yelo
{
	namespace MessageDeltas
	{
		struct message_dependant_header;
	};
	namespace Networking
	{
		struct s_network_game_client;
	};

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
		};
	};

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

		struct s_slayer_globals
		{
		}; //BOOST_STATIC_ASSERT( sizeof(s_slayer_globals) == 0x );

		struct s_oddball_globals
		{
			int32 score_to_win;
			int32 team_score[Enums::k_multiplayer_maximum_players];
			int32 individual_score[Enums::k_multiplayer_maximum_players];
			int32 undocumented1[Enums::k_multiplayer_maximum_players];
			datum_index current_ball_owners[Enums::k_multiplayer_maximum_players]; // player_index
			uint32 current_ball_held_times[Enums::k_multiplayer_maximum_players]; // game-ticks
		}; //BOOST_STATIC_ASSERT( sizeof(s_slayer_globals) == 0x );

		struct s_king_globals
		{
		}; //BOOST_STATIC_ASSERT( sizeof(s_king_globals) == 0x1B0 );

		struct s_race_globals
		{
		}; //BOOST_STATIC_ASSERT( sizeof(s_race_globals) == 0x );



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
	};
};