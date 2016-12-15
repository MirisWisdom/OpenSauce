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
		bool YeloDefinitionsInitialized();

		void YeloDefinitionsInitialize();
		void YeloDefinitionsDispose();

#if PLATFORM_TYPE == PLATFORM_TOOL
		bool YeloToolCheckTagsForGameStateUpgradeRequirements();
#endif
	};
};