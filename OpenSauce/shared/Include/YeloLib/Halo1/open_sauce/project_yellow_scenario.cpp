/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/cache/cache_files_structures.hpp>
#include <blamlib/Halo1/main/console.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include <YeloLib/Halo1/hs/hs_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_cv_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

namespace Yelo
{
	namespace Scenario
	{
		using namespace TagGroups;

		static project_yellow_globals null_yelo_globals = { project_yellow_globals::k_version };

		static project_yellow null_yelo =			{project_yellow::k_version, 
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0}
		};
		static project_yellow null_yelo_invalid =	{project_yellow::k_version, 
			{0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0}
		};

		static project_yellow_globals* g_py_globals;
		static project_yellow* g_project_yellow;

		static void FindYeloDefinitions()
		{
			// Reset first, in case scenario has no definitions
			g_project_yellow = &null_yelo;
			g_py_globals = &null_yelo_globals;

			s_tag_iterator iter;

			// find the yelo scenario tag
			blam::tag_iterator_new(iter, project_yellow::k_group_tag);
			datum_index tag_index = blam::tag_iterator_next(iter); // there should only be one yelo tag so we only need to call this once
			if (!tag_index.IsNull())
			{
#if PLATFORM_USES_CACHE_FILES
				g_project_yellow = TagGetUnsafe<project_yellow>(tag_index);
#else
				g_project_yellow = blam::tag_get<project_yellow>(tag_index);
#endif

				if (g_project_yellow->version != project_yellow::k_version)
					g_project_yellow = &null_yelo_invalid;
			}

			// find the yelo globals tag
			blam::tag_iterator_new(iter, project_yellow_globals::k_group_tag);
			tag_index = blam::tag_iterator_next(iter);
			if (!tag_index.IsNull())
			{
#if PLATFORM_USES_CACHE_FILES
				g_py_globals = TagGetUnsafe<project_yellow_globals>(tag_index);
#else
				g_py_globals = blam::tag_get<project_yellow_globals>(tag_index);
#endif

				if (g_py_globals->version != project_yellow_globals::k_version)
					g_py_globals = &null_yelo_globals;
			}
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

				g_project_yellow->flags.cache_is_protected_bit = is_protected;
			}

			if (!VerifyYeloScriptDefinitions())
			{
				PrepareToDropError(
					"This map's yelo script definitions appear to differ from this build of OS. "
					"We're probably about to crash...");
			}
#endif
		}

		const project_yellow_globals_cv* GetYeloCvGlobals()
		{
			auto* py_globals = GetYeloGlobals();

			if (py_globals->HasCvGlobals())
			{
				return blam::tag_get<project_yellow_globals_cv>(py_globals->cv_globals.tag_index);
			}

			return nullptr;
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
	};
};