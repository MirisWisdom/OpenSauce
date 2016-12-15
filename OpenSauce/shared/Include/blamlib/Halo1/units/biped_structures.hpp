/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/units/unit_structures.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum biped_movement_state : byte_enum
		{
			_biped_movement_state_moving,
			_biped_movement_state_idle_or_turning,
			_biped_movement_state_gesturing,
		};
	};

	namespace Flags
	{
		enum biped_flags
		{
			_biped_airborne_bit,
			_biped_slipping_bit,
			_biped_absolute_movement_bit,	// physics related. set by ai_command:animate code (when modifier has "absolute movement")
			_biped_no_collision_bit,		// physics related. set by ai_command:animate code (when modifier has "no collision")
			_biped_passes_through_other_bipeds_bit,
			_biped_limping_bit,
		};
	};

	namespace Objects
	{
		struct s_biped_datum_network_data
		{
			byte grenade_counts[Enums::k_unit_grenade_types_count];
			PAD16;
			real body_vitality, shield_vitality;
			bool shield_stun_ticks_greater_than_zero;
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_biped_datum_network_data) == 0x10 );

		struct s_biped_data
		{
			long_flags flags;								// 0x4CC
			UNKNOWN_TYPE(byte);								// 0x4D0
			UNKNOWN_TYPE(byte);								// 0x4D1
			Enums::biped_movement_state movement_state;		// 0x4D2
			UNKNOWN_TYPE(byte);								// 0x4D3
			UNKNOWN_TYPE(int32);							// 0x4D4
			UNKNOWN_TYPE(int32);							// 0x4D8
			UNKNOWN_TYPE(int32);							// 0x4DC
			UNKNOWN_TYPE(real_point3d);						// 0x4E0
			UNKNOWN_TYPE(int32);							// 0x4EC
			UNKNOWN_TYPE(int32);							// 0x4F0
			UNKNOWN_TYPE(int32);							// 0x4F4
			UNKNOWN_TYPE(int32);							// 0x4F8
			datum_index bump_source_unit_index;				// 0x4FC
			sbyte ticks_since_bump;							// 0x500
			sbyte airborne_ticks;							// 0x501
			sbyte slipping_ticks;							// 0x502
			UNKNOWN_TYPE(byte);								// 0x503 sbyte
			UNKNOWN_TYPE(byte);								// 0x504 sbyte
			sbyte melee_animation_time;						// 0x505 sbyte timer, melee related
			sbyte melee_cause_damage_time;					// 0x506 sbyte timer, melee related
			PAD8;
			UNKNOWN_TYPE(int16);							// 0x508
			PAD16;
			UNKNOWN_TYPE(real);								// 0x50C
			UNKNOWN_TYPE(real);								// 0x510
			UNKNOWN_TYPE(real_plane3d);						// 0x514 physics related
			UNKNOWN_TYPE(byte);								// 0x524 sbyte
			UNKNOWN_TYPE(byte);								// 0x525 sbyte
			//////////////////////////////////////////////////////////////////////////
			// Added in HaloPC
			bool baseline_valid;							// 0x526
			sbyte baseline_index;							// 0x527
			sbyte message_index;							// 0x528
			PAD24;
			s_biped_datum_network_data update_baseline;		// 0x52C
			UNKNOWN_TYPE(bool);								// 0x53C probably delta_valid. engine only writes to this, never reads. consider it 'unused'
			PAD24;
			s_biped_datum_network_data update_delta;		// 0x540
		}; BOOST_STATIC_ASSERT( sizeof(s_biped_data) == (Enums::k_object_size_biped - Enums::k_object_size_unit) );


		struct s_biped_datum : s_unit_datum
		{
			enum { k_object_types_mask = FLAG(Enums::_object_type_biped) };

			s_biped_data biped;
		}; BOOST_STATIC_ASSERT( sizeof(s_biped_datum) == Enums::k_object_size_biped );
	};
};