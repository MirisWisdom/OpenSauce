/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/cache/cache_files_structures.hpp>
#include <blamlib/Halo1/main/console.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include <YeloLib/Halo1/hs/hs_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

namespace Yelo
{
	namespace Scenario
	{
		using namespace TagGroups;

		static project_yellow_globals null_yelo_globals;

		static project_yellow null_yelo;
		static project_yellow null_yelo_invalid(true);

		static project_yellow_globals* g_py_globals = &null_yelo_globals;
		static project_yellow* g_project_yellow = &null_yelo;

		static void InitializeForNewYeloDefinitions(datum_index yelo_index, datum_index yelo_globals_index)
		{
			if (!yelo_index.IsNull())
			{
#if PLATFORM_USES_CACHE_FILES
				g_project_yellow = TagGetForModify<project_yellow>(yelo_index);
#else
				g_project_yellow = blam::tag_get<project_yellow>(yelo_index);
#endif

				if (g_project_yellow->version != project_yellow::k_version)
					g_project_yellow = &null_yelo_invalid;
			}
			else
				g_project_yellow = &null_yelo;

			if (!yelo_globals_index.IsNull())
			{
#if PLATFORM_USES_CACHE_FILES
				g_py_globals = TagGetForModify<project_yellow_globals>(yelo_globals_index);
#else
				g_py_globals = blam::tag_get<project_yellow_globals>(yelo_globals_index);
#endif

				if (g_py_globals->version != project_yellow_globals::k_version)
					g_py_globals = &null_yelo_globals;
			}
			else
				g_py_globals = &null_yelo_globals;
		}

		static void FindYeloDefinitions()
		{
			s_tag_iterator iter;

			// find the yelo scenario tag
			blam::tag_iterator_new(iter, project_yellow::k_group_tag);
			datum_index yelo_index = blam::tag_iterator_next(iter); // there should only be one yelo tag so we only need to call this once

			// find the yelo globals tag
			blam::tag_iterator_new(iter, project_yellow_globals::k_group_tag);
			datum_index yelo_globals_index = blam::tag_iterator_next(iter);

			InitializeForNewYeloDefinitions(yelo_index, yelo_globals_index);
		}
		static bool VerifyYeloScriptDefinitions()
		{
#if PLATFORM_USES_CACHE_FILES
			bool mismatch = false;

			// Verify that the map's project_yellow's script definitions are included in this 
			// build of Yelo.
			const TAG_TBLOCK(& user_script_block, s_scripting_definitions) = g_project_yellow->user_scripting;
			if( user_script_block.Count == 1 &&
				!Scripting::DefinitionsMatch( user_script_block[0] ))
			{
				mismatch = true;
				blam::console_warning("Map's project_yellow's script definitions don't match this build of Yelo!");
			}

			// Verify that the map's project_yellow_globals's script definitions are included in this 
			// build of Yelo.
			const TAG_TBLOCK(& global_script_block, s_scripting_definitions) = g_py_globals->yelo_scripting;
			if( global_script_block.Count == 1 &&
				!Scripting::DefinitionsMatch( global_script_block[0] ))
			{
				mismatch = true;
				blam::console_warning("Map's project_yellow_globals's script definitions don't match this build of Yelo!");
			}

			return !mismatch;
#else
			return true;
#endif
		}

		void ProjectYellowInitialize()
		{

		}
		void ProjectYellowDispose()
		{

		}
		void ProjectYellowInitializeForNewMap()
		{
			FindYeloDefinitions();

#if PLATFORM_USES_CACHE_FILES
			// Just incase someone else comes out with a tool which 
			// sets the bit when they protect the cache, we don't want 
			// to undo their courtesy
			if (!g_project_yellow->IsNull() && !g_project_yellow->IsCacheProtected())
			{
				bool is_protected = TagGroups::Instances()[0].group_tag == Enums::k_protected_group_tag;

				SET_FLAG(g_project_yellow->flags, Flags::_project_yellow_cache_is_protected_bit, is_protected);
			}

			if (!VerifyYeloScriptDefinitions())
			{
				PrepareToDropError(
					"This map's yelo script definitions appear to differ from this build of OS. "
					"We're probably about to crash...");
			}
#endif
		}
		void ProjectYellowDisposeFromOldMap()
		{
			InitializeForNewYeloDefinitions(datum_index::null, datum_index::null);
		}

		const project_yellow_globals* GetYeloGlobals()
		{
			YELO_ASSERT(g_py_globals);

			return g_py_globals;
		}
		const project_yellow* GetYelo()
		{
			YELO_ASSERT(g_project_yellow);

			return g_project_yellow;
		}

#if PLATFORM_IS_EDITOR
		TagGroups::project_yellow_globals* TryGetYeloGlobalsForModification()
		{			
			return g_py_globals != &null_yelo_globals
				? g_py_globals
				: nullptr;
		}
		TagGroups::project_yellow* TryGetYeloForModification()
		{
			return !g_project_yellow->IsNull()
				? g_project_yellow
				: nullptr;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Process a tag reference to a yelo scenario for the current operating mode (editing or cache building).
		/// </summary>
		///
		/// <param name="yelo_reference"> 	Scenario's yelo reference. </param>
		/// <param name="for_build_cache">	True if we're building a cache file, false if we're editing. </param>
		///
		/// <returns>	Returns the loaded yelo scenario's handle or datum_index::null </returns>
		static datum_index YeloPrepareDefinitionsYeloScenario(tag_reference& yelo_reference, const bool for_build_cache)
		{
			datum_index yelo_index = datum_index::null;

			// check if the scenario has a Yelo definition and load it and if not, create it
			bool yelo_isnt_new = false;
			if (yelo_reference.name_length > 0 && yelo_reference.group_tag == project_yellow::k_group_tag)
			{
				yelo_index = blam::tag_load<project_yellow>(yelo_reference.name, 0);
				yelo_isnt_new = true;
			}
			else if (for_build_cache) // Only use the internal tag for cache-building only, not for editing
				yelo_index = blam::tag_new<project_yellow>(project_yellow::k_default_name);

			// Just in case the tag fails to load or fails to be created
			if (!yelo_index.IsNull())
			{
				auto* yelo = blam::tag_get<project_yellow>(yelo_index);

				// set the scenario's yelo reference if it isn't already set-up
				if (!yelo_isnt_new)
					blam::tag_reference_set<project_yellow>(yelo_reference, project_yellow::k_default_name);

				datum_index yelo_globals_index = yelo->LoadYeloGlobals(for_build_cache);

				InitializeForNewYeloDefinitions(yelo_index, yelo_globals_index);
			}

			return yelo_index;
		}
		datum_index YeloPrepareDefinitions(cstring scenario_name, const bool for_build_cache)
		{
			// If we're not building a cache file, just load the scenario into memory without any of its references
			datum_index scenario_index = blam::tag_load<TagGroups::scenario>(scenario_name, for_build_cache
				? FLAG(Flags::_tag_load_from_file_system_bit)
				: FLAG(Flags::_tag_load_non_resolving_references_bit));

			if(!scenario_index.IsNull())
			{
				auto* scenario = blam::tag_get<TagGroups::scenario>(scenario_index);

				datum_index yelo_index = YeloPrepareDefinitionsYeloScenario(
					scenario->GetYeloReferenceHack(), for_build_cache);

				// if we're not building a cache, then this is sapien and we want it to load the scenario and all of 
				// its dependencies after we return the code flow back to it
				if(!for_build_cache)
					blam::tag_unload(scenario_index);

				return yelo_index;
			}

			return datum_index::null;
		}
#endif
	};
};