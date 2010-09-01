/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "Objects/ObjectDefinitions.hpp"

namespace Yelo
{
	namespace Flags
	{
		enum unit_bits
		{
			_unit_unk1_bit = BIT32(0),
			_unit_unk2_bit = BIT32(1),
			_unit_unk3_bit = BIT32(2),
			_unit_unk4_bit = BIT32(3),
			_unit_powerup_on_bit = BIT32(4),
			_unit_powerup_additional_bit = BIT32(5),
			_unit_controllable_bit = BIT32(6),
			_unit_unk8_bit = BIT32(7),
			_unit_unk9_bit = BIT32(8),
			_unit_unk10_bit = BIT32(9),
			_unit_unk11_bit = BIT32(10),
			_unit_unk12_bit = BIT32(11),
			_unit_unk13_bit = BIT32(12),
			_unit_unk14_bit = BIT32(13),
			_unit_unk15_bit = BIT32(14),
			_unit_unk16_bit = BIT32(15),
			_unit_unk17_bit = BIT32(16),
			_unit_unk18_bit = BIT32(17),
			_unit_unk19_bit = BIT32(18),
			_unit_unk20_bit = BIT32(19),
			_unit_doesnt_drop_items_bit = BIT32(20),
			_unit_unk22_bit = BIT32(21),
			_unit_can_blink_bit = BIT32(22),
			_unit_unk24_bit = BIT32(23),
			_unit_suspended_bit = BIT32(24),
			_unit_unk26_bit = BIT32(25),
			_unit_unk27_bit = BIT32(26),
			_unit_possessed_bit = BIT32(27),
			_unit_desires_flashlight_on_bit = BIT32(28),
			_unit_desires_flashlight_off_bit = BIT32(29),
			_unit_unk31_bit = BIT32(30),
			_unit_unk32_bit = BIT32(31)
		};

		enum biped_bits
		{
			_biped_airborne_bit = BIT32(0),
			_biped_hovering_bit = BIT32(1),
		};
	};

	namespace Objects
	{
		struct s_unit_data : TStructImpl(Enums::k_object_size_unit - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object, };

			TStructSubGetPtrImpl(datum_index,			ActorIndex, 0x1F4);
			TStructSubGetPtrImpl(datum_index,			SwarmActorIndex, 0x1F8);
			TStructSubGetPtrImpl(datum_index,			SwamPrevUnitIndex, 0x200);
			TStructSubGetPtrImpl(long_flags,			Flags, 0x204);
			// datum_index, 0x218, Controlling Player Index
			TStructSubGetPtrImpl(int16,					EmotionAnimationIndex, 0x21E);
			TStructSubGetPtrImpl(real_vector3d,			DesiredFacingVector, 0x224);
			TStructSubGetPtrImpl(real_vector3d,			DesiredAimingVector, 0x230);
			TStructSubGetPtrImpl(real_vector3d,			AimingVector, 0x23C);
			TStructSubGetPtrImpl(real_vector3d,			AimingVelocity, 0x248);
			TStructSubGetPtrImpl(real_euler_angles3d,	LookingAngles, 0x254);
			TStructSubGetPtrImpl(real_vector3d,			LookingVector, 0x260);
			TStructSubGetPtrImpl(real_vector3d,			LookingVelocity, 0x26C);
			TStructSubGetPtrImpl(real_rectangle2d,		AimingBounds, 0x2B8);
			TStructSubGetPtrImpl(int16,					VehicleSeatIndex, 0x2F0);
			TStructSubGetPtrImpl(int16,					CurrentWeaponIndex, 0x2F2);
			TStructSubGetPtrImpl(int16,					VehicleType, 0x2F4);
			TStructSubGetPtrImpl(datum_index,			WeaponObjectIndices, 0x2F8); // [4]
			TStructSubGetPtrImpl(int16,					CurrentGrenadeIndex, 0x31C);
			TStructSubGetPtrImpl(byte,					GrenadeCounts, 0x31E); // [2]
				TStructSubGetPtrImpl(byte,				GrenadePlasmaCount, 0x31E);
				TStructSubGetPtrImpl(byte,				GrenadeFragCount, 0x31F);
			TStructSubGetPtrImpl(datum_index,			DriverObj, 0x324);
			TStructSubGetPtrImpl(datum_index,			GunnerObj, 0x328);
			TStructSubGetPtrImpl(int32,					IntegratedPower, 0x340);
			TStructSubGetPtrImpl(real,					CamoPower, 0x37C);
			TStructSubGetPtrImpl(int16,					FeignDeathTimer, 0x420);

			// These values are also used in determining assists
			// These values would be checked in the killed unit's data.
			struct s_recent_damage
			{
				uint32 game_tick;				// the last game tick damage was dealt
				real damage;					// total (read: additive) damage the responsible object has done
				datum_index responsible_unit;
				datum_index responsible_player;	// would be NONE if killed by AI
			};
			TStructSubGetPtrImpl(s_recent_damage,		RecentDamage, 0x430); // [4]

			//bool,0x4B8
			TStructSubGetPtrImpl(int32,					LastCompletedClientUpdateId, 0x37C);

		};

		struct s_biped_data : TStructImpl(Enums::k_object_size_biped - Enums::k_object_size_unit)
		{
			enum { DATA_OFFSET = Enums::k_object_size_unit, };

			TStructSubGetPtrImpl(byte_flags,			Flags, 0x4CC);
			TStructSubGetPtrImpl(real_point3d,			HoveringPosition, 0x4FC);
			TStructSubGetPtrImpl(bool,					BaselineValid, 0x526);
			TStructSubGetPtrImpl(byte,					BaselineIndex, 0x527);
			TStructSubGetPtrImpl(byte,					MessageIndex, 0x528);

			// 0x52C baseline data structure
		};

		struct s_vehicle_data : TStructImpl(Enums::k_object_size_vehicle - Enums::k_object_size_unit)
		{
			enum { DATA_OFFSET = Enums::k_object_size_unit, };

			TStructSubGetPtrImpl(bool,					BaselineValid, 0x525);
			TStructSubGetPtrImpl(byte,					BaselineIndex, 0x526);
			TStructSubGetPtrImpl(byte,					MessageIndex, 0x527);

			// 0x52C baseline data structure
		};



		struct s_unit_datum
		{
			s_object_data object;
			s_unit_data unit;
		}; BOOST_STATIC_ASSERT( sizeof(s_unit_datum) == Enums::k_object_size_unit );

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