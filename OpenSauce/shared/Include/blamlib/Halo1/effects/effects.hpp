/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>

#include <blamlib/Halo1/effects/effects_constants.hpp>
#include <YeloLib/Halo1/effects/effects_constants_yelo.hpp>

namespace Yelo
{
	namespace Effects
	{
		//////////////////////////////////////////////////////////////////////////
		// game-state: effects
		struct s_effect_datum : TStructImpl(252)
		{
		};
		typedef Memory::DataArray<	s_effect_datum, 
									Enums::k_maximum_number_of_effects_per_map,
									Enums::k_maximum_number_of_effects_per_map_upgrade> 
			t_effect_data;


		//////////////////////////////////////////////////////////////////////////
		// game-state: effect locations
		struct s_effect_location_datum : TStructImpl(60)
		{
		};
		typedef Memory::DataArray<	s_effect_location_datum, 
									Enums::k_maximum_number_of_effect_locations_per_map,
									Enums::k_maximum_number_of_effect_locations_per_map_upgrade> 
			t_effect_location_data;
	};
};