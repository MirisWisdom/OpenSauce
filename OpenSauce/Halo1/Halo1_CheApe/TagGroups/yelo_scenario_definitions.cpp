/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"
#include "TagGroups/yelo_scenario_definitions.hpp"

#include <Blam/Halo1/project_yellow_shared_definitions.hpp>
#include <TagGroups/Halo1/scenario_definitions.hpp>

#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		static datum_index YeloPrepareDefinitionsYeloGlobals(project_yellow& yelo, const bool for_build_cache)
		{
			// internal name of the cache's Yelo Globals when the user doesn't supply a definition
			static cstring k_yelo_globals_name = "there they are all standing in a row";

			datum_index yelo_globals_index = datum_index::null;

			// check if the Yelo definition has a Yelo Globals tag reference and load it
			if(yelo.yelo_globals.name_length > 0 && yelo.yelo_globals.group_tag == project_yellow_globals::k_group_tag)
				yelo_globals_index = tag_load<project_yellow_globals>(yelo.yelo_globals.name, 0);
			else if(for_build_cache) // Only use the internal tag for cache-building only, not for editing
			{
				yelo_globals_index = tag_new<project_yellow_globals>(k_yelo_globals_name);

				if(!yelo_globals_index.IsNull())
					tag_reference_set<project_yellow_globals>(&yelo.yelo_globals, k_yelo_globals_name);
			}

			return yelo_globals_index;
		}

		static datum_index YeloPrepareDefinitionsYeloScenario(tag_reference& yelo_reference, const bool for_build_cache)
		{
			// internal name of the cache's Yelo tag when the user doesn't supply a definition
			static cstring k_yelo_name = "i've got a lovely bunch of corncobs";

			datum_index yelo_index = datum_index::null;

			// check if the scenario has a Yelo definition and load it and if not, create it
			bool yelo_isnt_new = false;
			if(yelo_reference.name_length > 0 && yelo_reference.group_tag == project_yellow::k_group_tag)
			{
				yelo_index = tag_load<project_yellow>(yelo_reference.name, 0);
				yelo_isnt_new = true;
			}
			else if(for_build_cache) // Only use the internal tag for cache-building only, not for editing
				yelo_index = tag_new<project_yellow>(k_yelo_name);

			// Just in case the tag fails to load or fails to be created
			if(!yelo_index.IsNull())
			{
				project_yellow* yelo = tag_get<project_yellow>(yelo_index);

				// set the scenario's yelo reference if it isn't already set-up
				if(!yelo_isnt_new)
					tag_reference_set<project_yellow>(&yelo_reference, k_yelo_name);

				YeloPrepareDefinitionsYeloGlobals(*yelo, for_build_cache);
			}

			return yelo_index;
		}

		Yelo::datum_index YeloPrepareDefinitions(cstring scenario_name, const bool for_build_cache)
		{
			// If we're not building a cache file, just load the scenario into memory without any of its references
			datum_index scenario_index = tag_load<TagGroups::scenario>(scenario_name, for_build_cache ? 
				Flags::_tag_load_verify_exist_first : 
				Flags::_tag_load_non_resolving_references);

			if(!scenario_index.IsNull())
			{
				TagGroups::scenario* scenario = tag_get<TagGroups::scenario>(scenario_index);

				datum_index yelo = YeloPrepareDefinitionsYeloScenario(
					scenario->GetYeloReferenceHack(), for_build_cache);

				// if we're not building a cache, then this is sapien and we want it to load 
				// the scenario and all of it's dependencies after we return the code flow 
				// back to it
				if(!for_build_cache) tag_unload(scenario_index);

				return yelo;
			}

			return datum_index::null;
		}


		//////////////////////////////////////////////////////////////////////////
		// scenario_yelo_load
		// This is a Tool and Sapien specific hack to use the Yelo's globals tag 
		// override reference.
		static void scenario_yelo_load(cstring scenario_name)
		{
			// References to the string "globals\globals"
			cstring* K_GLOBALS_TAG_NAME_REFERENCES[] = {
				CAST_PTR(cstring*, PLATFORM_VALUE(NULL, 0x4434CA, 0x51675F)),
				CAST_PTR(cstring*, PLATFORM_VALUE(NULL, 0x443C95, 0x517525)),
#if PLATFORM_ID == PLATFORM_TOOL
				CAST_PTR(cstring*, PLATFORM_VALUE(NULL, 0x4541A2, NULL)),
#endif
			};
			// Address of the string "globals\globals"
			cstring K_GLOBALS_TAG_NAME_ADDRESS = CAST_PTR(cstring, 
				PLATFORM_VALUE(NULL, 0x612FA0, 0x9116CC));

			// The new globals reference name which we're going to force the tool code to use.
			// Defaults to the stock globals tag name.
			cstring globals_tag_name = K_GLOBALS_TAG_NAME_ADDRESS;

			// A little trick to reset the globals tag back to the stock reference name
			if(scenario_name == NULL)
				globals_tag_name = K_GLOBALS_TAG_NAME_ADDRESS;
			else
			{
				const bool k_for_cache_build = 
#if PLATFORM_ID == PLATFORM_TOOL
					true
#elif PLATFORM_ID == PLATFORM_SAPIEN || PLATFORM_ID == PLATFORM_GUERILLA
					false
#endif
					;
				// Load the Yelo definitions then check to see if there is a globals override 
				// reference and update all tool code to then use its name
				datum_index yelo_index = YeloPrepareDefinitions(scenario_name, k_for_cache_build);
				if(!yelo_index.IsNull())
				{
					project_yellow* yelo = tag_get<project_yellow>(yelo_index);

					if(yelo->game_globals.name_length > 0)
						globals_tag_name = yelo->game_globals.name;
				}
			}

			if(globals_tag_name != NULL)
				for(int32 x = 0; x < NUMBEROF(K_GLOBALS_TAG_NAME_REFERENCES); x++)
					*K_GLOBALS_TAG_NAME_REFERENCES[x] = globals_tag_name;
			else
			{
				YELO_ERROR(_error_message_priority_warning, "CheApe: scenario_yelo_load failed to do anything! %s", 
					scenario_name != NULL ? scenario_name : "NULL");
			}
		}

		static const uint32 SCENARIO_LOAD_HOOK = 
			PLATFORM_VALUE(NULL, 0x45546B, // call game_globals_preprocess
				0x6181BB); // call scenario_load
		static uint32 SCENARIO_LOAD_HOOK_restore_point = 0;
