/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

// [terminology]	- [tag group]
// yelo scenario	- project_yellow
// yelo globals		- project_yellow_globals
// blam scenario	- scenario

namespace Yelo
{
	namespace TagGroups
	{
		struct project_yellow_globals;
		struct project_yellow;
	};

	namespace Scenario
	{
		void ProjectYellowInitialize();
		void ProjectYellowDispose();
		void ProjectYellowInitializeForNewMap();// should be called in scenario_tags_load
		void ProjectYellowDisposeFromOldMap();	// should be called in scenario_tags_unload

		const TagGroups::project_yellow_globals* GetYeloGlobals();
		const TagGroups::project_yellow* GetYelo();

		TagGroups::project_yellow_globals* TryGetYeloGlobalsForModification();
		TagGroups::project_yellow* TryGetYeloForModification();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Process a blam scenario for the current operating mode (editing or cache building). </summary>
		///
		/// <param name="scenario_name">  	Name of the scenario tag. </param>
		/// <param name="for_build_cache">	True if we're building a cache file, false if we're editing. </param>
		///
		/// <returns>
		/// 	Returns the blam scenario's [project_yellow] index or datum_index::null if there isn't one or it fails to load.
		/// </returns>
		datum_index YeloPrepareDefinitions(cstring scenario_name, const bool for_build_cache);
	};
};