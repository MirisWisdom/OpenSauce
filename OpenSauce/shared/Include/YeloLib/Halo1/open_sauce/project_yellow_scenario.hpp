/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace TagGroups
	{
		struct project_yellow_globals_cv;
		struct project_yellow_globals;
		struct project_yellow;
	};

	namespace Scenario
	{
		void ProjectYellowInitialize();
		void ProjectYellowDispose();
		void ProjectYellowInitializeForNewMap();

		const TagGroups::project_yellow_globals_cv* GetYeloCvGlobals();
		const TagGroups::project_yellow_globals* GetYeloGlobals();
		const TagGroups::project_yellow* GetYelo();
	};
};