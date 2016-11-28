/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <blamlib/Halo2/memory/data.hpp>

#include "Physics/Havok.hpp"

namespace Yelo
{
	namespace GameState
	{
		Havok::hkWorld* _HavokWorld();

		struct s_physics_constants
		{
			static const real k_gravity;

			real Gravity; // scale inputs using 4.1712594f
			real WaterDensity;
			real AirDensity;
			bool CharacterGroundAdhesionForcesEnableTime; // k_physics_character_ground_adhesion_force_maximum_disable_time = 5.0
			PAD24;
			real_vector3d VelocityFrame;

			inline void ScaleGravity(real scale){ Gravity = k_gravity * scale; }
			inline void ResetGravity()			{ Gravity = k_gravity; }
		};
		s_physics_constants* _PhysicsConstants();

		struct s_kill_volumes : TStructImpl(36)
		{
		};
		s_kill_volumes* _KillVolumes();

		struct s_breakable_surface_globals : TStructImpl(16900)
		{
		};
		s_breakable_surface_globals* _BreakableSurfaceGlobals();

		struct s_havok_globals
		{
			int32 ShapeCount;
		};
		s_havok_globals* _HavokGlobals();

		struct s_havok_shape_globals : TStructImpl(8000)
		{
			int32 ObjectIndexesInMopp[2000]; // scenery
		};
		s_havok_shape_globals* _HavokShapeGlobals();

		struct s_impacts_datum : TStructImpl(160)
		{
#if PLATFORM_ID == PLATFORM_H2_ALPHA
			TStructGetPtrImpl(int16, ReferenceCount, 8);
			// 0xA byte
			TStructGetPtrImpl(datum_index, HavokComponentIndexA, 0x24);
			TStructGetPtrImpl(datum_index, HavokComponentIndexB, 0x28);
			// 0x30 int32
			// 0x34 int32
			// 0x38 int32
			// 0x3C int32
#else
#endif
		};
		typedef Memory::DataArray<s_impacts_datum, 32> t_impacts_data;
		t_impacts_data* _Impacts();

		struct s_impact_arrays_datum : TStructImpl(64)
		{
		};
		typedef Memory::DataArray<s_impact_arrays_datum, 32> t_impact_arrays_data;
		t_impact_arrays_data* _ImpactArrays();

		struct s_havok_components_datum : TStructImpl(160)
		{
#if PLATFORM_ID == PLATFORM_H2_ALPHA
			// bit 0 = _records_contact_points_bit
			// bit 5 = _connected_to_world_bit
			// bit 6 = has_phantom_transform
			// bit 22 = has_friction
			TStructGetPtrImpl(byte_flags, Flags, 2);
			TStructGetPtrImpl(byte, MaximumRigidBodySize, 4);
			TStructGetPtrImpl(byte, ImpactCount, 5);
			TStructGetPtrImpl(datum_index, ObjectIndex, 0x48);
			//TStructGetPtrImpl(Havok::hkArray<?>*, RigidBodies, 0xA0);
			//TStructGetPtrImpl(Havok::hkArray<?>*, Constraints, 0xA4);
			//TStructGetPtrImpl(?, ContactPoints, 0xA8);
			//TStructGetPtrImpl(Havok::hkArray<?>*, BodiesInPhantom, 0xA4);
#else
			// bit 0 = _records_contact_points_bit
			// bit 5 = _connected_to_world_bit
			// bit 6 = has_phantom_transform
			// bit 22 = has_friction
			TStructGetPtrImpl(byte_flags, Flags, 4);
			TStructGetPtrImpl(datum_index, ObjectIndex, 8);
			//TStructGetPtrImpl(datum_index, , 0x48);
			//TStructGetPtrImpl(Havok::?, Deactivator, 0x54); // custom hkSpatialRigidBodyDeactivator
			TStructGetPtrImpl(Havok::hkArray<byte>, Shape0, 0x70); // 0x80 byte shape. Havok::hkArray<shape> @ 0x48, 0x bytes
			TStructGetPtrImpl(Havok::hkArray<byte>, Shape1, 0x7C); // 0x10 byte shape
			TStructGetPtrImpl(Havok::hkArray<byte>, Shape2, 0x88); // 0x50 byte shape
			TStructGetPtrImpl(Havok::hkArray<byte>*, Shape3, 0x94); // 0x8 byte child shape
#endif
		};
		typedef Memory::DataArray<s_havok_components_datum, 512> t_havok_components_data;
		t_havok_components_data* _HavokComponents();
	};
};