/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/ai/action_fight.hpp>
#include <blamlib/Halo1/ai/action_flee.hpp>
#include <blamlib/Halo1/ai/actions.hpp>
#include <blamlib/Halo1/ai/actor_combat.hpp>
#include <blamlib/Halo1/ai/actor_looking.hpp>
#include <blamlib/Halo1/ai/actor_moving.hpp>
#include <blamlib/Halo1/ai/actor_perception.hpp>
#include <blamlib/Halo1/ai/actor_stimulus.hpp>
#include <blamlib/Halo1/ai/actors.hpp>
#include <blamlib/Halo1/memory/data.hpp>
#include <blamlib/Halo1/game/game_allegiance.hpp>

namespace Yelo
{
	namespace AI
	{
		struct s_actor_meta_data
		{
			int16 type;
			bool swarm;
			PAD8; // unknown field
			bool active;
			bool encounterless;
			PAD8; // unknown field
			PAD8;
			PAD32; // unknown field
			PAD16; // unknown field
			PAD8; // unknown field
			bool dormant;
			PAD16; // unknown field
			PAD16;
			datum_index unit_index;
			PAD8; // unknown field
			PAD8;
			int16 swarm_unit_count;
			PAD16; // unknown field
			PAD16; // unknown field
			datum_index swarm_unit_index;
			datum_index swarm_cache_index;
			datum_index encounter_next_actor_index;
			PAD32; // unknown field
			datum_index encounter_index;
			PAD16; // unknown field
			int16 squad_index;
			int16 platoon_index;
			Enums::game_team team;
			PAD8; // unknown field
			PAD24;
			PAD32; // unknown field
			PAD16; // unknown field
			PAD16; // unknown field
			bool timeslice;
			PAD24;
			datum_index first_prop_index;
			PAD32; // unknown field
			datum_index actor_definition_index;
			datum_index actor_variant_definition_index;
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_meta_data) == 0x5C );

