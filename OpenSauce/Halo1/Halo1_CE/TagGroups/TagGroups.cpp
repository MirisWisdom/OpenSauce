/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/TagGroups.hpp"

#include "Memory/MemoryInterface.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Game/EngineFunctions.hpp"

#include <Blam/Halo1/shader_postprocess_definitions.hpp>
#include "TagGroups/CacheFiles.hpp"
#include "TagGroups/project_yellow_definitions.hpp"

//////////////////////////////////////////////////////////////////////////
// 
#include <TagGroups/Halo1/object_definitions.hpp>
#include <TagGroups/Halo1/device_definitions.hpp>
#include <TagGroups/Halo1/item_definitions.hpp>
#include <TagGroups/Halo1/unit_definitions.hpp>

#include <TagGroups/Halo1/bitmap_definitions.hpp>
#include <TagGroups/Halo1/damage_effect_definitions.hpp>
#include <TagGroups/Halo1/game_globals_definitions.hpp>
#include <TagGroups/Halo1/global_definitions.hpp>
#include <TagGroups/Halo1/model_definitions.hpp>
#include <TagGroups/Halo1/scenario_definitions.hpp>
#include <TagGroups/Halo1/shader_definitions.hpp>
#include <TagGroups/Halo1/sound_definitions.hpp>
#include <TagGroups/Halo1/structure_bsp_definitions.hpp>
//#include <TagGroups/Halo1>
//////////////////////////////////////////////////////////////////////////

namespace Yelo
{
	namespace TagGroups
	{
		static const project_yellow_globals null_yelo_globals = {project_yellow_globals::k_version};
		const project_yellow_globals* _global_yelo_globals = &null_yelo_globals; // extern defined in project_yellow_definitions.hpp

		static const project_yellow null_yelo =			{project_yellow::k_version, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0} };
		static const project_yellow null_yelo_invalid =	{project_yellow::k_version, {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0} };
		const project_yellow* _global_yelo = &null_yelo; // extern defined in project_yellow_definitions.hpp

		//////////////////////////////////////////////////////////////////////////

#define __EL_INCLUDE_ID			__EL_INCLUDE_TAGGROUPS
#define __EL_INCLUDE_FILE_ID	__EL_TAGGROUPS_TAG_GROUPS
#include "Memory/_EngineLayout.inl"

		s_cache_tag_header* Index()					{ return Cache::CacheFileGlobals()->current_cache.tag_header; }
		s_cache_tag_instance const* Instances()		DPTR_IMP_GET(tag_instances);


		void Initialize()	{}

		void Dispose()		{}

		static void FindCacheYeloDefinitions()
		{
			// Reset first, in case map has no definitions
			_global_yelo = &null_yelo;
			_global_yelo_globals = &null_yelo_globals;

			tag_iterator iter;

			// find the yelo scenario tag
			tag_iterator_new(iter, project_yellow::k_group_tag);
			datum_index tag_index = tag_iterator_next(iter); // there should only be one yelo tag so we only need to call this once
			if(!tag_index.IsNull())
			{
				_global_yelo = Instances()[tag_index.index].Definition<project_yellow>();

				if(_global_yelo->version != project_yellow::k_version)
					_global_yelo = &null_yelo_invalid;
			}

			// find the yelo globals tag
			tag_iterator_new(iter, project_yellow_globals::k_group_tag);
			tag_index = tag_iterator_next(iter);
			if(!tag_index.IsNull())
			{
				_global_yelo_globals = Instances()[tag_index.index].Definition<project_yellow_globals>();

				if(_global_yelo_globals->version != project_yellow_globals::k_version)
					_global_yelo_globals = &null_yelo_globals;
			}
		}
		static bool VerifyYeloScriptDefinitions()
		{
			bool mismatch = false;

			// Verify that the map's project_yellow's script definitions are included in this 
			// build of Yelo.
			const TAG_TBLOCK(& user_script_block, scripting_block) = _global_yelo->user_scripting;
			if( user_script_block.Count == 1 &&
				!Scripting::DefinitionsMatch( user_script_block[0] ))
			{
				mismatch = true;
				Engine::Console::Warning("Map's project_yellow's script definitions don't match this build of Yelo!");
			}

			// Verify that the map's project_yellow_globals's script definitions are included in this 
			// build of Yelo.
			const TAG_TBLOCK(& global_script_block, scripting_block) = _global_yelo_globals->yelo_scripting;
			if( global_script_block.Count == 1 &&
				!Scripting::DefinitionsMatch( global_script_block[0] ))
			{
				mismatch = true;
				Engine::Console::Warning("Map's project_yellow_globals's script definitions don't match this build of Yelo!");
			}

			return !mismatch;
		}
		void InitializeForNewMap()
		{
			bool is_protected = Instances()[0].group_tag == Enums::k_protected_group_tag;

			FindCacheYeloDefinitions();

			// Just incase someone else comes out with a tool which 
			// sets the bit when they protect the cache, we don't want 
			// to undo their courtesy
			if(!_global_yelo->IsNull() && !_global_yelo->IsCacheProtected())
				// Hacks? In MY Yelo? Ya rly
				CAST_QUAL(project_yellow*, _global_yelo)->flags.cache_is_protected_bit = 
					is_protected;


			if(!VerifyYeloScriptDefinitions())
			{
				PrepareToDropError("This map's yelo script definitions appear to differ from this build of OS. We're probably about to crash...");
			}
		}

		datum_index tag_iterator_next(tag_iterator& iter)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(TAG_ITERATOR_NEXT);

			__asm {
				push	esi

				mov		esi, iter
				call	TEMP_CALL_ADDR

				pop		esi
			}
		}
	};
};