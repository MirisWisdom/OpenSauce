/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/hs/hs.hpp>

#include "TagGroups/TagGroups.hpp"

/*
Notes on custom script and memory upgrades and stock game compatibility:
	* The runtime (ie, game) still has the code left in for recompiling the script files. Since cache build process culls debug data like
	hs_source_files, there's nothing to recompile. If tool "somehow" left the source files in though, the runtime would recompile them.
	* There are a couple of factors which causes the stock runtime to think it needs to recompile the source files, for OS they're namely:
		1) If scenario->hs_syntax_data's size is _NOT EQUAL_ to Enums::k_total_scenario_hs_syntax_data
		2) If there is a function or global name which the engine cannot resolve.
	* OS enables higher HS tag limits (on by default) and also allows you to define custom function and globals (must be explicitly setup).
	* Since custom script definitions and one of the memory upgrades are incompatible with the stock runtime, we have to be explicit when
	turning them on when in tool builds. In build-cache-file-ex, they'll get turned on when use-memory-upgrades is true. We wrap the
	build-cache-file command to give an explicit warning and to turn off memory upgrades
*/

namespace Yelo
{
	namespace TagGroups
	{
		struct s_scripting_definitions;
	};

	namespace Enums
	{
		enum {
			_hs_function_profile_load = 0x20A,

			/// <summary>	offset of the implementation asm call in the evaluation function </summary>
			/// <remarks>	only verified for sapien </remarks>
			_hs_function_profile_load_call_offset = 0x1F,
		};
	};

	namespace Scripting
	{
		size_t GetTotalScenarioHsSyntaxData();
		size_t GetTotalScenarioHsSyntaxDataUpgrade();

		void Initialize();
		void Dispose();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Hooks the script definitions in CheApe.map into the engine. </summary>
		///
		/// <remarks>
		/// 	This must be explicitly called under PLATFORM_TOOL builds (other platforms do this 'automatically')
		/// </remarks>
		void InitializeCustomScriptingDefinitions();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Turn on the script node upgrades (these upgrades are not backwards compatible with stock game builds)
		/// </summary>
		void InitializeScriptNodeUpgrades();
		/// <summary>	Turn off the script node upgrades (ie, reverts the max script nodes back the stock settings) </summary>
		void DisposeScriptNodeUpgrades();

		hs_function_definition* GetFunctionDefintion(int16 function_index);
	};
};