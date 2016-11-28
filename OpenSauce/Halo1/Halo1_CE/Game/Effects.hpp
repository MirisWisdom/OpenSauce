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
		contrail_data_t&					Contrails();
		contrail_point_data_t&				ContrailPoints();
		particle_data_t&					Particles();
		effect_data_t&						Effects();
		effect_location_data_t&				EffectLocations();
		particle_systems_data_t&			ParticleSystems();
		particle_system_particles_data_t&	ParticleSystemParticles();


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
		typedef Memory::DataArray<s_decals_datum, 2048> decals_data_t;
		decals_data_t*						Decals();
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