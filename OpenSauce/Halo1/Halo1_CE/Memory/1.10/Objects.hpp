/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Memory/1.10/Pointers/HaloCE_110_Runtime.Objects.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Runtime_Manual.Objects.inl"
#else
#include "Memory/1.10/Pointers/HaloCE_110_Dedi.Objects.inl"
#endif

#include <Objects/Objects.hpp>
#include <blamlib/objects/widgets/widgets.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelolib/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace Objects
	{
		ENGINE_PTR(s_object_type_definition**, object_type_definitions, DUO_PTR(K_OBJECT_TYPE_DEFINITIONS));
		ENGINE_PTR(s_widget_type_definition*, widget_type_definitions, DUO_PTR(K_WIDGET_TYPE_DEFINITIONS));

		ENGINE_DPTR(widget_data_t, widgets, DUO_PTR(K_WIDGETS));
		ENGINE_DPTR(flag_data_t, flags, DUO_PTR(K_FLAGS));
		ENGINE_DPTR(antenna_data_t, antennas, DUO_PTR(K_ANTENNAS));
		ENGINE_DPTR(glow_data_t, glow, DUO_PTR(K_GLOW));
		ENGINE_DPTR(glow_particles_data_t, glow_particles, DUO_PTR(K_GLOW_PARTICLES));
		ENGINE_DPTR(light_volumes_data_t, light_volumes, DUO_PTR(K_LIGHT_VOLUMES));
		ENGINE_DPTR(lightnings_data_t, lightnings, DUO_PTR(K_LIGHTNINGS));

		ENGINE_DPTR(Render::cached_object_render_states_data_t, cached_object_render_states, DUO_PTR(K_CACHED_OBJECT_RENDER_STATES));
		ENGINE_DPTR(s_unit_globals_data, unit_globals, DUO_PTR(K_UNIT_GLOBALS));
		ENGINE_DPTR(device_groups_data_t, device_groups, DUO_PTR(K_DEVICE_GROUPS));
		ENGINE_DPTR(object_header_data_t, object_header, DUO_PTR(K_OBJECT_HEADER));
		ENGINE_DPTR(s_objects_pool_data, objects_pool, DUO_PTR(K_OBJECTS_POOL));
		ENGINE_DPTR(s_object_globals_data, object_globals, DUO_PTR(K_OBJECT_GLOBALS));
		ENGINE_DPTR(s_object_name_list_data, object_name_list, DUO_PTR(K_OBJECT_NAME_LIST));

		ENGINE_DPTR(collideable_object_data, collideable_object, DUO_PTR(K_COLLIDEABLE_OBJECT));
		ENGINE_DPTR(cluster_collideable_object_reference_data_t, cluster_collideable_object_reference, DUO_PTR(K_CLUSTER_COLLIDEABLE_OBJECT_REFERENCE));
		ENGINE_DPTR(collideable_object_cluster_reference_data_t, collideable_object_cluster_reference, DUO_PTR(K_COLLIDEABLE_OBJECT_CLUSTER_REFERENCE));
		ENGINE_DPTR(noncollideable_object_data, noncollideable_object, DUO_PTR(K_NONCOLLIDEABLE_OBJECT));
		ENGINE_DPTR(cluster_noncollideable_object_reference_data_t, cluster_noncollideable_object_reference, DUO_PTR(K_CLUSTER_NONCOLLIDEABLE_OBJECT_REFERENCE));
		ENGINE_DPTR(noncollideable_object_cluster_reference_data_t, noncollideable_object_cluster_reference, DUO_PTR(K_NONCOLLIDEABLE_OBJECT_CLUSTER_REFERENCE));

		ENGINE_PTR(size_t, object_memory_pool_allocation_size, DUO_PTR(K_OBJECT_MEMORY_POOL_ALLOCATION_SIZE));

		FUNC_PTR(OBJECT_TYPES_PLACE_ALL_MOD_VEHI_REMAP, DUO_PTR(K_OBJECT_TYPES_PLACE_ALL_MOD_VEHI_REMAP));
		FUNC_PTR(OBJECTS_UPDATE__OBJECT_IN_PLAYER_PVS_NOP1, K_OBJECTS_UPDATE__OBJECT_IN_PLAYER_PVS_NOP1, PTR_NULL);
		FUNC_PTR(OBJECTS_UPDATE__OBJECT_IN_PLAYER_PVS_NOP2, K_OBJECTS_UPDATE__OBJECT_IN_PLAYER_PVS_NOP2, PTR_NULL);
		FUNC_PTR(OBJECTS_UPDATE_CALL, DUO_PTR(K_OBJECTS_UPDATE_CALL));
		FUNC_PTR(OBJECTS_UPDATE_HOOK, DUO_PTR(K_OBJECTS_UPDATE_HOOK));
		FUNC_PTR(OBJECTS_GARBAGE_COLLECTION, DUO_PTR(K_OBJECTS_GARBAGE_COLLECTION));
		FUNC_PTR(OBJECT_DAMAGE_OBJECT_BODY_HOOK, DUO_PTR(K_OBJECT_DAMAGE_OBJECT_BODY_HOOK));
		FUNC_PTR(BIPED_UPDATE_MOVING_HOOK, DUO_PTR(K_BIPED_UPDATE_MOVING_HOOK));
		FUNC_PTR(BIPED_JUMP_MOD_STUN_PENALTY_FIELD_REF, DUO_PTR(K_BIPED_JUMP_MOD_STUN_PENALTY_FIELD_REF));
		FUNC_PTR(OBJECT_DEPLETE_BODY_KILL_CHILD_HOOK, DUO_PTR(K_OBJECT_DEPLETE_BODY_KILL_CHILD_HOOK));
		FUNC_PTR(OBJECT_DEPLETE_BODY_KILL_CHILD_RETN, DUO_PTR(K_OBJECT_DEPLETE_BODY_KILL_CHILD_RETN));

		static const uintptr_t OBJECT_DAMAGE_AFTERMATH__CALC_ACCEL_HOOK = K_OBJECT_DAMAGE_AFTERMATH__CALC_ACCEL_HOOK;
		static const uintptr_t OBJECT_DAMAGE_AFTERMATH__CALC_ACCEL_HOOK_RETURN = K_OBJECT_DAMAGE_AFTERMATH__CALC_ACCEL_HOOK_RETURN;

		static const uintptr_t OBJECT_DAMAGE_AFTERMATH__UNIT_VALIDATE_INST_ACCEL_HOOK = K_OBJECT_DAMAGE_AFTERMATH__UNIT_VALIDATE_INST_ACCEL_HOOK;
		static const uintptr_t OBJECT_DAMAGE_AFTERMATH__UNIT_VALIDATE_INST_ACCEL_HOOK_RETURN = K_OBJECT_DAMAGE_AFTERMATH__UNIT_VALIDATE_INST_ACCEL_HOOK_RETURN;
		static const uintptr_t OBJECT_DAMAGE_AFTERMATH__UNIT_VALIDATE_INST_ACCEL_HOOK_RETURN_INVALID = K_OBJECT_DAMAGE_AFTERMATH__UNIT_VALIDATE_INST_ACCEL_HOOK_RETURN_INVALID;

		#if PLATFORM_IS_USER
		namespace render_objects_mods
		{
			static uintptr_t K_MAXIMUM_RENDERED_OBJECTS_REFS_32bit[] = {
				K_MAXIMUM_RENDERED_OBJECTS_REF_32BIT_0,
				K_MAXIMUM_RENDERED_OBJECTS_REF_32BIT_1,
			};
			static uintptr_t K_MAXIMUM_RENDERED_OBJECTS_REFS_16bit[] = {
				K_MAXIMUM_RENDERED_OBJECTS_REF_16BIT_0,
			};
			static uintptr_t K_RENDER_OBJECT_GLOBALS__RENDERED_OBJECTS_REFS[] = {
				K_RENDER_OBJECT_GLOBALS__RENDERED_OBJECTS_REF_0,
				K_RENDER_OBJECT_GLOBALS__RENDERED_OBJECTS_REF_1,
				K_RENDER_OBJECT_GLOBALS__RENDERED_OBJECTS_REF_2,
				K_RENDER_OBJECT_GLOBALS__RENDERED_OBJECTS_REF_3,
			};

			FUNC_PTR(OBJECT_NEW_SET_SHADOW_FLAG_HOOK, K_OBJECT_NEW_SET_SHADOW_FLAG_HOOK, PTR_NULL);
			FUNC_PTR(OBJECT_NEW_SET_SHADOW_FLAG_RETN, K_OBJECT_NEW_SET_SHADOW_FLAG_RETN, PTR_NULL);
			static void* K_OBJECT_DISABLE_SHADOW_CAST_OVERRIDES[] = {
				CAST_PTR(void*,K_OBJECT_DISABLE_SHADOW_CAST_OVERRIDE_0),
				CAST_PTR(void*,K_OBJECT_DISABLE_SHADOW_CAST_OVERRIDE_1),
				CAST_PTR(void*,K_OBJECT_DISABLE_SHADOW_CAST_OVERRIDE_2),
			};
		}
		#endif

		namespace Weapon
		{
			FUNC_PTR(FIRST_PERSON_WEAPON_RENDER_UPDATE, K_FIRST_PERSON_WEAPON_RENDER_UPDATE, FUNC_PTR_NULL);
			FUNC_PTR(RENDER_WINDOW_CALL_HOOK_FIRST_PERSON_WEAPON_RENDER_UPDATE, K_RENDER_WINDOW_CALL_HOOK_FIRST_PERSON_WEAPON_RENDER_UPDATE, FUNC_PTR_NULL);
		}
	}
}
