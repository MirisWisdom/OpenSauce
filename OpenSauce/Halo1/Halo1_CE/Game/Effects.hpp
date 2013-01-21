/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>
#include <blamlib/Halo1/memory/lruv_cache.hpp>

#include <blamlib/Halo1/effects/contrails.hpp>
#include <blamlib/Halo1/effects/effects.hpp>
#include <blamlib/Halo1/effects/particle_systems.hpp>
#include <blamlib/Halo1/effects/particles.hpp>
#include <blamlib/Halo1/effects/weather_particle_systems.hpp>

namespace Yelo
{
	namespace Effects
	{
		t_contrail_data*					Contrails();
		t_contrail_point_data*				ContrailPoints();
		t_particle_data*					Particles();
		t_effect_data*						Effects();
		t_effect_location_data*				EffectLocations();
		t_particle_systems_data*			ParticleSystems();
		t_particle_system_particles_data*	ParticleSystemParticles();


		//////////////////////////////////////////////////////////////////////////
		// game-state: structure detail-objects
		struct s_structure_detail_objects_data : TStructImpl(42032)
		{
		};
		s_structure_detail_objects_data*		StructureDetailObjects();
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// game-state: structure decals
		struct s_structure_decals_data : TStructImpl(4)
		{
		};
		s_structure_decals_data*				StructureDecals();
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// game-state: breakable-surface globals
		struct s_breakable_surface_globals_data : TStructImpl(16900)
		{
		};
		s_breakable_surface_globals_data*		BreakableSurfaceGlobals();
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// game-state: decals
		struct s_decals_datum : TStructImpl(56)
		{
		};
		typedef Memory::DataArray<s_decals_datum, 2048> t_decals_data;
		t_decals_data*						Decals();
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// game-state: decal globals
		struct s_decal_globals_data : TStructImpl(10252)
		{
		};
		s_decal_globals_data*					DecalGlobals();
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// game-state: decal vertex cache
		struct s_decal_vertex_cache_data
		{
			Memory::s_lruv_cache cache;
			Memory::s_data_array header;

			Memory::s_lruv_cache_block datums[2048];
		};
		s_decal_vertex_cache_data*			DecalVertexCache();
		//////////////////////////////////////////////////////////////////////////


		void Initialize();
		void Dispose();
		void PLATFORM_API Update();
	};
};