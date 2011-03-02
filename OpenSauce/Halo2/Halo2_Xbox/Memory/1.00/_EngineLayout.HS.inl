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
// HS.cpp
#if __EL_INCLUDE_FILE_ID == __EL_HS_HS
namespace GameState
{
	ENGINE_DPTR(t_object_list_header_data, _ObjectListHeader,		0x4F55D8);
	ENGINE_DPTR(t_list_object_reference_data, _ListObjectReference,	0x4F55D4);
	ENGINE_DPTR(t_hs_nodes_data, _HsNodes,							0x4F9394);
	ENGINE_DPTR(t_hs_thread_data, _HsThread,						0x4F9384);
	ENGINE_DPTR(t_hs_globals_data, _HsGlobals,						0x4F9380);
	ENGINE_DPTR(t_recorded_animations_data, _RecordedAnimations,	0x4F5724);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_HS
#endif