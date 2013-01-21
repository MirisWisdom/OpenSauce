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
		struct scenario;

		// Removes Yelo-related data so that the scenario may be opened with no conflicts in the stock HEK
		void YeloCleanseScenario(scenario* scnr);

		// Returns the tag index of the scenario's [project_yellow] tag or datum_index::null if there isn't one or it fails to load
		datum_index YeloPrepareDefinitions(cstring scenario_name, const bool for_build_cache);


		void ScenarioYeloLoadHookInitialize();
		void ScenarioYeloLoadHookDispose();
	};
};