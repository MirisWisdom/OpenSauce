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

			k_maximum_number_of_campaign_maps = 10, // Halo1 is limited to this, Halo2+ allows up to 20
		};

		enum halo1_campaign_map
		{
			_halo1_campaign_map_a10,
			_halo1_campaign_map_a30,
			_halo1_campaign_map_a50,
			_halo1_campaign_map_b30,
			_halo1_campaign_map_b40,
			_halo1_campaign_map_c10,
			_halo1_campaign_map_c20,
			_halo1_campaign_map_c40,
			_halo1_campaign_map_d20,
			_halo1_campaign_map_d40,

			_halo1_campaign_map,
		}; BOOST_STATIC_ASSERT( _halo1_campaign_map == k_maximum_number_of_campaign_maps );
	};

	namespace Main
	{
	};
};