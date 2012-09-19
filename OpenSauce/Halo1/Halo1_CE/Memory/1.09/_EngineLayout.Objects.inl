/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Objects.cpp
#if __EL_INCLUDE_FILE_ID == __EL_OBJECTS_OBJECTS
	ENGINE_PTR(s_object_type_definition**, object_type_definitions,				0x6376A4, 0x5AD07C);
	ENGINE_PTR(s_widget_type_definition*,  widget_type_definitions,				0x6376D8, 0x5AD0B0);

	ENGINE_DPTR(t_widget_data, widgets,											0x7FB398, 0x6C69D8);
	ENGINE_DPTR(t_flag_data, flags,												0x7FB3A8, 0x6C69E8);
	ENGINE_DPTR(t_antenna_data, antennas,										0x7FB3AC, 0x6C69EC);
	ENGINE_DPTR(t_glow_data, glow,												0x7FB3A0, 0x6C69E0);
	ENGINE_DPTR(t_glow_particles_data, glow_particles,							0x7FB3A4, 0x6C69E4);
	ENGINE_DPTR(t_light_volumes_data, light_volumes,							0x653CC0, 0x5C5BA8);
	ENGINE_DPTR(t_lightnings_data, lightnings,									0x653CC4, 0x5C5BAC);

	ENGINE_DPTR(t_cached_object_render_states_data, cached_object_render_states,0x75E0EC, 0x629728);
	ENGINE_DPTR(s_unit_globals_data, unit_globals,								0x68A8F0, 0x5F30E8);
	ENGINE_DPTR(t_device_groups_data, device_groups,							0x815DB0, 0x6E1C70);
	ENGINE_DPTR(t_object_header_data, object_header,							0x7FB3B0, 0x6C69F0);
	ENGINE_DPTR(s_objects_pool_data, objects_pool,								0x653C04, 0x5C5AEC);
	ENGINE_DPTR(s_object_globals_data, object_globals,							0x653C0C, 0x5C5AF4);
	ENGINE_DPTR(s_object_name_list_data, object_name_list,						0x653C08, 0x5C5AF0);

	ENGINE_DPTR(collideable_object_data, collideable_object,					0x7FB3D0, 0x6C6A10);
	ENGINE_DPTR(t_cluster_collideable_object_reference_data, cluster_collideable_object_reference,	0x7FB3D4, 0x6C6A14);
	ENGINE_DPTR(t_collideable_object_cluster_reference_data, collideable_object_cluster_reference,	0x7FB3D8, 0x6C6A18);
	ENGINE_DPTR(noncollideable_object_data, noncollideable_object,				0x7FB3C0, 0x6C6A00);
	ENGINE_DPTR(t_cluster_noncollideable_object_reference_data, cluster_noncollideable_object_reference,0x7FB3C4, 0x6C6A04);
	ENGINE_DPTR(t_noncollideable_object_cluster_reference_data, noncollideable_object_cluster_reference,0x7FB3C8, 0x6C6A08);

	FUNC_PTR(OBJECTS_UPDATE_HOOK,						0x4F8D6E, 0x4DFB10);
	FUNC_PTR(OBJECTS_GARBAGE_COLLECTION,				0x4FD8F0, 0x4E47E0);
	FUNC_PTR(OBJECT_DAMAGE_OBJECT_BODY_HOOK,			0x4F34AA, 0x4DC70A);
	FUNC_PTR(UNIT_CAN_ENTER_SEAT_MOD,					0x56A3E3, 0x518D73);
	FUNC_PTR(UNIT_CAUSE_PARENT_MELEE_DAMAGE_HOOK,		0x573C69, 0x522379);
	FUNC_PTR(UNIT_CAUSE_PARENT_MELEE_DAMAGE_HOOK_RETN,	0x573C71, 0x522381);
	FUNC_PTR(BIPED_UPDATE_MOVING_HOOK,					0x561B96, 0x5105B6);

	namespace Weapon
	{
		FUNC_PTR(FIRST_PERSON_WEAPON_RENDER_UPDATE,							0x4953D0, FUNC_PTR_NULL);
		FUNC_PTR(RENDER_WINDOW_CALL_HOOK_FIRST_PERSON_WEAPON_RENDER_UPDATE,	0x50FD37, FUNC_PTR_NULL);
	};


