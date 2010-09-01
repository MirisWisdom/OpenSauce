/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
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

	FUNC_PTR(OBJECTS_UPDATE_HOOK,			0x4F8D6E, 0x4DFB10);
	FUNC_PTR(OBJECTS_GARBAGE_COLLECTION,	0x4FD8F0, 0x4E47E0);
	FUNC_PTR(UNIT_CAN_ENTER_SEAT_MOD,		0x56A3E1, 0x518D71);
	FUNC_PTR(OBJECT_DAMAGE_OBJECT_BODY_HOOK,0x4F34AA, 0x4DC70A);

	namespace Weapon
	{
		FUNC_PTR(FIRST_PERSON_WEAPON_RENDER_UPDATE,							0x4953D0, FUNC_PTR_NULL);
		FUNC_PTR(RENDER_WINDOW_CALL_HOOK_FIRST_PERSON_WEAPON_RENDER_UPDATE,	0x50FD37, FUNC_PTR_NULL);
	};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_OBJECTS
#endif