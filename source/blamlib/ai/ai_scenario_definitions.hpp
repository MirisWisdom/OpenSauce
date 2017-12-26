/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/math/real_math.hpp>
#include <blamlib/tag_files/tag_groups_base.hpp>
#include <YeloLib/tag_files/tag_groups_markup.hpp>
#include <blamlib/tag_files/tag_block.h>

namespace Yelo
{
	namespace AI
	{
		enum
		{
			k_maximum_actor_starting_locations = 32,
		};

		namespace e_ai_atom
		{
			typedef enum : _enum
			{
				pause,
				go_to,
				go_to_and_face,
				move_in_direction,
				look,
				animation_mode,
				crouch,
				shoot,
				grenade,
				vehicle,
				running_jump,
				targeted_jump,
				script,
				animate,
				recording,
				action,
				vocalize,
				targeting,
				initiative,
				wait,
				loop,
				die,
				move_immediate,
				look_random,
				look_player,
				look_object,
				set_radius,
				teleport,

				k_count
			} type_t;
		}
	}

	namespace TagGroups
	{
		struct s_squad_definition { }; //BOOST_STATIC_ASSERT( sizeof(s_squad_definition)==0x18 );

		struct s_platoon_definition { }; //BOOST_STATIC_ASSERT( sizeof(s_platoon_definition)==0x18 );

		struct s_firing_position
		{
			real_point3d position;
			_enum group_index;
			int16 runtime_cluster_index;
			PAD32;
			int32 runtime_surface_index; // not valid if the encounter uses 3d firing positions
		};

		BOOST_STATIC_ASSERT( sizeof(s_firing_position)==0x18 );

		struct s_encounter_definition
		{
			tag_string name;
			long_flags flags;
			TAG_ENUM(team_index, Enums::game_team);
			UNKNOWN_TYPE(int16); // set to 1 once postprocessed. not seeing any other references past that
			_enum search_behavior;
			int16 manual_structure_reference_index;
			real_bounds respawn_delay;
			TAG_PAD(byte, 74);
			int16 runtime_structure_reference_index;
			TAG_BLOCK(squads, s_squad_definition);
			TAG_BLOCK(platoons, s_platoon_definition);
			TAG_BLOCK(firing_positions, s_squad_definition);
			TAG_BLOCK(starting_locations, scenario_player);
		};

		BOOST_STATIC_ASSERT( sizeof(s_encounter_definition)==0xB0 );

		struct s_ai_command
		{
			_enum type;
			int16 atom_modifier;
			real parameter1;
			real parameter2;
			int16 point1_index;
			int16 point2_index;
			int16 ai_animation_index;
			int16 ai_script_index;
			int16 ai_recording_index;
			int16 command_index;
			int16 object_name_index;
			PAD16;
			PAD32;
		};

		BOOST_STATIC_ASSERT( sizeof(s_ai_command)==0x20 );

		struct s_ai_command_point
		{
			real_point3d position;
			int32 runtime_surface_index;
			PAD32;
		};

		BOOST_STATIC_ASSERT( sizeof(s_ai_command_point)==0x14 );

		struct s_ai_command_list
		{
			tag_string name;
			long_flags flags;
			PAD32;
			PAD32;
			int16 manual_structure_reference_index;
			int16 runtime_structure_reference_index;
			TAG_BLOCK(commands, s_ai_command);
			TAG_BLOCK(points, s_ai_command_point);
			TAG_PAD(tag_block, 2);
		};

		BOOST_STATIC_ASSERT( sizeof(s_ai_command_list)==0x60 );
	}
}
