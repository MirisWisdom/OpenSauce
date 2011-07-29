/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum object_sizes
		{
			k_object_size_object =			300,
			k_object_size_projectile =		128 + k_object_size_object,
			k_object_size_scenery =			16  + k_object_size_object,
			k_object_size_sound_scenery =	28  + k_object_size_object,
			k_object_size_crate =			4   + k_object_size_object,
			k_object_size_creature =		220 + k_object_size_object,

			k_object_size_device =			160 + k_object_size_object,
			k_object_size_machine =			20  + k_object_size_device,
			k_object_size_control =			8   + k_object_size_device,
			k_object_size_light_fixture =	24  + k_object_size_device,

			k_object_size_item =			64   + k_object_size_object, // only 4 bytes larger than Halo 1
			k_object_size_weapon =			240  + k_object_size_item,
			k_object_size_equipment =		24   + k_object_size_item,
			k_object_size_garbage =			4    + k_object_size_item,

			k_object_size_unit =			540  + k_object_size_object,
			k_object_size_biped =			284  + k_object_size_unit,
			k_object_size_vehicle =			224  + k_object_size_unit,
		};
	};

	namespace GameState
	{
		struct s_object_data : TStructImpl( Enums::k_object_size_object )
		{
			TStructGetPtrImpl(datum_index, Definition, 0x0);
			TStructGetPtrImpl(datum_index, NextIndex, 0xC);
			TStructGetPtrImpl(datum_index, ParentIndex, 0x14);
			TStructGetPtrImpl(int16, PlacementIndex, 0x1A);

			TStructGetPtrImpl(real_point2d, Location, 0x28);
			TStructGetPtrImpl(real_point3d, Center, 0x30);
			TStructGetPtrImpl(real, Radius, 0x3C);

#if PLATFORM_ID == PLATFORM_H2_ALPHA
			TStructGetPtrImpl(byte_enum, Type, 0x8A);
			TStructGetPtrImpl(int16, NameListIndex, 0x8C);
			TStructGetPtrImpl(byte, MultiplayerSpawnMonitorIndex, 0x8F);
			TStructGetPtrImpl(datum_index, HavokComponent, 0xA0);
			TStructGetPtrImpl(datum_index, SimulationEntityIndex, 0xA8);
#else
			TStructGetPtrImpl(real_vector3d, Origin, 0x64); // really a real_point3d
			TStructGetPtrImpl(real_vector3d, Forward, 0x70);
			TStructGetPtrImpl(real_euler_angles3d, Up, 0x7C);
			TStructGetPtrImpl(real_vector3d, TransitionalVelocity, 0x88);
			TStructGetPtrImpl(real_vector3d, AngularVelocity, 0x94);
			TStructGetPtrImpl(real, Scale, 0xA0);

			//TStructGetPtrImpl(struct, , 0xA4); // {int32, int16, byte type, byte}
			TStructGetPtrImpl(byte_enum, Type, 0xAA);
			TStructGetPtrImpl(int16, NameListIndex, 0xAC);
			TStructGetPtrImpl(byte, NetgameEquipmentIndex, 0xAF); // index in the game engine global's equipment indices
			TStructGetPtrImpl(datum_index, HavokComponent, 0xB4);
			TStructGetPtrImpl(real, MaximumVitality, 0xE4); // just a guess
			TStructGetPtrImpl(real, CurrentVitality, 0xE8); // just a guess
			TStructGetPtrImpl(real, Health, 0xEC);
			TStructGetPtrImpl(real, Shields, 0xF0);

			TStructGetPtrImpl(byte_enum, CollisionFlags, 0x10A); // these are just bs names btw
			TStructGetPtrImpl(byte_enum, HealthFlags, 0x10B);
#endif
		};

		struct s_scenery_data : TStructImpl(Enums::k_object_size_scenery - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object };
			//TStructSubGetPtrImpl(int32, , 0x134); // index to lightmap_group->block128->block88 (the data in that block and the data at 0xA4 in THIS equal)
		};

		struct s_sound_scenery_data : TStructImpl(Enums::k_object_size_sound_scenery - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object };
		};

		struct s_projectile_data : TStructImpl(Enums::k_object_size_projectile - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object };
		};

		struct s_crate_data : TStructImpl(Enums::k_object_size_crate - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object };
		};

		struct s_creature_data : TStructImpl(Enums::k_object_size_creature - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object };
		};



		struct s_scenery_datum
		{
			s_object_data object;
			s_scenery_data scenery;
		};

		struct s_sound_scenery_datum
		{
			s_object_data object;
			s_sound_scenery_data sound_scenery;
		};

		struct s_projectile_datum
		{
			s_object_data object;
			s_projectile_data projectile;
		};

		struct s_crate_datum
		{
			s_object_data object;
			s_crate_data crate;
		};

		struct s_creature_datum
		{
			s_object_data object;
			s_creature_data creature;
		};
	};

	namespace TagGroups
	{
	};
};