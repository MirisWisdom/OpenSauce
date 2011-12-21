/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <Blam/Halo1/BlamScriptingDefinitions.hpp>
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		struct scripting_block;
	};

	namespace Enums
	{
		enum {
			_hs_function_profile_load = 0x20A,

			_hs_function_profile_load_call_offset = 0x1F, // offset of the implementation call in the evaluation function (at least in sapien)
		};
	};

	namespace Scripting
	{
		size_t GetTotalScenarioHsSyntaxData();
		size_t GetTotalScenarioHsSyntaxDataUpgrade();

		void Initialize();
		void Dispose();

		// Turn on the script node upgrades (these upgrades are not backwards compatible with stock game builds)
		void InitializeScriptNodeUpgrades();
		// Turn off the script node upgrades (ie, reverts the max script nodes back the stock settings)
		void DisposeScriptNodeUpgrades();

		hs_function_definition* GetFunctionDefintion(int16 function_index);

		// Copy the scripting definitions CheApe loaded into the tool code from the memory map
		// [only_internals] - Copy definitions that are part of the stock OpenSauce source code 
		// only, else copy only the user's definitions
		void ScriptingBlockAddDefinitions(
			TAG_TBLOCK(& script_block, TagGroups::scripting_block), bool only_internals);
	};
};