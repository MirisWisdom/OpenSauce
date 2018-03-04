/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/memory/data.hpp>

#include <blamlib/effects/effects_constants.hpp>
#include <YeloLib/effects/effects_constants_yelo.hpp>
#include <YeloLib/cseries/cseries_yelo_base.hpp>
#include <YeloLib/cseries/cseries_yelo_base.hpp>

namespace Yelo
{
	namespace Effects
	{
		//////////////////////////////////////////////////////////////////////////
		// game-state: particle-systems
		struct s_particle_systems_datum : TStructImpl(344)
		{
		};
		typedef Memory::DataArray<	s_particle_systems_datum, 
									Enums::k_maximum_number_of_particle_systems_per_map> 
			particle_systems_data_t;

		//////////////////////////////////////////////////////////////////////////
		// game-state: particle-system particles
		struct s_particle_system_particles_datum : TStructImpl(128)
		{
		};
		typedef Memory::DataArray<	s_particle_system_particles_datum, 
									Enums::k_maximum_number_of_particle_system_particles_per_map,
									Enums::k_maximum_number_of_particle_system_particles_per_map_upgrade> 
			particle_system_particles_data_t;
	};
};