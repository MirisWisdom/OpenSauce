/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/main/main.hpp>

namespace Yelo
{
	namespace GameState
	{
		struct s_main_globals
		{
			UNKNOWN_TYPE(uint32); // time related
			PAD32;
			LARGE_INTEGER performance_counter;
			UNKNOWN_TYPE(bool);
			bool is_taking_screenshot;
			PAD16;
			real delta_time;
			Enums::game_connection game_connection;

			struct s_screenshot {
				int16 counter;
				void* movie; // screenshot bitmap
				PAD32; PAD32;
				int32 movie_frame_index;
				UNKNOWN_TYPE(real);
			}screenshot;

			struct s_map {
				bool reset_map;
				bool switch_to_campaign;
				bool revert_map;
				bool skip_cinematic;
				bool save_map;
				bool save_map_nonsafe;
				bool save_map_with_timeout;
				bool is_saving_map;
				int32 saving_map_timeout_countdown;
				int32 saving_map_timeout_timer;
				UNKNOWN_TYPE(int32);
				UNKNOWN_TYPE(int16);
				bool won_map;
				bool lost_map;
				bool respawn_coop_players;

				struct s_core {
					bool save;
					bool load;
					bool load_at_startup;
				}core;

				int16 switch_to_structure_bsp; // if not NONE, switches to the scenario's bsp by index
				bool main_menu_scenario_loaded;
				bool main_menu_scenario_load;
			}map;

			UNUSED_TYPE(bool);
			UNKNOWN_TYPE(bool);
			UNKNOWN_TYPE(bool);
			bool quit;
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			bool set_game_connection_to_film_playback;
			bool time_is_stopped;
			bool start_time;
			UNUSED_TYPE(bool);

			bool skip_frames;
			PAD8;
			int16 skip_frames_count;

			int16 lost_map_count;
			int16 respawn_count;

			UNKNOWN_TYPE(bool);
			PAD24;
			UNKNOWN_TYPE(bool);
			char scenario_tag_path[256];
			char multiplayer_map_name[256];
			char queued_map[256];

			UNKNOWN_TYPE(bool);
			UNKNOWN_TYPE(tag_string);
			PAD(0, 8+1); // char[8+1]
			PAD8;
			PAD32;

			void QuitToMainMenu();
		}; BOOST_STATIC_ASSERT( sizeof(s_main_globals) == 0x3A0 );
	};
};