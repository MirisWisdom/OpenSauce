/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Physics/Physics.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_PHYSICS
#define __EL_INCLUDE_FILE_ID	__EL_PHYSICS_PHYSICS
#include "Memory/_EngineLayout.inl"

	namespace GameState
	{
		const real s_physics_constants::k_gravity = 4.1712594f;

		Havok::hkWorld* _HavokWorld()							DPTR_IMP_GET(_HavokWorld);
		s_physics_constants* _PhysicsConstants()				DPTR_IMP_GET(_PhysicsConstants);
		s_kill_volumes* _KillVolumes()							DPTR_IMP_GET(_KillVolumes);
		s_breakable_surface_globals* _BreakableSurfaceGlobals()	DPTR_IMP_GET(_BreakableSurfaceGlobals);
		s_havok_globals* _HavokGlobals()						DPTR_IMP_GET(_HavokGlobals);
		s_havok_shape_globals* _HavokShapeGlobals()				DPTR_IMP_GET(_HavokShapeGlobals);
		t_impacts_data* _Impacts()								DPTR_IMP_GET(_Impacts);
		t_impact_arrays_data* _ImpactArrays()					DPTR_IMP_GET(_ImpactArrays);
		t_havok_components_data* _HavokComponents()				DPTR_IMP_GET(_HavokComponents);
	};
};