/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Objects.cpp
#if __EL_INCLUDE_FILE_ID == __EL_OBJECTS_OBJECTS
namespace GameState
{
	ENGINE_DPTR(t_device_groups_data, _DeviceGroups,							NULL);
	ENGINE_DPTR(t_object_data, _Object,											NULL);
	ENGINE_DPTR(Memory::s_memory_pool, _Objects,								NULL);
	ENGINE_DPTR(s_object_name_list, _ObjectNameList,							NULL);
	ENGINE_DPTR(s_object_globals, _ObjectGlobals,								NULL);
	ENGINE_DPTR(s_object_clusters, _ObjectClusters,								NULL);
	ENGINE_DPTR(s_object_scripting_globals, _ObjectScriptingGlobals,			NULL);
	ENGINE_DPTR(t_cached_object_render_states_data, _CachedObjectRenderStates,	NULL);
	ENGINE_DPTR(s_object_placement_data, _ObjectPlacementData,					NULL);
};


//////////////////////////////////////////////////////////////////////////
// Widgets.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_OBJECTS_WIDGETS
namespace GameState
{
	ENGINE_DPTR(t_widgets_data, _Widgets,			NULL);
	ENGINE_DPTR(t_antenna_data, _Antenna,			NULL);
	ENGINE_DPTR(t_cloth_data, _Cloth,				NULL);
	ENGINE_DPTR(t_liquid_data, _Liquid,				NULL);
	ENGINE_DPTR(t_lights_data, _Lights,				NULL);
	ENGINE_DPTR(s_light_globals, _LightGlobals,		NULL);
	ENGINE_DPTR(s_light_clusters, _LightClusters,	NULL);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_OBJECTS
#endif