/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/yelo_scenario_definitions.hpp"

#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

#include "Engine/AI.hpp"
#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		//////////////////////////////////////////////////////////////////////////
		// scenario_yelo_load
		// This is a Tool and Sapien specific hack to use the Yelo's globals tag 
		// override reference.
		static void scenario_yelo_load(cstring scenario_name)
		{
			// References to the string "globals\globals"
			static cstring* K_GLOBALS_TAG_NAME_REFERENCES[] = {
				PLATFORM_PTR(cstring*, nullptr, 0x4434CA, 0x51675F),
				PLATFORM_PTR(cstring*, nullptr, 0x443C95, 0x517525),
#if PLATFORM_TYPE == PLATFORM_TOOL
				PLATFORM_PTR(cstring*, nullptr, 0x4541A2, nullptr),
#endif
			};
			// Address of the string "globals\globals"
			static cstring const K_GLOBALS_TAG_NAME_ADDRESS = PLATFORM_PTR(cstring,
				nullptr, 0x612FA0, 0x9116CC);

			static char local_globals_tag_name[Enums::k_max_tag_name_length + 1];

			// The new globals reference name which we're going to force the tool code to use.
			// Defaults to the stock globals tag name.
			cstring globals_tag_name = K_GLOBALS_TAG_NAME_ADDRESS;

			// A little trick to reset the globals tag back to the stock reference name
			if (scenario_name == nullptr)
			{
				globals_tag_name = K_GLOBALS_TAG_NAME_ADDRESS;
				local_globals_tag_name[0] = '\0';
			}
			else
			{
				const bool k_for_cache_build = PLATFORM_VALUE(false, true, false);
				// Load the Yelo definitions then check to see if there is a globals override 
				// reference and update all tool code to then use its name
				datum_index yelo_index = Scenario::YeloPrepareDefinitions(scenario_name, k_for_cache_build);
				if(!yelo_index.IsNull())
				{
					auto* yelo = blam::tag_get<project_yellow>(yelo_index);

					if (yelo->game_globals.name_length > 0)
					{
						strcpy_s(local_globals_tag_name, yelo->game_globals.name);
						globals_tag_name = local_globals_tag_name;
					}
				}
			}

			if(globals_tag_name != nullptr)
				for(auto ptr : K_GLOBALS_TAG_NAME_REFERENCES)
					*ptr = globals_tag_name;
			else
			{
				YELO_WARN("CheApe: scenario_yelo_load failed to do anything! %s", 
					scenario_name != nullptr ? scenario_name : "nullptr");
			}

			AI::Transform::CollateTransformCollections();
		}

		static const uintptr_t SCENARIO_LOAD_HOOK = 
			PLATFORM_VALUE(NULL, 0x45546B, // call game_globals_preprocess
				0x6181BB); // call scenario_load
		static uintptr_t SCENARIO_LOAD_HOOK_restore_point = NULL;
#if PLATFORM_TYPE == PLATFORM_TOOL
		API_FUNC_NAKED static void PLATFORM_API scenario_yelo_load_hook()
		{
			static const uintptr_t GAME_GLOBALS_PREPROCESS = PLATFORM_VALUE(nullptr, 0x454190, nullptr);

			__asm {
				push	eax

				push	eax
				call	scenario_yelo_load

				pop		eax
				call	GAME_GLOBALS_PREPROCESS
				retn
			}
		}
#elif PLATFORM_TYPE == PLATFORM_SAPIEN
		API_FUNC_NAKED static void PLATFORM_API scenario_yelo_load_hook(cstring scenario_name)
		{
			static const uintptr_t SCENARIO_LOAD = PLATFORM_VALUE(nullptr, nullptr, 0x5174E0);

			__asm {
				push	ebp
				mov		ebp, esp

				push	scenario_name
				call	scenario_yelo_load

				push	scenario_name
				call	SCENARIO_LOAD
				add		esp, 4 * 1
				pop		ebp
				retn
			}
		}
#endif

		void ScenarioYeloLoadHookInitialize()
		{
#if PLATFORM_TYPE != PLATFORM_GUERILLA
			SCENARIO_LOAD_HOOK_restore_point = 
				Memory::WriteRelativeCall(&scenario_yelo_load_hook, CAST_PTR(void*, SCENARIO_LOAD_HOOK));
#endif
		}

		void ScenarioYeloLoadHookDispose()
		{
#if PLATFORM_TYPE != PLATFORM_GUERILLA
			Memory::WriteRelativeCall(CAST_PTR(void*, SCENARIO_LOAD_HOOK_restore_point),
				CAST_PTR(void*, SCENARIO_LOAD_HOOK));

			scenario_yelo_load(nullptr);
#endif
		}
	};
};