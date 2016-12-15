/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Effects/Effects.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_EFFECTS
#define __EL_INCLUDE_FILE_ID	__EL_EFFECTS_EFFECTS
#include "Memory/_EngineLayout.inl"

	namespace Effects
	{
	};

	namespace GameState
	{
		t_contrail_data* _Contrails()					DPTR_IMP_GET(_Contrails);
		t_contrail_point_data* _ContrailPoints()		DPTR_IMP_GET(_ContrailPoints);
		t_effect_data* _Effects()						DPTR_IMP_GET(_Effects);
		t_effect_location_data* _EffectLocations()		DPTR_IMP_GET(_EffectLocations);
		t_particle_system_data* _ParticleSystems()		DPTR_IMP_GET(_ParticleSystems);
		t_particles_data* _Particles()					DPTR_IMP_GET(_Particles);
		t_particle_emitter_data* _ParticleEmitters()	DPTR_IMP_GET(_ParticleEmitters);
		t_particle_location_data* _ParticleLocations()	DPTR_IMP_GET(_ParticleLocations);
	};
};