/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Scenario.cpp
#if __EL_INCLUDE_FILE_ID == __EL_SCENARIO_SCENARIO
	ENGINE_DPTR(s_scenario_globals, scenario_globals,				0x6E2194, 0x6293B4);

	ENGINE_PTR(TagGroups::scenario*, global_scenario,				0x6E218C, 0x6293AC);
	ENGINE_PTR(TagGroups::collision_bsp*, global_bsp3d,				0x6E2190, 0x6293B0);
	ENGINE_PTR(TagGroups::collision_bsp*, global_collision_bsp,		0x6E2198, 0x6293B8);
	ENGINE_PTR(TagGroups::structure_bsp*, global_structure_bsp,		0x6E219C, 0x6293BC);

	ENGINE_PTR(datum_index, global_scenario_index,					0x6397DC, 0x5AD85C);
	ENGINE_PTR(int16, structure_bsp_index,							0x6397E0, 0x5AD860);

#if !PLATFORM_IS_DEDI
	ENGINE_PTR(int16, render_sky_flag,								0x75E26D, DATA_PTR_UNKNOWN);
	ENGINE_PTR(int16, sky_index,									0x75E26E, DATA_PTR_UNKNOWN);
#endif

	FUNC_PTR(OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__READ,	0x4F8557, 0x4DF547);
	FUNC_PTR(OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__WRITE,	0x4F8557, 0x4DF70E);
	FUNC_PTR(OBJECTS_INITIALIZE_FOR_NEW_MAP_MOD_PROCESSED_BSPS,		0x4F8832, 0x4DF822);


//////////////////////////////////////////////////////////////////////////
// .cpp
//#elif __EL_INCLUDE_FILE_ID == __EL_SCENARIO_
	
#elif __EL_INCLUDE_FILE_ID == __EL_SCENARIO_SCENARIOINFO
	FUNC_PTR(INITIALIZE_RENDER_SKY_CALL, 0x556F94 - 0x30, FUNC_PTR_UNKNOWN);
#else
	#error Undefined engine layout include for: __EL_SCENARIO_SCENARIO
#endif