		struct s_actor_state_data
		{
			Enums::actor_action action;
			PAD16; // unknown field
			PAD8; // unknown field
			PAD8;
			PAD16; // unknown field
			PAD16; // unknown field
			PAD(1, 14);
			PAD32; // unknown field
			PAD32; // unknown field
			PAD8; // unknown field
			PAD8; // unknown field
			PAD8; // unknown field
			PAD8;
			PAD16; // unknown field
			PAD16;
			PAD32; // unknown field
			PAD8; // unknown field
			PAD8; // unknown field
			PAD16;
			byte action_data[132];
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_state_data) == 0xB4 );

		struct s_actor_input_data
		{
			PAD_TYPE(real_point3d); // unknown field
			PAD_TYPE(real_vector3d); // unknown field
			PAD(1, 32);
			datum_index vehicle_index;
			PAD8; // unknown field
			PAD8; // unknown field
			PAD16; // unknown field
			bool vehicle_passenger;
			bool vehicle_driver;
			PAD16;
			int32 surface_index;
			real_point3d position;
			real_vector3d facing_vector;
			real_vector3d aiming_vector;
			real_vector3d looking_vector;
			PAD(2, 24);
			PAD32; // unknown field
			PAD8; // unknown field
			bool burning_to_death;
			PAD16;
			real unit_damage_body;
			real unit_damage_shield;
			real unit_damage_body_recent;
			real unit_damage_shield_recent;
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_input_data) == 0xA8 );

		struct s_actor_target_data
		{
			PAD16; // unknown field
			PAD16;
			PAD32; // unknown field
			datum_index target_prop_index;
			PAD8; // unknown field
			PAD24;
			PAD16; // unknown field
			PAD16; // unknown field
			PAD8; // unknown field
			PAD8;
			PAD8; // unknown field
			PAD8; // unknown field
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_target_data) == 0x18 );

		struct s_actor_danger_zone_data
		{
			PAD(1, 104);
			PAD16; // unknown field
			PAD8; // unknown field
			PAD8; // unknown field
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_danger_zone_data) == 0x6C );

		struct s_actor_stimulus_data
		{
			PAD8; // unknown field
			PAD8;
			PAD16; // unknown field
			PAD32;
			PAD32; // unknown field
			PAD8; // unknown field
			PAD24;
			PAD_TYPE(real_vector3d); // unknown field
			PAD16; // unknown field
			PAD16;
			PAD32; // unknown field
			PAD16; // unknown field
			_enum combat_transition;
			PAD8; // unknown field
			PAD24;
			PAD_TYPE(real_vector3d); // unknown field
			PAD32; // unknown field
			PAD32; // unknown field
			PAD8; // unknown field
			PAD24;
			PAD_TYPE(real_vector3d); // unknown field
			PAD32; // unknown field
			datum_index combat_transition_prop_index;
			PAD32; // unknown field
			PAD8; // unknown field
			PAD8;
			PAD16; // unknown field
			PAD32; // unknown field
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_stimulus_data) == 0x64 );

		struct s_actor_emotion_data
		{
			PAD32; // unknown field
			PAD32; // unknown field
			PAD16; // unknown field
			PAD16; // unknown field
			PAD8; // unknown field
			PAD8; // unknown field
			PAD8; // unknown field
			PAD8; // unknown field
			PAD16; // unknown field
			PAD8; // unknown field
			PAD8; // unknown field
			int16 crouch_switching_change_timer;
			PAD16; // unknown field
			PAD16; // unknown field
			PAD16;
			PAD32; // unknown field
			PAD32; // unknown field
			PAD8; // unknown field
			PAD8; // unknown field
			bool ignorant;
			PAD8; // unknown field
			bool berserking;
			PAD8; // unknown field
			PAD16;
			PAD32; // unknown field
			PAD32; // unknown field
			PAD32; // unknown field
			PAD32; // unknown field
			PAD8; // unknown field
			PAD24;
			PAD32; // unknown field
			PAD32; // unknown field
			PAD32; // unknown field
			PAD32; // unknown field
			PAD32; // unknown field
			PAD32; // unknown field
			PAD16; // unknown field
			PAD16;
			PAD32; // unknown field
			PAD32; // unknown field
			PAD32; // unknown field
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_emotion_data) == 0x68 );

		struct s_actor_firing_position_data
		{
			PAD16; // unknown field
			PAD(1, 6);
			PAD32; // unknown field
			PAD(2, 32); // unknown field
			PAD16; // unknown field
			PAD8; // unknown field
			PAD8; // unknown field
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_firing_position_data) == 0x30 );

		struct s_actor_orders_data
		{
			PAD(1, 128); // unknown field
			PAD16; // unknown field
			PAD8; // unknown field
			PAD8; // unknown field
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_orders_data) == 0x84 );

		struct s_actor_control_data
		{
			DOC_TODO("Fill in this struct");
			PAD(1, 0x2B8);
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_control_data) == 0x2B8 );

		struct s_actor_datum
		{
			PAD16; // unknown field
			PAD16;
			s_actor_meta_data meta;
			Enums::actor_default_state initial_state;
			Enums::actor_default_state return_state;
			PAD32; // unknown field
			PAD16;
			_enum state_flags;
			s_actor_state_data state;
			s_actor_input_data input;
			PAD8; // unknown field
			PAD8; // unknown field
			PAD8; // unknown field
			PAD8;
			PAD8; // unknown field
			PAD24;
			PAD32; // unknown field
			PAD16; // unknown field
			PAD32;PAD16;
			PAD32; // unknown field
			PAD_TYPE(datum_index); // unknown field
			_enum postcombat_state;
			PAD16;
			PAD32; // unknown field
			byte situation[123];
			PAD8; // unknown field
			s_actor_target_data target;
			s_actor_danger_zone_data danger_zone;
			s_actor_stimulus_data stimulus;
			s_actor_emotion_data emotions;
			s_actor_firing_position_data firing_positions;
			s_actor_orders_data orders;
			s_actor_control_data control;
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_datum) == 0x724 );

		typedef Memory::DataArray<s_actor_datum, 256> actor_data_t;

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

		typedef Memory::DataArray<	s_swarm_datum, Enums::k_maximum_number_of_active_swarms> swarm_data_t;

		struct s_swarm_component_datum : TStructImpl(64)
		{
		};

		typedef Memory::DataArray<s_swarm_component_datum, 256> swarm_component_data_t;
		
		actor_data_t&					Actors();

		swarm_data_t&					Swarms();

		swarm_component_data_t&			SwarmComponents();
	};
};