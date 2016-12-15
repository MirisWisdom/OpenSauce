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
	ENGINE_DPTR(Havok::hkWorld, _HavokWorld,							0x6D9E3C);
	ENGINE_DPTR(s_physics_constants, _PhysicsConstants,					0x6D9E64);
	ENGINE_DPTR(s_kill_volumes, _KillVolumes,							PTR_UNKNOWN);
	ENGINE_DPTR(s_breakable_surface_globals, _BreakableSurfaceGlobals,	0x660CB0);
	ENGINE_DPTR(s_havok_globals, _HavokGlobals,							PTR_UNKNOWN);
	ENGINE_DPTR(s_havok_shape_globals, _HavokShapeGlobals,				0x6DA6B4);
	ENGINE_DPTR(t_impacts_data, _Impacts,								0x6C5C6C);
	ENGINE_DPTR(t_impact_arrays_data, _ImpactArrays,					PTR_UNKNOWN);
	ENGINE_DPTR(t_havok_components_data, _HavokComponents,				0x6D9E38);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_PHYSICS
#endif