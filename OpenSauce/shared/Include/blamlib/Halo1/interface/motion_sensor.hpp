/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_allegiance.hpp> // for game teams
#include <blamlib/Halo1/game/game_configuration.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_max_custom_blips = 16, // MAX_CUSTOM_BLIPS
		};

		enum blip_type : byte_enum
		{
			_blip_type_local_player,			// 1, .5, 0
			_blip_type_friendly_team,			// 1, 1, 0
			_blip_type_enemy_team,				// 1, 0, 0
			_blip_type_friendly_team_vehicle,	// 1, 1, 0
			_blip_type_enemy_team_vehicle,		// 1, 0, 0
			_blip_type_objective,				// .5, .5, 1
			_blip_type_none,					// 0, 0, 0
		};
	};

	namespace GameUI
	{
		struct s_motion_sensor
		{
			struct s_custom_blip
			{
				sbyte x;
				sbyte y;
			};
			struct s_blip : s_custom_blip
			{
				Enums::blip_type type; // set to _blip_type_none when not used
				byte_enum size;	// a la object's size (tiny, large, etc)
			}; BOOST_STATIC_ASSERT( sizeof(s_blip) == 0x4 );
			struct s_team_data
			{
				s_blip object_blips[Enums::k_max_custom_blips];			// objects belonging to just this team
				struct {
					s_custom_blip goal_blips[Enums::k_max_custom_blips];
					sbyte goal_indices[Enums::k_max_custom_blips];		// indices to the game engine's global goals
				}game_engine;

				real_point2d world_position;
				UNKNOWN_TYPE(int32); // game time related
				UNKNOWN_TYPE(real); // related to calculations with objects and facing angle of the player
				sbyte active_object_blips_count;
				PAD24;
			}; BOOST_STATIC_ASSERT( sizeof(s_team_data) == 0x84 );
			struct s_local_player
			{
				s_team_data nearby_team_objects[Enums::k_number_of_game_teams];
				datum_index nearby_object_indexes[Enums::k_max_custom_blips];
			}; BOOST_STATIC_ASSERT( sizeof(s_local_player) == 0x568 );


			s_local_player local_players[Enums::k_maximum_number_of_local_players];

			struct {
				UNKNOWN_TYPE(int32); // game time related
				int16 current_object_blip_index;
				PAD8; // actually boolean, but you don't need to worry about this value!
				PAD8;
			}update_data;
		}; BOOST_STATIC_ASSERT( sizeof(s_motion_sensor) == 0x570 );
		s_motion_sensor*			MotionSensor();
	};
};