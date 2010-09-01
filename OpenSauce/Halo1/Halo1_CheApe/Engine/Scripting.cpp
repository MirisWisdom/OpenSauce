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
#include "Engine/Scripting.hpp"

#include <Blam/Halo1/BlamScriptingDefinitions.hpp>
#include <Blam/Halo1/project_yellow_shared_definitions.hpp>
#include <Memory/Halo1/DataShared.hpp>
#include "Blam/Halo1/BlamMemoryUpgrades.hpp"
#include "CheApeCore.hpp"
#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace Scripting
	{
		FUNC_PTR(HS_MACRO_FUNCTION_PARSE,		0x5A89B0, 0x5396A0, 0x67C9E0);
		FUNC_PTR(HS_NULL_EVALUATE,				0x4ED815, 0x4BE2B5, 0x57F9D5); // actually part of playback's evaluate
		FUNC_PTR(HS_NULL_WITH_PARAMS_EVALUATE,	0x4F04F0, 0x4BF510, 0x5831A0);

		FUNC_PTR(HS_ARGUMENTS_EVALUATE,			0x4FC0B0, 0x508610, 0x622E70);
		FUNC_PTR(HS_RETURN,						0x4FBAE0, 0x5080A0, 0x6228A0);

		static void NullifyScriptFunction(hs_function_definition& function)
		{
			function.parse = CAST_PTR(hs_parse_proc,GET_FUNC_VPTR(HS_MACRO_FUNCTION_PARSE));

			function.evaluate = CAST_PTR(hs_evaluate_proc,GET_FUNC_VPTR(HS_NULL_EVALUATE));
		}

		static void NullifyScriptFunctionWithParams(hs_function_definition& function)
		{
			function.parse = CAST_PTR(hs_parse_proc,GET_FUNC_VPTR(HS_MACRO_FUNCTION_PARSE));

			function.evaluate = CAST_PTR(hs_evaluate_proc,GET_FUNC_VPTR(HS_NULL_WITH_PARAMS_EVALUATE));
		}


		ENGINE_DPTR(hs_function_definition, hs_function_table, 0x9FD950, 0x6EC220, 0xA391A8);
		static hs_function_definition** hs_function_table = GET_DPTR2(hs_function_table);

		ENGINE_DPTR(hs_global_definition, hs_external_globals, 0xA621E8, 0x728B00, 0xA933C0);
		static hs_global_definition** hs_external_globals = GET_DPTR2(hs_external_globals);

#include "Engine/ScriptLibraryMemoryUpgrades.inl"

		void Initialize()
		{
			CheApe::s_cache_header& header = CheApe::GlobalCacheHeader();

			if(	header.Scripting.Functions.Count > 0 || 
				header.Scripting.Globals.Count > 0)
			{
				MemoryUpgradesInitialize();

				hs_global_definition** globals = _upgrade_globals.globals.table;

				for(int32 x = 0; x < _upgrade_globals.globals.count; x++)
					if( !strcmp(globals[x]->name, "") && globals[x]->address != NULL )
						globals[x]->Value._bool = false;
			}
		}

		void Dispose()
		{
		}

		static void ScriptingNameCopyToTagNameData(cstring scripting_name, tag_string tag_name_data[2])
		{
			size_t length = strlen(scripting_name);
			memset(tag_name_data[0], 0, sizeof(tag_name_data[0])*2);

			if(length > Enums::k_tag_string_length*2)
			{
				char local[Enums::k_tag_string_length*2];
				memcpy(local, scripting_name, sizeof(local));

				scripting_name = local;
			}

			strncpy_s(	tag_name_data[0], 
						scripting_name, Enums::k_tag_string_length);

			// if the scripting name is longer than 31 characters, copy the other characters too
			if(length > Enums::k_tag_string_length)
				strncpy_s(	tag_name_data[1], 
							scripting_name+(Enums::k_tag_string_length+1),
							Enums::k_tag_string_length);
		}

		static void ScriptingBlockAddFunctionDefinitions(
			TAG_TBLOCK(& functions, TagGroups::script_function_block), bool only_internals)
		{
			size_t old_threshold = _CrtSetDebugFillThreshold( 0 ); // so the CRT doesn't fill our name buffer with 0xFD
			using namespace TagGroups;

			hs_function_definition* def;
			for(int32 x = _upgrade_globals.functions.yelo_start_index;
				x < _upgrade_globals.functions.count; x++)
			{
				def = _upgrade_globals.functions.table[x];

				if( TEST_BIT(def->flags, CAST(uint16, only_internals)) )
				{
					script_function_block* element = tag_block_get_element(functions, tag_block_add_element(functions));

					ScriptingNameCopyToTagNameData(def->name, element->name);
					element->index = CAST(int16, x);
					element->return_type = def->return_type;

					if( !tag_block_resize(element->parameters.to_tag_block(), def->paramc) )
					{
						YELO_ERROR(_error_message_priority_critical, 
							"CheApe: failed to add function parameters for %s", def->name);
					}

					memcpy(element->parameters.Definitions,
						def->params, sizeof(_enum) * def->paramc);
				}
			}

			_CrtSetDebugFillThreshold( old_threshold );
		}

		static void ScriptingBlockAddGlobalDefinitions(
			TAG_TBLOCK(& globals, TagGroups::script_global_block), bool only_internals)
		{
			size_t old_threshold = _CrtSetDebugFillThreshold( 0 ); // so the CRT doesn't fill our name buffer with 0xFD
			using namespace TagGroups;

			hs_global_definition* def;
			for(int32 x = _upgrade_globals.globals.yelo_start_index;
				x < _upgrade_globals.globals.count; x++)
			{
				def = _upgrade_globals.globals.table[x];

				if( TEST_BIT(def->flags, CAST(uint16, only_internals)) )
				{
					script_global_block* element = tag_block_get_element(globals, tag_block_add_element(globals));

					ScriptingNameCopyToTagNameData(def->name, element->name);
					element->index = CAST(int16, x)+1; // NOTE: for globals, we do +1 since in game builds there is a global which isn't present in editor builds
					element->type = def->type;
				}
			}

			_CrtSetDebugFillThreshold( old_threshold );
		}

		void ScriptingBlockAddDefinitions(
			TAG_TBLOCK(& script_block, TagGroups::scripting_block), bool only_internals)
		{
			using namespace TagGroups;

			tag_block_resize(script_block, 0);

			scripting_block* element = tag_block_get_element(script_block, tag_block_add_element(script_block));

			ScriptingBlockAddFunctionDefinitions(element->new_functions, only_internals);
			ScriptingBlockAddGlobalDefinitions(element->new_globals, only_internals);
		}
	};
};