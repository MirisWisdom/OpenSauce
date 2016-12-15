/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <blamlib/Halo2/memory/data.hpp>

namespace Yelo
{
	namespace Effects
	{
	};

	namespace GameState
	{
		struct s_contrail_datum : TStructImpl(72)
		{
		};
		typedef Memory::DataArray<s_contrail_datum, 256> t_contrail_data;
		t_contrail_data* _Contrails();

		struct s_contrail_point_datum : TStructImpl(56)
		{
		};
		typedef Memory::DataArray<s_contrail_point_datum, 1024> t_contrail_point_data;
		t_contrail_point_data* _ContrailPoints();

		struct s_effect_datum : TStructImpl(400)
		{
		};
		typedef Memory::DataArray<s_effect_datum, 256> t_effect_data;
		t_effect_data* _Effects();

		struct s_effect_location_datum : TStructImpl(60)
		{
		};
		typedef Memory::DataArray<s_effect_location_datum, 512> t_effect_location_data;
		t_effect_location_data* _EffectLocations();

		struct s_particle_system_datum : TStructImpl(84)
		{
		};
		typedef Memory::DataArray<s_particle_system_datum, 128> t_particle_system_data;
		t_particle_system_data* _ParticleSystems();

		struct s_particles_datum : TStructImpl(64)
		{
		};
		typedef Memory::DataArray<s_particles_datum, 1024> t_particles_data;
		t_particles_data* _Particles();

		struct s_particle_emitter_datum : TStructImpl(76)
		{
		};
		typedef Memory::DataArray<s_particle_emitter_datum, 256> t_particle_emitter_data;
		t_particle_emitter_data* _ParticleEmitters();

		struct s_particle_location_datum : TStructImpl(52)
		{
		};
		typedef Memory::DataArray<s_particle_location_datum, 256> t_particle_location_data;
		t_particle_location_data* _ParticleLocations();
	};
};