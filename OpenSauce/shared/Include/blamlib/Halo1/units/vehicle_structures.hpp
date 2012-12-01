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
	};

	namespace Flags
	{
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
			// 0x4D4, DWORD
			// 0x4D8, DWORD
			// 0x4DC, DWORD
			// 0x4E0, DWORD
			// 0x4E4, DWORD
			// 0x4E8, DWORD
			// 0x4EC, DWORD
			// 0x4F0, DWORD
			// 0x4F4, sizeof(0x8)
			TStructSubGetPtrImpl(real_point3d,					HoveringPosition, 0x4FC);
			// 0x508, DWORD
			// 0x50C, DWORD
			// 0x510, DWORD
			// 0x514, DWORD
			// 0x518, DWORD
			// 0x51C, DWORD
			// 0x520, DWORD
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
			s_vehicle_data vehicle;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_datum) == Enums::k_object_size_vehicle );
	};
};