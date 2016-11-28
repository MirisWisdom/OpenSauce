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

		struct s_path
		{
			bool valid;															//	0x0
			PAD(1, 3);															//	0x1
			byte destination[20];												//	0x4
			PAD8; // unknown field												//	0x18
			PAD8; // unknown field												//	0x19
			PAD8; // unknown field												//	0x1A
			PAD8;
			PAD(2, 16);															//	0x1C
			PAD(3, 16);															//	0x2C
			PAD(4, 16);															//	0x3C
			PAD(5, 16);															//	0x4C
		}; BOOST_STATIC_ASSERT( sizeof(s_path) == 0x5C );

		struct s_actor_control_direction_specification
		{
			_enum type;															//	0x0
			PAD16;
			datum_index prop_index;												//	0x4
			PAD(1, 6);
			PAD8; // unknown field												//	0xE
			PAD8; // unknown field												//	0xF
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_control_direction_specification) == 0x10 );

		struct s_actor_control_data
		{
			PAD16; // unknown field												//	0x0
			PAD16;
			PAD32; // unknown field												//	0x4
			PAD(1, 12);

			struct
			{
				struct
				{
					datum_index	ignore_target_object_index;						//	0x14
					bool at_destination;										//	0x18
					PAD24;
				} destination_orders;

				PAD_TYPE(real_vector3d); // unknown field						//	0x1C
				PAD32; // unknown field											//	0x28
				PAD32; // unknown field											//	0x2C
				PAD32;
				PAD32; // unknown field											//	0x34
				PAD8; // unknown field											//	0x38
				PAD24;
				s_path path;													//	0x3C
			} path;

			PAD8; // unknown field												//	0x98
			PAD8; // unknown field												//	0x99
			PAD8; // unknown field												//	0x9A
			PAD8; // unknown field												//	0x9B
			PAD8; // unknown field												//	0x9C
			PAD8;
			PAD16; // unknown field												//	0x9E
			PAD_TYPE(real_point3d); // unknown field							//	0xA0
			PAD_TYPE(real_vector3d); // unknown field							//	0xAC
			PAD(2, 12);
			PAD8; // unknown field												//	0xC4
			PAD24;
			PAD32; // unknown field												//	0xC8
			PAD32; // unknown field												//	0xCC
			PAD32; // unknown field												//	0xD0
			PAD32; // unknown field												//	0xD4
			int16 secondary_look_type;											//	0xD8
			PAD16;
			PAD16; // unknown field												//	0xDC
			PAD16;
			s_actor_control_direction_specification secondary_look_direction;	//	0xE0
			bool idle_major_active;												//	0xF0
			PAD8; // unknown field												//	0xF1
			PAD8; // unknown field												//	0xF2
			bool idle_minor_active;												//	0xF3
			PAD32; // unknown field												//	0xF4
			int32 idle_major_timer;												//	0xF8
			int32 idle_minor_timer;												//	0xFC
			s_actor_control_direction_specification idle_major_direction;		//	0x100
			s_actor_control_direction_specification idle_minor_direction;		//	0x110
			PAD8; // unknown field												//	0x120
			PAD8; // unknown field												//	0x121
			PAD8; // unknown field												//	0x122
			PAD8; // unknown field												//	0x123
			PAD8; // unknown field												//	0x124
			PAD8; // unknown field												//	0x125
			PAD(3, 6);
			PAD_TYPE(real_vector3d); // unknown field							//	0x12C
			real_vector3d  desired_facing_vector;								//	0x138
			PAD_TYPE(real_vector3d); // unknown field							//	0x144
			PAD_TYPE(real_vector3d); // unknown field							//	0x150
			PAD(4, 16); // unknown field										//	0x15C
			PAD16; // unknown field												//	0x16C
			PAD16;
			PAD_TYPE(real_vector3d); // unknown field							//	0x170
			PAD(5, 8);
			PAD16; // unknown field												//	0x184
			int16 fire_state;													//	0x186
			PAD16; // unknown field												//	0x188
			PAD16; // unknown field												//	0x18A
			PAD16; // unknown field												//	0x18C
			PAD16; // unknown field												//	0x18E
			PAD16; // unknown field												//	0x190
			PAD16; // unknown field												//	0x192
			PAD8; // unknown field												//	0x194
			PAD8; // unknown field												//	0x195
			PAD8; // unknown field												//	0x196
			PAD8; // unknown field												//	0x197
			PAD8; // unknown field												//	0x198
			PAD24;
			PAD32; // unknown field												//	0x19C
			int16 current_fire_target_type;										//	0x1A0
			PAD16;
			datum_index	current_fire_target_prop_index;							//	0x1A4
			PAD(6, 8);
			PAD32; // unknown field												//	0x1B0
			PAD8; // unknown field												//	0x1B4
			PAD8; // unknown field												//	0x1B5
			PAD8; // unknown field												//	0x1B6
			PAD8; // unknown field												//	0x1B7
			PAD8; // unknown field												//	0x1B8
			PAD8;
			PAD16; // unknown field												//	0x1BA
			PAD8; // unknown field												//	0x1BC
			PAD24;
			PAD_TYPE(real_vector3d); // unknown field							//	0x1C0
			PAD32; // unknown field												//	0x1CC
			real_vector3d  fire_target_aim_vector;								//	0x1D0
			PAD32; // unknown field												//	0x1DC
			PAD_TYPE(real_vector3d); // unknown field							//	0x1E0
			PAD_TYPE(real_point3d); // unknown field							//	0x1EC
			PAD(7, 12);
			PAD_TYPE(real_point3d); // unknown field							//	0x204
			PAD_TYPE(real_point3d); // unknown field							//	0x210
			PAD8; // unknown field												//	0x21C
			PAD24;
			real_vector3d  burst_aim_vector;									//	0x220
			PAD(8, 8);
			PAD8; // unknown field												//	0x234
			PAD24;
			PAD32; // unknown field												//	0x238
			PAD_TYPE(real_vector3d); // unknown field							//	0x23C
			PAD32; // unknown field												//	0x248
			PAD(9, 20);
			PAD8; // unknown field												//	0x260
			PAD8;
			PAD16; // unknown field												//	0x262
			struct
			{
				word_flags	control_flags;										//	0x264
				PAD16;
				int16	persistent_control_ticks;								//	0x268
				PAD16;
				long_flags	persistent_control_flags;							//	0x26C
				PAD16; // unknown field											//	0x270
				PAD16;
				real_vector3d  throttle;										//	0x274
				PAD16; // unknown field											//	0x280
				PAD16;
				PAD_TYPE(real_point2d); // unknown field						//	0x284
				sbyte	aiming_speed;											//	0x28C
				PAD24;
				real_vector3d  facing_vector;									//	0x290
				real_vector3d  aiming_vector;									//	0x29C
				real_vector3d  looking_vector;									//	0x2A8
				datum_index	primary_trigger;									//	0x2B4
			} output;
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
			Enums::actor_default_state current_state;
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