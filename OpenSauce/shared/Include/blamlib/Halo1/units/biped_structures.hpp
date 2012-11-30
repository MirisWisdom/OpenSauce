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
	};
};