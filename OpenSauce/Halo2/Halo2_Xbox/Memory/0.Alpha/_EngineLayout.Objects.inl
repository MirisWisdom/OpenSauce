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
	ENGINE_DPTR(t_device_groups_data, _DeviceGroups,							0x650ED4);
	ENGINE_DPTR(t_object_data, _Object,											0x650EA0);
	ENGINE_DPTR(Memory::s_memory_pool, _Objects,								0x650E88);
	ENGINE_DPTR(s_object_name_list, _ObjectNameList,							0x650E6C);
	ENGINE_DPTR(s_object_globals, _ObjectGlobals,								0x650E90);
	ENGINE_DPTR(s_object_clusters, _ObjectClusters,								0x650E7C);
	ENGINE_DPTR(s_object_scripting_globals, _ObjectScriptingGlobals,			PTR_NULL);
	ENGINE_DPTR(t_cached_object_render_states_data, _CachedObjectRenderStates,	0x63FBE4);
	ENGINE_DPTR(s_object_placement_data, _ObjectPlacementData,					0x650EC4);
};


//////////////////////////////////////////////////////////////////////////
// Widgets.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_OBJECTS_WIDGETS
namespace GameState
{
	ENGINE_DPTR(t_widgets_data, _Widgets,			0x650EBC);
	ENGINE_DPTR(t_antenna_data, _Antenna,			0x650EC8);
	ENGINE_DPTR(t_cloth_data, _Cloth,				0x650ED0);
	ENGINE_DPTR(t_liquid_data, _Liquid,				PTR_UNKNOWN); // was this even in alpha?
	ENGINE_DPTR(t_lights_data, _Lights,				0x650EAC);
	ENGINE_DPTR(s_light_globals, _LightGlobals,		0x6D9420);
	ENGINE_DPTR(s_light_clusters, _LightClusters,	0x650EB0);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_OBJECTS
#endif