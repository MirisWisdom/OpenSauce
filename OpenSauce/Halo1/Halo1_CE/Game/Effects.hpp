/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "Memory/Data.hpp"

namespace Yelo
{
	namespace Effects
	{
		//////////////////////////////////////////////////////////////////////////
		// game-state: contrails
		struct s_contrail_datum : TStructImpl(68)
		{
		};
		typedef Memory::DataArray<s_contrail_datum, 256> t_contrail_data;
		t_contrail_data*					Contrails();
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// game-state: contrail points
		struct s_contrail_point_datum : TStructImpl(56)
		{
		};
		typedef Memory::DataArray<s_contrail_point_datum, 1024> t_contrail_point_data;
		t_contrail_point_data*				ContrailPoints();
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// game-state: particles
		struct s_particle_datum : TStructImpl(112)
		{
		};
		typedef Memory::DataArray<s_particle_datum, 1024> t_particle_data;
		t_particle_data*					Particles();
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// game-state: effects
		struct s_effect_datum : TStructImpl(252)
		{
		};
		typedef Memory::DataArray<s_effect_datum, 256> t_effect_data;
		t_effect_data*						Effects();
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// game-state: effect locations
		struct s_effect_location_datum : TStructImpl(60)
		{
		};
		typedef Memory::DataArray<s_effect_location_datum, 512> t_effect_location_data;
		t_effect_location_data*				EffectLocations();
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// game-state: particle-systems
		struct s_particle_systems_datum : TStructImpl(344)
		{
		};
		typedef Memory::DataArray<s_particle_systems_datum, 64> t_particle_systems_data;
		t_particle_systems_data*			ParticleSystems();
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// game-state: particle-system particles
		struct s_particle_system_particles_datum : TStructImpl(128)
		{
		};
		typedef Memory::DataArray<s_particle_system_particles_datum, 512> t_particle_system_particles_data;
		t_particle_system_particles_data*	ParticleSystemParticles();
		//////////////////////////////////////////////////////////////////////////



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