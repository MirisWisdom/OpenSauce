/*
    Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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