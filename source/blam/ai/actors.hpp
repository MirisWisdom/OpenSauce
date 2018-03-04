/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/memory/datum_index.hpp>
#include <blamlib/ai/actions.hpp>

namespace Yelo
{
	namespace AI
	{
		enum
		{
			k_maximum_number_of_units_per_swarm = 16,

			k_maximum_number_of_active_swarms = 32,
		};

		namespace e_actor_default_state
		{
			typedef enum : _enum
			{
				none,
				sleeping,
				alert,
				moving_repeat_same_position,
				moving_loop,
				moving_loop_back_and_forth,
				moving_loop_randomly,
				moving_randomly,
				guarding,
				guarding_at_guard_position,
				searching,
				fleeing,

				k_count
			} type_t;
		}

		namespace e_actor_fire_target
		{
			typedef enum : _enum
			{
				prop = 1,
				manual_point = 2,

				k_count = 2
			} type_t;
		}

		namespace e_actor_acknowledgement
		{
			typedef enum : _enum
			{
				never,
				combat,
				instant,
				searching,
				definite,

				k_count
			} type_t;
		}

		namespace e_actor_knowledge_type
		{
			typedef enum : _enum
			{
				noncombat0,
				guard1,
				guard2,
				noncombat3,

				k_count
			} type_t;
		}
	}

	namespace blam
	{
		extern cstring g_ai_default_state_names[AI::e_actor_default_state::k_count];

		// Deletes and detaches the specified actor from all AI (encounters, etc.)
		void PLATFORM_API actor_delete(
			const datum_index actor_index,
			const bool is_dead = false);

		void PLATFORM_API actor_update(
			const datum_index actor_index);

		void PLATFORM_API actor_customize_unit(
			const datum_index actor_variant,
			const datum_index unit_index);

		void PLATFORM_API actor_set_active(
			const datum_index actor_index,
			const bool active);

		void PLATFORM_API actor_set_dormant(
			const datum_index actor_index,
			const bool dormant);

		void PLATFORM_API actor_delete_props(
			const datum_index actor_index);

		void PLATFORM_API actor_freeze(
			const datum_index actor_index);

		void PLATFORM_API actor_braindead(
			const datum_index actor_index,
			const bool braindead);

		datum_index PLATFORM_API actor_create_for_unit(
			const bool is_swarm,
			const datum_index unit_index,
			const datum_index actor_variant,
			const datum_index encounter_index,
			const int32 squad_index,
			const int32 arg7,
			const int32 arg6,
			const bool magic_sight_after_timer,
			const AI::e_actor_default_state::type_t initial_state,
			const AI::e_actor_default_state::type_t return_state,
			const int32 command_list_index,
			const int32 sequence_id);

		void PLATFORM_API actor_action_change(
			const datum_index actor_index,
			const AI::e_actor_action::type_t new_action_type,
			const byte* const new_action_data);
	}
}
