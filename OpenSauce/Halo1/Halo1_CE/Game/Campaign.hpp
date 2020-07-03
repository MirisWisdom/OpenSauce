/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

// Campaign upgrades as of 1.10 are unsupported/deprecated/fuck off
#define YELO_CAMPAIGN_UPGRADES_ENABLED FALSE

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Campaign
	{
		void Initialize();
		void Dispose();
	};
};
#endif