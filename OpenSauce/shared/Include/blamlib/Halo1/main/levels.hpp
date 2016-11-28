/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_number_of_campaigns = 4, // based on Halo2+

			k_maximum_number_of_campaign_levels = 10, // Halo1 is limited to this, Halo2+ allows up to 20
		};

		enum halo1_campaign_level
		{
			_halo1_campaign_level_a10,
			_halo1_campaign_level_a30,
			_halo1_campaign_level_a50,
			_halo1_campaign_level_b30,
			_halo1_campaign_level_b40,
			_halo1_campaign_level_c10,
			_halo1_campaign_level_c20,
			_halo1_campaign_level_c40,
			_halo1_campaign_level_d20,
			_halo1_campaign_level_d40,

			k_number_of_halo1_campaign_levels,
		}; BOOST_STATIC_ASSERT( k_number_of_halo1_campaign_levels == k_maximum_number_of_campaign_levels );
	};

	namespace Main
	{
		// actually scenario paths
		extern cstring k_halo1_campaign_level_names[Enums::k_number_of_halo1_campaign_levels];
	};

	namespace blam
	{
		int16 PLATFORM_API main_get_campaign_level_from_name(cstring level_name);

		cstring PLATFORM_API main_get_campaign_level_name(_enum level_index);
	};
};