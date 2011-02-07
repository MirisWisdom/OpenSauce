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
// Physics.cpp
#if __EL_INCLUDE_FILE_ID == __EL_PHYSICS_PHYSICS
namespace GameState
{
	ENGINE_DPTR(Havok::hkWorld, _HavokWorld,							NULL);
	ENGINE_DPTR(s_physics_constants, _PhysicsConstants,					NULL);
	ENGINE_DPTR(s_kill_volumes, _KillVolumes,							NULL);
	ENGINE_DPTR(s_breakable_surface_globals, _BreakableSurfaceGlobals,	NULL);
	ENGINE_DPTR(s_havok_globals, _HavokGlobals,							NULL);
	ENGINE_DPTR(s_havok_shape_globals, _HavokShapeGlobals,				NULL);
	ENGINE_DPTR(t_impacts_data, _Impacts,								NULL);
	ENGINE_DPTR(t_impact_arrays_data, _ImpactArrays,					NULL);
	ENGINE_DPTR(t_havok_components_data, _HavokComponents,				NULL);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_PHYSICS
#endif