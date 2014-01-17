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
	};
};