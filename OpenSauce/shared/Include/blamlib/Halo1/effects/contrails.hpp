/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>

#include <blamlib/Halo1/effects/effects_constants.hpp>

namespace Yelo
{
	namespace Effects
	{
		//////////////////////////////////////////////////////////////////////////
		// game-state: contrails
		struct s_contrail_datum : TStructImpl(68)
		{
		};
		typedef Memory::DataArray<s_contrail_datum, Enums::k_maximum_number_of_contrails_per_map> 
			contrail_data_t;

		//////////////////////////////////////////////////////////////////////////
		// game-state: contrail points
		struct s_contrail_point_datum : TStructImpl(56)
		{
		};
		typedef Memory::DataArray<s_contrail_point_datum, Enums::k_maximum_number_of_contrail_points_per_map> 
			contrail_point_data_t;
	};
};