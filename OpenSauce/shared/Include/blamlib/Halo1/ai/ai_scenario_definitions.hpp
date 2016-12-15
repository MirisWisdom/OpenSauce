/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_actor_starting_locations = 32,
		};

		enum {
			_ai_atom_pause,
			_ai_atom_go_to,
			_ai_atom_go_to_and_face,
			_ai_atom_move_in_direction,
			_ai_atom_look,
			_ai_atom_animation_mode,
			_ai_atom_crouch,
			_ai_atom_shoot,
			_ai_atom_grenade,
			_ai_atom_vehicle,
			_ai_atom_running_jump,
			_ai_atom_targeted_jump,
			_ai_atom_script,
			_ai_atom_animate,
			_ai_atom_recording,
			_ai_atom_action,
			_ai_atom_vocalize,
			_ai_atom_targeting,
			_ai_atom_initiative,
			_ai_atom_wait,
			_ai_atom_loop,
			_ai_atom_die,
			_ai_atom_move_immediate,
			_ai_atom_look_random,
			_ai_atom_look_player,
			_ai_atom_look_object,
			_ai_atom_set_radius,
			_ai_atom_teleport,

			k_number_of_ai_atoms,
		};
	};

	namespace TagGroups
	{
		struct s_squad_definition
		{

		}; //BOOST_STATIC_ASSERT( sizeof(s_squad_definition)==0x18 );

		struct s_platoon_definition
		{

		}; //BOOST_STATIC_ASSERT( sizeof(s_platoon_definition)==0x18 );

		struct s_firing_position
		{
			real_point3d position;
			_enum group_index;
			int16 runtime_cluster_index;
			PAD32;
			int32 runtime_surface_index; // not valid if the encounter uses 3d firing positions
		}; BOOST_STATIC_ASSERT( sizeof(s_firing_position)==0x18 );

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
		}; BOOST_STATIC_ASSERT( sizeof(s_encounter_definition)==0xB0 );

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
		}; BOOST_STATIC_ASSERT( sizeof(s_ai_command)==0x20 );
		struct s_ai_command_point
		{
			real_point3d position;
			int32 runtime_surface_index;
			PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_ai_command_point)==0x14 );
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
		}; BOOST_STATIC_ASSERT( sizeof(s_ai_command_list)==0x60 );
	};
};