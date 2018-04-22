/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blam/memory/data.hpp>

#include <blam/effects/effects_constants.hpp>
#include <yelo/effects/effects_constants_yelo.hpp>
#include <yelo/cseries/cseries_yelo_base.hpp>

namespace Yelo
{
	namespace Effects
	{
		//////////////////////////////////////////////////////////////////////////
		// game-state: particles
		struct s_particle_datum : TStructImpl(112)
		{
		};
		typedef Memory::DataArray<	s_particle_datum, 
									Enums::k_maximum_number_of_particles_per_map,
									Enums::k_maximum_number_of_particles_per_map_upgrade> 
			particle_data_t;
	};
};