//////////////////////////////////////////////////////////////////////////
// Equipment.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_OBJECTS_EQUIPMENT


//////////////////////////////////////////////////////////////////////////
// Units.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_OBJECTS_UNITS

	static uint32 K_UNIT_ZOOM_LEVEL_OFFSET_REFS[] = {
		PLATFORM_VALUE(0x459F81, 0x456AF1)+0x10, PLATFORM_VALUE(0x55D7A9, 0x50C209)+0x30, PLATFORM_VALUE(0x565CD9, 0x514669)+0x30,
		PLATFORM_VALUE(0x5667AD, 0x51513D)+0x30, PLATFORM_VALUE(0x5667BB, 0x51514B)+0x30, PLATFORM_VALUE(0x566849, 0x5151D9)+0x30,
		PLATFORM_VALUE(0x566D52, 0x5156E2)+0x30, PLATFORM_VALUE(0x5694EA, 0x517E7A)+0x30, PLATFORM_VALUE(0x569556, 0x517EE6)+0x30,
		PLATFORM_VALUE(0x56964E, 0x517FDE)+0x30,
	};
	static uint32 K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REFS[] = {
		PLATFORM_VALUE(0x473DE1, 0x46B8E1)+0x10, PLATFORM_VALUE(0x55EDD4, 0x50D824)+0x30, PLATFORM_VALUE(0x565CDF, 0x51466F)+0x30,
		PLATFORM_VALUE(0x5667A7, 0x515137)+0x30, PLATFORM_VALUE(0x567562, 0x515EF2)+0x30, PLATFORM_VALUE(0x56955D, 0x517EED)+0x30,
	};

	static uint32 K_NUMBER_OF_UNIT_GRENADE_TYPES_REFS[] = {
		PLATFORM_VALUE(0x476B4A, 0x46E5CA)+1+0x10, PLATFORM_VALUE(0x47F2FC, 0x476E7C)+1+0x10, PLATFORM_VALUE(0x4B51F9, 0x4A1649)+1+0x30, 
		PLATFORM_VALUE(0x566774, 0x515104)+1+0x30, 
#if PLATFORM_IS_USER
		PLATFORM_VALUE(0x4B665C, NULL)+1+0x30, 
#endif
		PLATFORM_VALUE(0x572CA6, 0x5213A6)+4+0x30, 
	};

	FUNC_PTR(ACTOR_DIED_UNIT_GRENADE_COUNT_MOD,					0x428D38+0x10, 0x428D48+0x10);
	FUNC_PTR(PLAYER_ADD_EQUIPMENT_UNIT_GRENADE_COUNT_MOD,		0x476ACA+0x10, 0x46E54A+0x10);
	FUNC_PTR(BIPED_NEW_FROM_NETWORK_UNIT_GRENADE_COUNT_MOD,		0x55EE21+0x30, 0x50D871+0x30);
	FUNC_PTR(BIPED_UPDATE_BASELINE_UNIT_GRENADE_COUNT_MOD1,		0x55EEB9+0x30, 0x50D909+0x30);
	FUNC_PTR(BIPED_UPDATE_BASELINE_UNIT_GRENADE_COUNT_MOD2,		0x55EED4+0x30, 0x50D924+0x30);
	FUNC_PTR(BIPED_BUILD_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD1,	0x55EFE2+0x30, 0x50DA32+0x30);
	FUNC_PTR(BIPED_BUILD_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD2,	0x55EFF5+0x30, 0x50DA45+0x30);
	FUNC_PTR(UNIT_PLACE_UNIT_GRENADE_COUNT_MOD,					0x55F1A9+0x30, 0x50DBF9+0x30);

#else
	#error Undefined engine layout include for: __EL_INCLUDE_OBJECTS
#endif