/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once
#include "TagGroups/project_yellow_definitions.hpp"

namespace Yelo
{
	namespace Scenario { namespace ScenarioInfo
	{
		const TagGroups::s_project_yellow_scenario_information*		ScenarioInfo();

		const bool													HasBSPInfo();
		const int16													BSPInfoIndex();
		const TagGroups::s_scenario_information_bsp*				BSPInfo();

		void InitializeForNewMap();
		void DisposeFromOldMap();

		void InitializeForNewBSP();
		void DisposeFromOldBSP();

		void Initialize();
		void Dispose();
	};};
};