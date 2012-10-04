/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <Blam/Halo1/project_yellow_shared_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		extern struct s_yelo_definition_globals {
			bool initialized;
			PAD24;
		}_yelo_definition_globals;

		void YeloDefinitionsInitialize();
		void YeloDefinitionsDispose();

		void YeloGlobalsDefinitionCull(TagGroups::project_yellow_globals* globals);

#if PLATFORM_ID == PLATFORM_TOOL
		bool YeloToolCheckTagsForGameStateUpgradeRequirements();
#endif
	};
};