/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#if PLATFORM_ID == PLATFORM_TOOL
#include "Tool/ToolMisc.hpp"

#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

#include "TagGroups/TagGroups.hpp"
#include "TagGroups/yelo_scenario_definitions.hpp"

namespace Yelo
{
	namespace Tool
	{
		void PLATFORM_API scenario_compile_scripts(void** arguments)
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

		void PLATFORM_API scenario_cleanse_of_yelo_data(void** arguments)
		{
			struct s_arguments {
				cstring scenario_name;
			}* args = CAST_PTR(s_arguments*, arguments);

			datum_index scnr_index = tag_load<TagGroups::scenario>(args->scenario_name, Flags::_tag_load_non_resolving_references);

			if(scnr_index.IsNull())
			{
				YELO_ERROR(_error_message_priority_warning, "OS_tool: failed to open scenario for editing: %s", args->scenario_name);
				return;
			}

			TagGroups::scenario* scnr = tag_get<TagGroups::scenario>(scnr_index);
			printf_s("stock'ifying scenario: %s...\n", args->scenario_name);
			TagGroups::YeloCleanseScenario(scnr);
			printf_s("saving...\n");
			tag_save(scnr_index);
			printf_s("done.\n");
			tag_unload(scnr_index);
		}
	};
};
#endif