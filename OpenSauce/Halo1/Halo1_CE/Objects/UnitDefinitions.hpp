/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/units/biped_structures.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>
#include <blamlib/Halo1/units/vehicle_structures.hpp>

#include "Objects/ObjectDefinitions.hpp"

namespace Yelo
{
	namespace Objects
	{
		struct s_biped_data : TStructImpl(Enums::k_object_size_biped - Enums::k_object_size_unit)
		{
			enum { DATA_OFFSET = Enums::k_object_size_unit, };

			TStructSubGetPtrImpl(long_flags,					Flags, 0x4CC);
			//TStructSubGetPtrImpl(byte,							, 0x4D0);
			//TStructSubGetPtrImpl(byte,							, 0x4D1);
			TStructSubGetPtrImpl(Enums::biped_movement_state,	MovementState, 0x4D2);
			//TStructSubGetPtrImpl(byte,							, 0x4D3);
			//TStructSubGetPtrImpl(int32,							, 0x4D4);
			//TStructSubGetPtrImpl(int32,							, 0x4D8);
			//TStructSubGetPtrImpl(int32,							, 0x4DC);
			//TStructSubGetPtrImpl(real_point3d,					, 0x4E0);
			//TStructSubGetPtrImpl(int32,							, 0x4EC);
			//TStructSubGetPtrImpl(int32,							, 0x4F0);
			//TStructSubGetPtrImpl(int32,							, 0x4F4);
			//TStructSubGetPtrImpl(int32,							, 0x4F8);
			TStructSubGetPtrImpl(datum_index,					BumpSourceUnitIndex, 0x4FC); // unit that bumped into (or possessed) us
			TStructSubGetPtrImpl(sbyte,							TicksSinceBump, 0x500);
			TStructSubGetPtrImpl(sbyte,							AirborneTicks, 0x501);
			TStructSubGetPtrImpl(sbyte,							SlippingTicks, 0x502);
			//TStructSubGetPtrImpl(sbyte,							, 0x503);
			//TStructSubGetPtrImpl(sbyte,							, 0x504);
			//TStructSubGetPtrImpl(sbyte,							, 0x505); // timer, melee related
			//TStructSubGetPtrImpl(sbyte,							, 0x506); // timer, melee related
			//PAD8; // unused
			//TStructSubGetPtrImpl(int16,							, 0x508);
			//PAD16;
			//TStructSubGetPtrImpl(real,								, 0x50C);
			//TStructSubGetPtrImpl(real,								, 0x510);
			//TStructSubGetPtrImpl(real_plane3d,						, 0x514); // physics related
			//TStructSubGetPtrImpl(sbyte,							, 0x524);
			//TStructSubGetPtrImpl(sbyte,							, 0x525);
			TStructSubGetPtrImpl(bool,							BaselineValid, 0x526);
			TStructSubGetPtrImpl(byte,							BaselineIndex, 0x527);
			TStructSubGetPtrImpl(byte,							MessageIndex, 0x528);
			TStructSubGetPtrImpl(s_biped_datum_network_data,	UpdateBaseline, 0x52C);
			TStructSubGetPtrImpl(bool,							ShieldStunTicksGreaterThanZero, 0x538);
			TStructSubGetPtrImpl(s_biped_datum_network_data,	UpdateDelta, 0x540);
		};

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



		struct s_biped_datum : s_unit_datum
		{
			s_biped_data biped;
		}; BOOST_STATIC_ASSERT( sizeof(s_biped_datum) == Enums::k_object_size_biped );

		struct s_vehicle_datum : s_unit_datum
		{
			s_vehicle_data vehicle;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_datum) == Enums::k_object_size_vehicle );
	};
};