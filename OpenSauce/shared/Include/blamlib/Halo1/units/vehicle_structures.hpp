/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/units/unit_structures.hpp>
#include <blamlib/Halo1/units/vehicle_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
	};

	namespace Flags
	{
		enum vehicle_flags
		{
			_vehicle_unk0_bit, // ~blur related
			_vehicle_hovering_bit,
			_vehicle_crouched_bit,
			_vehicle_jumping_bit,
			_vehicle_unk4_bit, // flip related
		};
	};

	namespace Objects
	{
		struct s_vehicle_datum_network_data
		{
			bool at_rest_bit;
			PAD24;
			real_point3d position;
			real_vector3d transitional_velocity;
			real_vector3d angular_velocity;
			real_vector3d forward;
			real_vector3d up;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_datum_network_data) == 0x40 );

		struct s_vehicle_data : TStructImpl(Enums::k_object_size_vehicle - Enums::k_object_size_unit)
		{
			enum { DATA_OFFSET = Enums::k_object_size_unit, };

			TStructSubGetPtrImpl(word_flags,					Flags, 0x4CC);
			// 0x4CE, WORD
			// 0x4D0, BYTE
			// 0x4D1, BYTE
			// 0x4D2, BYTE
			// 0x4D3, BYTE
			TStructSubGetPtrImpl(real,							Speed, 0x4D4);
			TStructSubGetPtrImpl(real,							Slide, 0x4D8);
			TStructSubGetPtrImpl(real,							Turn, 0x4DC);
			TStructSubGetPtrImpl(real,							TirePosition, 0x4E0);
			TStructSubGetPtrImpl(real,							TreadPositionLeft, 0x4E4);
			TStructSubGetPtrImpl(real,							TreadPositionRight, 0x4E8);
			TStructSubGetPtrImpl(real,							Hover, 0x4EC);
			TStructSubGetPtrImpl(real,							Thrust, 0x4F0);
			// 0x4F4, sbyte suspension states[Enums::k_max_vehicle_suspensions]?
			TStructSubGetPtrImpl(real_point3d,					HoveringPosition, 0x4FC);
			// 0x508, real_vector3d
			// 0x514, real_vector3d
			// 0x520, DWORD
			//////////////////////////////////////////////////////////////////////////
			// Added in HaloPC
			TStructSubGetPtrImpl(bool,							NetworkTimeValid, 0x524);
			TStructSubGetPtrImpl(bool,							BaselineValid, 0x525);
			TStructSubGetPtrImpl(byte,							BaselineIndex, 0x526);
			TStructSubGetPtrImpl(byte,							MessageIndex, 0x527);
			TStructSubGetPtrImpl(s_vehicle_datum_network_data,	UpdateBaseline, 0x528);
			// 0x568 unused?
			TStructSubGetPtrImpl(s_vehicle_datum_network_data,	UpdateDelta, 0x56C);
			// 0x5AC, uint32, game time related
			// block index of the scenario datum used for respawning
			// For all game engines besides race, this will be a scenario vehicle datum
			// For race, it's a scenario_netpoint, aka "scenario_netgame_flags_block"
			TStructSubGetPtrImpl(int16,							ScenarioDatumRespawningIndex, 0x5B0);
			// 0x5B2 PAD16
			// 0x5B4, real_point3d, position based
		};


		struct s_vehicle_datum : s_unit_datum
		{
			enum { k_object_types_mask = FLAG(Enums::_object_type_vehicle) };

			s_vehicle_data vehicle;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_datum) == Enums::k_object_size_vehicle );
	};
};