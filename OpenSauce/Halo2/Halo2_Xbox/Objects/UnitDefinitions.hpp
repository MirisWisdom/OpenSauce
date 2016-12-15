/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once
#include "Objects/ObjectDefinitions.hpp"

namespace Yelo
{
	namespace GameState
	{
		struct s_unit_data : TStructImpl(Enums::k_object_size_unit - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object };

#if PLATFORM_ID == PLATFORM_H2_ALPHA
			TStructSubGetPtrImpl(datum_index, DriverUnit, 0x208);
			TStructSubGetPtrImpl(datum_index, GunnerUnit, 0x20C);
#else
			// 1 = controllable
			// 2 = actively controlled
			TStructSubGetPtrImpl(byte_flags, ControlFlags, 0x134);
			TStructSubGetPtrImpl(datum_index, PlayerIndex, PLATFORM_VALUE_(0x13C, 0x10C));

			TStructSubGetPtrImpl(int16, VehicleSeatIndex, 0x1FC);

			TStructSubGetPtrImpl(byte, CurrentWeaponIndex, 0x212);
			TStructSubGetPtrImpl(byte, CurrentWeaponIndex2, 0x213); // dual'd weapon?
			//TStructSubGetPtrImpl(int32, , 0x214);
			TStructSubGetPtrImpl(datum_index, WeaponsList, 0x218);
			TStructSubGetPtrImpl(byte, CurrentGrenadesIndex, 0x23D);
			TStructSubGetPtrImpl(byte, GrenadesList, 0x23E);

			TStructSubGetPtrImpl(datum_index, PredictedPlayerIndex, PLATFORM_VALUE_(0x2A8, 0x268));

			TStructSubGetPtrImpl(real, CamoPower, 0x2B0);
#endif
		};

		struct s_biped_data : TStructImpl(Enums::k_object_size_biped - Enums::k_object_size_unit)
		{
			enum { DATA_OFFSET = Enums::k_object_size_unit };
		};

		struct s_vehicle_data : TStructImpl(Enums::k_object_size_vehicle - Enums::k_object_size_unit)
		{
			enum { DATA_OFFSET = Enums::k_object_size_unit };
		};



		struct s_unit_datum
		{
			s_object_data object;
			s_unit_data unit;
		};

		struct s_biped_datum : s_unit_datum
		{
			s_biped_data biped;
		};

		struct s_vehicle_datum : s_unit_datum
		{
			s_vehicle_data vehicle;
		};
	};

	namespace TagGroups
	{
	};
};