/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Effects.cpp
#if __EL_INCLUDE_FILE_ID == __EL_EFFECTS_EFFECTS
namespace GameState
{
	ENGINE_DPTR(t_contrail_data, _Contrails,					NULL);
	ENGINE_DPTR(t_contrail_point_data, _ContrailPoints,			NULL);
	ENGINE_DPTR(t_effect_data, _Effects,						NULL);
	ENGINE_DPTR(t_effect_location_data, _EffectLocations,		NULL);
	ENGINE_DPTR(t_particle_system_data, _ParticleSystems,		NULL);
	ENGINE_DPTR(t_particles_data, _Particles,					NULL);
	ENGINE_DPTR(t_particle_emitter_data, _ParticleEmitters,		NULL);
	ENGINE_DPTR(t_particle_location_data, _ParticleLocations,	NULL);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_EFFECTS
#endif