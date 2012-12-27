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

	ENGINE_DPTR(t_widget_data, widgets,											0x7FB2B8, 0x6C6938);
	ENGINE_DPTR(t_flag_data, flags,												0x7FB2C8, 0x6C6948);
	ENGINE_DPTR(t_antenna_data, antennas,										0x7FB2CC, 0x6C694C);
	ENGINE_DPTR(t_glow_data, glow,												0x7FB2C0, 0x6C6940);
	ENGINE_DPTR(t_glow_particles_data, glow_particles,							0x7FB2C4, 0x6C6944);
	ENGINE_DPTR(t_light_volumes_data, light_volumes,							0x653BD8, 0x5C5B10);
	ENGINE_DPTR(t_lightnings_data, lightnings,									0x653BDC, 0x5C5B14);

	ENGINE_DPTR(Render::cached_object_render_states_data_t, cached_object_render_states,	0x75E00C, 0x629688);
	ENGINE_DPTR(s_unit_globals_data, unit_globals,								0x68A808, 0x5F3050);
	ENGINE_DPTR(t_device_groups_data, device_groups,							0x815CD0, 0x6E1BD0);
	ENGINE_DPTR(t_object_header_data, object_header,							0x7FB2D0, 0x6C6950);
	ENGINE_DPTR(s_objects_pool_data, objects_pool,								0x653B1C, 0x5C5A54);
	ENGINE_DPTR(s_object_globals_data, object_globals,							0x653B24, 0x5C5A5C);
	ENGINE_DPTR(s_object_name_list_data, object_name_list,						0x653B20, 0x5C5A58);

	ENGINE_DPTR(collideable_object_data, collideable_object,					0x7FB2F0, 0x6C6970);
	ENGINE_DPTR(t_cluster_collideable_object_reference_data, cluster_collideable_object_reference,	0x7FB2F4, 0x6C6974);
	ENGINE_DPTR(t_collideable_object_cluster_reference_data, collideable_object_cluster_reference,	0x7FB2F8, 0x6C6978);
	ENGINE_DPTR(noncollideable_object_data, noncollideable_object,				0x7FB2E0, 0x6C6960);
	ENGINE_DPTR(t_cluster_noncollideable_object_reference_data, cluster_noncollideable_object_reference,0x7FB2E4, 0x6C6964);
	ENGINE_DPTR(t_noncollideable_object_cluster_reference_data, noncollideable_object_cluster_reference,0x7FB2E8, 0x6C6968);

	ENGINE_PTR(size_t, object_memory_pool_allocation_size,						0x4F875D, 0x4DF74D);

	FUNC_PTR(OBJECT_TYPES_PLACE_ALL_MOD_VEHI_REMAP,		0x4F7833, 0x4DE893);
	FUNC_PTR(OBJECTS_UPDATE_HOOK,						0x4F8D3E, 0x4DFAE0);
	FUNC_PTR(OBJECTS_GARBAGE_COLLECTION,				0x4FD8C0, 0x4E47B0);
	FUNC_PTR(OBJECT_DAMAGE_OBJECT_BODY_HOOK,			0x4F347A, 0x4DC6DA);
	FUNC_PTR(UNIT_CAN_ENTER_SEAT_MOD,					0x56A3B3, 0x518D43);
	FUNC_PTR(UNIT_CAUSE_PARENT_MELEE_DAMAGE_HOOK,		0x573C39, 0x522349);
	FUNC_PTR(UNIT_CAUSE_PARENT_MELEE_DAMAGE_HOOK_RETN,	0x573C41, 0x522351);
	FUNC_PTR(BIPED_UPDATE_MOVING_HOOK,					0x561B66, 0x510586);
	FUNC_PTR(BIPED_JUMP_MOD_STUN_PENALTY_FIELD_REF,		0x56283E, 0x51125E);

#if PLATFORM_IS_USER
	namespace render_objects_mods
	{
		static uintptr_t K_MAXIMUM_RENDERED_OBJECTS_REFS_32bit[] = {
			0x45B60C+1,
			0x512755+1,	0x512789+1,
		};
		static uintptr_t K_MAXIMUM_RENDERED_OBJECTS_REFS_16bit[] = {
			0x5127BF+7,
		};
		static uintptr_t K_RENDER_OBJECT_GLOBALS__RENDERED_OBJECTS_REFS[] = {
			0x512613+3,
			0x51275A+1, 0x51279E+3,
		};
	};
