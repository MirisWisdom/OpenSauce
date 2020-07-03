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
	ENGINE_DPTR(Havok::hkWorld, _HavokWorld,							0x51E9A4);
	ENGINE_DPTR(s_physics_constants, _PhysicsConstants,					0x51E9C4);
	ENGINE_DPTR(s_kill_volumes, _KillVolumes,							0x51E9C8);
	ENGINE_DPTR(s_breakable_surface_globals, _BreakableSurfaceGlobals,	0x4ED280);
	ENGINE_DPTR(s_havok_globals, _HavokGlobals,							0x51E9A0);
	ENGINE_DPTR(s_havok_shape_globals, _HavokShapeGlobals,				0x51E9CC);
	ENGINE_DPTR(t_impacts_data, _Impacts,								0x51EBFC);
	ENGINE_DPTR(t_impact_arrays_data, _ImpactArrays,					0x51EC00);
	ENGINE_DPTR(t_havok_components_data, _HavokComponents,				0x51E9B8);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_PHYSICS
#endif