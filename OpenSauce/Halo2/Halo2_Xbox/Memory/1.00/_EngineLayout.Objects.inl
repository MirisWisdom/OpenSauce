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
	ENGINE_DPTR(t_device_groups_data, _DeviceGroups,							0x4E0328);
	ENGINE_DPTR(t_object_data, _Object,											0x4E0300);
	ENGINE_DPTR(Memory::s_memory_pool, _Objects,								0x4DE2EC);
	ENGINE_DPTR(s_object_name_list, _ObjectNameList,							0x4DE2D0);
	ENGINE_DPTR(s_object_globals, _ObjectGlobals,								0x4DE2F4);
	ENGINE_DPTR(s_object_clusters, _ObjectClusters,								0x4DE2E0);
	ENGINE_DPTR(s_object_scripting_globals, _ObjectScriptingGlobals,			0x5107F4);
	ENGINE_DPTR(t_cached_object_render_states_data, _CachedObjectRenderStates,	0x509434);
	ENGINE_DPTR(s_object_placement_data, _ObjectPlacementData,					0x4E0324);
};


//////////////////////////////////////////////////////////////////////////
// Widgets.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_OBJECTS_WIDGETS
namespace GameState
{
	ENGINE_DPTR(t_widgets_data, _Widgets,			0x4E0320);
	ENGINE_DPTR(t_antenna_data, _Antenna,			0x4E0334);
	ENGINE_DPTR(t_cloth_data, _Cloth,				0x4E0338);
	ENGINE_DPTR(t_liquid_data, _Liquid,				0x4E031C);
	ENGINE_DPTR(t_lights_data, _Lights,				0x4E030C);
	ENGINE_DPTR(s_light_globals, _LightGlobals,		0x5107E8);
	ENGINE_DPTR(s_light_clusters, _LightClusters,	0x4E0310);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_OBJECTS
#endif