#endif

	namespace Weapon
	{
		FUNC_PTR(FIRST_PERSON_WEAPON_RENDER_UPDATE,							0x4953C0, FUNC_PTR_NULL);
		FUNC_PTR(RENDER_WINDOW_CALL_HOOK_FIRST_PERSON_WEAPON_RENDER_UPDATE,	0x50FD07, FUNC_PTR_NULL);
	};


//////////////////////////////////////////////////////////////////////////
// Equipment.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_OBJECTS_EQUIPMENT


//////////////////////////////////////////////////////////////////////////
// Units.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_OBJECTS_UNITS

	static uint32 K_UNIT_ZOOM_LEVEL_OFFSET_REFS[] = {
		PLATFORM_VALUE(0x459F81, 0x456AF1), PLATFORM_VALUE(0x55D7A9, 0x50C209), PLATFORM_VALUE(0x565CD9, 0x514669),
		PLATFORM_VALUE(0x5667AD, 0x51513D), PLATFORM_VALUE(0x5667BB, 0x51514B), PLATFORM_VALUE(0x566849, 0x5151D9),
		PLATFORM_VALUE(0x566D52, 0x5156E2), PLATFORM_VALUE(0x5694EA, 0x517E7A), PLATFORM_VALUE(0x569556, 0x517EE6),
		PLATFORM_VALUE(0x56964E, 0x517FDE),
	};
	static uint32 K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REFS[] = {
		PLATFORM_VALUE(0x473DE1, 0x46B8E1), PLATFORM_VALUE(0x55EDD4, 0x50D824), PLATFORM_VALUE(0x565CDF, 0x51466F),
		PLATFORM_VALUE(0x5667A7, 0x515137), PLATFORM_VALUE(0x567562, 0x515EF2), PLATFORM_VALUE(0x56955D, 0x517EED),
	};

	static uint32 K_NUMBER_OF_UNIT_GRENADE_TYPES_REFS[] = {
		PLATFORM_VALUE(0x476B4A, 0x46E5CA)+1, PLATFORM_VALUE(0x47F2FC, 0x476E7C)+1, PLATFORM_VALUE(0x4B51F9, 0x4A1649)+1, 
		PLATFORM_VALUE(0x566774, 0x515104)+1, 
#if PLATFORM_IS_USER
		PLATFORM_VALUE(0x4B665C, NULL)+1, 
#endif
		PLATFORM_VALUE(0x572CA6, 0x5213A6)+4, 
	};
	static uint32 K_MAXIMUM_UNIT_GRENADE_INDEX_REFS_32bit[] = {
		PLATFORM_VALUE(0x56D703, 0x51BE53)+1,
	};
	static uint32 K_MAXIMUM_UNIT_GRENADE_INDEX_REFS_8bit[] = {
		PLATFORM_VALUE(0x56D70F, 0x51BE5F)+3,	PLATFORM_VALUE(0x474BF4, 0x46C6F4)+2, 
	};

	FUNC_PTR(ACTOR_DIED_UNIT_GRENADE_COUNT_MOD,					0x428D35, 0x428D45);
	FUNC_PTR(PLAYER_ADD_EQUIPMENT_UNIT_GRENADE_COUNT_MOD,		0x476AC7, 0x46E547);
	FUNC_PTR(BIPED_NEW_FROM_NETWORK_UNIT_GRENADE_COUNT_MOD,		0x55EE1A, 0x50D86A);
	FUNC_PTR(BIPED_UPDATE_BASELINE_UNIT_GRENADE_COUNT_MOD1,		0x55EEB6, 0x50D906);
	FUNC_PTR(BIPED_UPDATE_BASELINE_UNIT_GRENADE_COUNT_MOD2,		0x55EED4, 0x50D924);
	FUNC_PTR(BIPED_BUILD_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD1,	0x55EFDF, 0x50DA2F);
	FUNC_PTR(BIPED_BUILD_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD2,	0x55EFF5, 0x50DA45);
	FUNC_PTR(BIPED_PROCESS_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD,	0x55F1A6, 0x50DBF6);


#else
	#error Undefined engine layout include for: __EL_INCLUDE_OBJECTS
#endif