#if PLATFORM_ID == PLATFORM_TOOL
		API_FUNC_NAKED static void PLATFORM_API scenario_yelo_load_hook()
		{
			static uint32 GAME_GLOBALS_PREPROCESS = PLATFORM_VALUE(NULL, 0x454190, NULL);

			__asm {
				push	eax

				push	eax
				call	scenario_yelo_load

				pop		eax
				call	GAME_GLOBALS_PREPROCESS
				retn
			}
		}
#elif PLATFORM_ID == PLATFORM_SAPIEN
		API_FUNC_NAKED static void PLATFORM_API scenario_yelo_load_hook(cstring scenario_name)
		{
			static uint32 SCENARIO_LOAD = PLATFORM_VALUE(NULL, NULL, 0x5174E0);

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
#if PLATFORM_ID != PLATFORM_GUERILLA
			SCENARIO_LOAD_HOOK_restore_point = 
				Memory::WriteRelativeCall(&scenario_yelo_load_hook, CAST_PTR(void*, SCENARIO_LOAD_HOOK));
#endif
		}

		void ScenarioYeloLoadHookDispose()
		{
#if PLATFORM_ID != PLATFORM_GUERILLA
			Memory::WriteRelativeCall(CAST_PTR(void*, SCENARIO_LOAD_HOOK_restore_point),
				CAST_PTR(void*, SCENARIO_LOAD_HOOK));

			scenario_yelo_load(NULL);
#endif
		}
	};
};