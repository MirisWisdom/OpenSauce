/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>

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

		enum actor_target_type : _enum
		{
			k_number_of_actor_target_types = 12
		};

		enum actor_danger_zone : _enum
		{
			_actor_danger_zone_none,
			_actor_danger_zone_suicide,
			_actor_danger_zone_projectile,
			_actor_danger_zone_vehicle,
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

		enum actor_perception_type : _enum
		{
			_actor_perception_type_none,
			_actor_perception_type_partial,
			_actor_perception_type_full,
			_actor_perception_type_unmistakable,

			k_number_of_actor_perception_types,
		};

		enum actor_movement_state : _enum
		{
			_actor_movement_state_noncombat,
			_actor_movement_state_asleep,
			_actor_movement_state_combat,
			_actor_movement_state_flee,
		};

		// Move to actor_looking.hpp
		enum secondary_look_type {
			_secondary_look_type_none,
			_secondary_look_type_environment,
			_secondary_look_type_moving_prop,
			_secondary_look_type_impact,
			_secondary_look_type_new_prop,
			_secondary_look_type_bumped,
			_secondary_look_type_detonation,
			_secondary_look_type_shooting_prop,
			_secondary_look_type_comm_prop,
			_secondary_look_type_comm_direction,
			_secondary_look_type_combat_prop,
			_secondary_look_type_damage,
			_secondary_look_type_danger,
			_secondary_look_type_scripted,

			k_number_of_secondary_look_types
		};
	};

	namespace AI
	{
		struct s_swarm_datum : Memory::s_datum_base
		{
			int16 unit_count;			// 0x2
			datum_index actor_index;	// 0x4
			UNKNOWN_TYPE(int16);		// 0x8
			PAD16;
			UNKNOWN_TYPE(real_vector3d);// 0xC
			datum_index unit_indices[Enums::k_maximum_number_of_units_per_swarm];
			datum_index component_indices[Enums::k_maximum_number_of_units_per_swarm];
		}; BOOST_STATIC_ASSERT( sizeof(s_swarm_datum) == 0x98 );
		typedef Memory::DataArray<	s_swarm_datum, 
									Enums::k_maximum_number_of_active_swarms> 
			swarm_data_t;
	};

	namespace blam
	{
		// Deletes and detaches the specified actor from all AI (encounters, etc.)
		void PLATFORM_API actor_delete(datum_index actor_index, bool is_dead = false);
	};
};