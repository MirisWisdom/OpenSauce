/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Objects.cpp
#if __EL_INCLUDE_FILE_ID == __EL_OBJECTS_OBJECTS
	ENGINE_DPTR(t_widget_data, widgets,											0x7FB2B8, 0x6C6938);
	ENGINE_DPTR(t_flag_data, flags,												0x7FB2C8, 0x6C6948);
	ENGINE_DPTR(t_antenna_data, antennas,										0x7FB2CC, 0x6C694C);
	ENGINE_DPTR(t_glow_data, glow,												0x7FB2C0, 0x6C6940);
	ENGINE_DPTR(t_glow_particles_data, glow_particles,							0x7FB2C4, 0x6C6944);
	ENGINE_DPTR(t_light_volumes_data, light_volumes,							0x653BD8, 0x5C5B10);
	ENGINE_DPTR(t_lightnings_data, lightnings,									0x653BDC, 0x5C5B14);

	ENGINE_DPTR(t_cached_object_render_states_data, cached_object_render_states,0x75E00C, 0x629688);
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

	FUNC_PTR(OBJECTS_UPDATE_HOOK,						0x4F8D3E, 0x4DFAE0);
	FUNC_PTR(OBJECTS_GARBAGE_COLLECTION,				0x4FD8C0, 0x4E47B0);
	FUNC_PTR(OBJECT_DAMAGE_OBJECT_BODY_HOOK,			0x4F347A, 0x4DC6DA);
	FUNC_PTR(UNIT_CAN_ENTER_SEAT_MOD,					0x56A3B1, 0x518D41);
	FUNC_PTR(UNIT_CAUSE_PARENT_MELEE_DAMAGE_HOOK,		0x573C39, 0x522349);
	FUNC_PTR(UNIT_CAUSE_PARENT_MELEE_DAMAGE_HOOK_RETN,	0x573C41, 0x522351);
	FUNC_PTR(BIPED_UPDATE_MOVING_HOOK,					0x561B66, 0x510586);

	namespace Weapon
	{
		FUNC_PTR(FIRST_PERSON_WEAPON_RENDER_UPDATE,							0x4953C0, FUNC_PTR_NULL);
		FUNC_PTR(RENDER_WINDOW_CALL_HOOK_FIRST_PERSON_WEAPON_RENDER_UPDATE,	0x50FD07, FUNC_PTR_NULL);
	};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_OBJECTS
#endif