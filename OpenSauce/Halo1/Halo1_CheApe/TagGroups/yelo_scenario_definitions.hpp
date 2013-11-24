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
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Removes Yelo-related data so that the scenario may be opened with no conflicts in the stock HEK.
		/// </summary>
		void YeloCleanseScenario(scenario* scnr);


		void ScenarioYeloLoadHookInitialize();
		void ScenarioYeloLoadHookDispose();
	};
};