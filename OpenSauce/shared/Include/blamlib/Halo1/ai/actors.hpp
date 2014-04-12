/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_number_of_units_per_swarm = 16,

			k_maximum_number_of_active_swarms = 32,
		};

		enum actor_default_state
		{
			_actor_default_state_none,
			_actor_default_state_sleeping,
			_actor_default_state_alert,
			_actor_default_state_moving_repeat_same_position,
			_actor_default_state_moving_loop,
			_actor_default_state_moving_loop_back_and_forth,
			_actor_default_state_moving_loop_randomly,
			_actor_default_state_moving_randomly,
			_actor_default_state_guarding,
			_actor_default_state_guarding_at_guard_position,
			_actor_default_state_searching,
			_actor_default_state_fleeing,

			k_number_of_actor_default_states,
		};

		enum actor_fire_target : _enum
		{
			_actor_fire_target_prop = 1,
			_actor_fire_target_manual_point = 2,
		};

		enum actor_acknowledgement : _enum
		{
			_actor_acknowledgement_never,
			_actor_acknowledgement_combat,
			_actor_acknowledgement_instant,
			_actor_acknowledgement_searching,
			_actor_acknowledgement_definite,
		};

		enum actor_knowledge_type : _enum
		{
			_actor_knowledge_type_noncombat0,
			_actor_knowledge_type_guard1,
			_actor_knowledge_type_guard2,
			_actor_knowledge_type_noncombat3,

			k_number_of_actor_knowledge_types,
		};
	};

	namespace AI
	{
	};

	namespace blam
	{
		// Deletes and detaches the specified actor from all AI (encounters, etc.)
		void PLATFORM_API actor_delete(datum_index actor_index, bool is_dead = false);
	};
};