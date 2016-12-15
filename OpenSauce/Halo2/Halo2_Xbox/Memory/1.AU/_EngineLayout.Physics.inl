/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
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