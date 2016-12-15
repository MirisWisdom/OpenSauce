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
	ENGINE_DPTR(t_contrail_data, _Contrails,					0x4EA944);
	ENGINE_DPTR(t_contrail_point_data, _ContrailPoints,			0x4EA940);
	ENGINE_DPTR(t_effect_data, _Effects,						0x4EA93C);
	ENGINE_DPTR(t_effect_location_data, _EffectLocations,		0x4EA938);
	ENGINE_DPTR(t_particle_system_data, _ParticleSystems,		0x510C74);
	ENGINE_DPTR(t_particles_data, _Particles,					0x51EC84);
	ENGINE_DPTR(t_particle_emitter_data, _ParticleEmitters,		0x51EC88);
	ENGINE_DPTR(t_particle_location_data, _ParticleLocations,	0x51EC8C);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_EFFECTS
#endif