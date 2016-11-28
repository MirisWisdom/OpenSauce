/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#if !PLATFORM_IS_DEDI
#include "Memory/1.10/Pointers/HaloCE_110_Runtime.Scenario.inl"
#else
#include "Memory/1.10/Pointers/HaloCE_110_Dedi.Scenario.inl"
#endif

//////////////////////////////////////////////////////////////////////////
// Scenario.cpp
#if __EL_INCLUDE_FILE_ID == __EL_SCENARIO_SCENARIO
	ENGINE_DPTR(s_scenario_globals, scenario_globals,				DUO_PTR(K_SCENARIO_GLOBALS));

	ENGINE_PTR(TagGroups::scenario*, global_scenario,				DUO_PTR(K_GLOBAL_SCENARIO));
	ENGINE_PTR(TagGroups::collision_bsp*, global_bsp3d,				DUO_PTR(K_GLOBAL_BSP3D));
	ENGINE_PTR(TagGroups::collision_bsp*, global_collision_bsp,		DUO_PTR(K_GLOBAL_COLLISION_BSP));
	ENGINE_PTR(TagGroups::structure_bsp*, global_structure_bsp,		DUO_PTR(K_GLOBAL_STRUCTURE_BSP));

	ENGINE_PTR(datum_index, global_scenario_index,					DUO_PTR(K_GLOBAL_SCENARIO_INDEX));
	ENGINE_PTR(int16, structure_bsp_index,							DUO_PTR(K_STRUCTURE_BSP_INDEX));

	FUNC_PTR(OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__READ,	DUO_PTR(K_OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__READ));
	FUNC_PTR(OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__WRITE,	DUO_PTR(K_OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__WRITE));
	FUNC_PTR(OBJECTS_INITIALIZE_FOR_NEW_MAP_MOD_PROCESSED_BSPS,		DUO_PTR(K_OBJECTS_INITIALIZE_FOR_NEW_MAP_MOD_PROCESSED_BSPS));


//////////////////////////////////////////////////////////////////////////
// .cpp
//#elif __EL_INCLUDE_FILE_ID == __EL_SCENARIO_


#else
	#error Undefined engine layout include for: __EL_SCENARIO_SCENARIO
#endif