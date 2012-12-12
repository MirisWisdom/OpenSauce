/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

namespace Yelo
{
	namespace TagGroups
	{
		struct project_yellow_globals;

		extern struct s_yelo_definition_globals {
			bool initialized;
			PAD24;
		}_yelo_definition_globals;

		void YeloDefinitionsInitialize();
		void YeloDefinitionsDispose();

#if PLATFORM_ID == PLATFORM_TOOL
		bool YeloToolCheckTagsForGameStateUpgradeRequirements();
#endif
	};
};