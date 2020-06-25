/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>

#include <blamlib/Halo1/effects/effects_constants.hpp>
#include <YeloLib/Halo1/effects/effects_constants_yelo.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum particle_flags : word_flags
		{
			_particle_animating_backwards_bit,
			_particle_unk1_bit,		// at rest ?
			_particle_unk2_bit,		// horizontally mirrored
			_particle_unk3_bit,		// vertically mirrored

			_particle_unk4_bit,
			_particle_unk5_bit,
			_particle_unk6_bit,
			_particle_unk7_bit,

			_particle_unk8_bit,		// create in third person
			_particle_unk9_bit,		// create in first person
			_particle_unk10_bit,	// node is first person

		};
	}

	namespace Effects
	{

		struct s_particle_placement_data
		{
			datum_index definition_index;				// 0x0
			datum_index object_index;					// 0x4
			int16 node_index;							// 0x8
			int16 local_player_index;					// 0xA
			bool is_node_first_person;					// 0xC
			bool is_third_person_only;					// 0xD
			bool is_first_person_only;					// 0xE
			PAD8;										// 0xF
			real_vector3d offset;						// 0x10
			real_vector3d direction;					// 0x1C
			real_vector3d velocity;						// 0x28
			real_vector3d scales;						// 0x34 multiplied by animation radius, never used ?
			real initial_angle;							// 0x40
			real angular_velocity;						// 0x44
			real radius;								// 0x48
			real_argb_color tint;						// 0x4C

		}; BOOST_STATIC_ASSERT( sizeof(s_particle_placement_data) == 0x5C ); // guessed size
		
		//////////////////////////////////////////////////////////////////////////
		// game-state: particles
		struct s_particle_datum : Memory::s_datum_base
		{
			word_flags flags;						// 0x2
			datum_index definition_index;			// 0x4
			datum_index object_index;				// 0x8
			int16 node_index;						// 0xC
			UNKNOWN_TYPE(byte);						// 0xE some sort of staging, init -> loop -> final
			byte local_player_index;				// 0xF
			UNKNOWN_TYPE(int32);					// 0x10 frame counter (for rendering ?)
			real life;								// 0x14
			real lifespan;							// 0x18
			UNKNOWN_TYPE(real);						// 0x1C animation (initialized to -1.0f which means needs sprite to be advanced)
			real animation_rate;					// 0x20
			int16 sequence_index;					// 0x24
			int16 sprite_index;						// 0x26
			s_scenario_location location;			// 0x28
			real_point3d position;					// 0x30
			real_point3d forward;					// 0x3C 
			real_point3d velocity;					// 0x48
			real angle;								// 0x54
			real angular_velocity;					// 0x58
			real radius;							// 0x5C
			real_argb_color tint;					// 0x60

		}; BOOST_STATIC_ASSERT( sizeof(s_particle_datum) == 0x70 );
		typedef Memory::DataArray<	s_particle_datum, 
									Enums::k_maximum_number_of_particles_per_map,
									Enums::k_maximum_number_of_particles_per_map_upgrade> 
			particle_data_t;
	};
};