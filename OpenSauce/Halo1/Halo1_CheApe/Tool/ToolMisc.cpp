/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#if PLATFORM_TYPE == PLATFORM_TOOL
#include "Tool/ToolMisc.hpp"

#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Tool
	{
		void PLATFORM_API scenario_compile_scripts(char* arguments[])
		{
			struct s_arguments {
				cstring scenario_name;
			}* args = CAST_PTR(s_arguments*, arguments);

			// load scenario tag

			// call 4C27C0 (hs_scenario_postprocess or hs_scenario_initialize_for_new_map?)

			// save scenario tag

			// might have to call some of the system dispose functions

			printf_s("FUCK YOU! Oh and %s has to be the worse scenario ever. kill you're saelf, hth.\n", args->scenario_name);
			printf_s("If you can see this, it means this feature hasn't been implemented.\n");
		}
	};
};
#endif