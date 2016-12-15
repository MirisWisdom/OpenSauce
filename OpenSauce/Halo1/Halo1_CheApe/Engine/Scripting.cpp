/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/Scripting.hpp"

#include <blamlib/Halo1/hs/hs_structures.hpp>
#include <blamlib/Halo1/hs/object_lists.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>

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

		ENGINE_DPTR(object_list_header_data_t, object_list_header,			PTR_NULL, PTR_NULL, 0xE43758);
		ENGINE_DPTR(list_object_reference_data_t, list_object_reference,	PTR_NULL, PTR_NULL, 0xE43768);

		object_list_header_data_t& ObjectListHeader()						DPTR_IMP_GET_BYREF(object_list_header);
		list_object_reference_data_t& ListObjectReference()					DPTR_IMP_GET_BYREF(list_object_reference);

		static void NullifyScriptFunction(hs_function_definition& function)
		{
			function.parse = CAST_PTR(proc_hs_parse,GET_FUNC_VPTR(HS_MACRO_FUNCTION_PARSE));

			function.evaluate = CAST_PTR(proc_hs_evaluate,GET_FUNC_VPTR(HS_NULL_EVALUATE));
		}

		static void NullifyScriptFunctionWithParams(hs_function_definition& function)
		{
			function.parse = CAST_PTR(proc_hs_parse, GET_FUNC_VPTR(HS_MACRO_FUNCTION_PARSE));

			function.evaluate = CAST_PTR(proc_hs_evaluate, GET_FUNC_VPTR(HS_NULL_WITH_PARAMS_EVALUATE));
		}


		ENGINE_DPTR(hs_function_definition, hs_function_table, 0x9FD950, 0x6EC220, 0xA391A8);
		static hs_function_definition** hs_function_table;

		ENGINE_DPTR(hs_global_definition, hs_external_globals, 0xA621E8, 0x728B00, 0xA933C0);
		static hs_global_definition** hs_external_globals;

#include "Engine/ScriptLibraryMemoryUpgrades.inl"

		void Initialize()
		{
			hs_function_table = GET_DPTR2(hs_function_table);
			hs_external_globals = GET_DPTR2(hs_external_globals);

			// We enable tag definition upgrades by default on all platforms...
			MemoryUpdatesInitializeTagDefinitions();
			// but custom scripting must be explicitly setup in Tool builds
#if PLATFORM_TYPE != PLATFORM_TOOL
			Scripting::InitializeCustomScriptingDefinitions();
#endif
		}

		void Dispose()
		{
		}

		void InitializeCustomScriptingDefinitions()
		{
			CheApe::s_cache_header& header = CheApe::GlobalCacheHeader();

			if(	header.Scripting.Functions.Count > 0 || 
				header.Scripting.Globals.Count > 0)
			{
				MemoryUpgradesCustomScriptingDefinitions();
			}
		}

		void InitializeScriptNodeUpgrades()	{ MemoryUpgradesSyntaxInitialize(true); }
		void DisposeScriptNodeUpgrades()	{ MemoryUpgradesSyntaxInitialize(false); }

		hs_function_definition* GetFunctionDefintion(int16 function_index)
		{
			if(function_index > NONE && function_index < _upgrade_globals.functions.count)
				return _upgrade_globals.functions.table[function_index];

			return nullptr;
		}

		static void ScriptingNameCopyToTagNameData(cstring scripting_name, tag_string tag_name_data[2])
		{
			size_t length = strlen(scripting_name);
			memset(tag_name_data, 0, sizeof(tag_name_data[0])*2);

			char local[Enums::k_tag_string_length*2];
			if(length > Enums::k_tag_string_length*2)
			{
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
			TAG_TBLOCK(& functions, TagGroups::s_script_function_definition), bool only_internals)
		{
			CRT_DEBUG_FILL_OFF(); // so the CRT doesn't fill our name buffer with 0xFD
			using namespace TagGroups;

			hs_function_definition* def;
			for(int32 x = _upgrade_globals.functions.yelo_start_index;
				x < _upgrade_globals.functions.count; x++)
			{
				def = _upgrade_globals.functions.table[x];

				if( only_internals != TEST_FLAG(def->yelo_flags, Flags::_hs_yelo_definition_internal_bit) )
					continue;

				s_script_function_definition* element = functions.add_and_get_element();

				ScriptingNameCopyToTagNameData(def->name, element->name);
				element->index = CAST(int16, x);
				element->return_type = def->return_type;

				if( !element->parameters.resize(def->paramc) )
				{
					YELO_ERROR_CRITICAL("CheApe: failed to add function parameters for %s", 
						def->name);
				}

				memcpy(element->parameters.Definitions,
					def->params, sizeof(_enum) * def->paramc);
			}
		}

		static void ScriptingBlockAddGlobalDefinitions(
			TAG_TBLOCK(& globals, TagGroups::s_script_global_definition), bool only_internals)
		{
			CRT_DEBUG_FILL_OFF(); // so the CRT doesn't fill our name buffer with 0xFD
			using namespace TagGroups;

			hs_global_definition* def;
			for(int32 x = _upgrade_globals.globals.yelo_start_index;
				x < _upgrade_globals.globals.count; x++)
			{
				def = _upgrade_globals.globals.table[x];

				if( only_internals != TEST_FLAG(def->flags, Flags::_hs_yelo_definition_internal_bit) )
					continue;

				s_script_global_definition* element = globals.add_and_get_element();

				ScriptingNameCopyToTagNameData(def->name, element->name);
				element->index = CAST(int16, x)+1; // NOTE: for globals, we do +1 since in game builds there is a global which isn't present in editor builds
				element->type = def->type;
			}
		}

		void ScriptingBlockClear(
			TAG_TBLOCK(& script_block, TagGroups::s_scripting_definitions))
		{
			blam::tag_block_resize(script_block, 0);
		}
		void ScriptingBlockAddDefinitions(
			TAG_TBLOCK(& script_block, TagGroups::s_scripting_definitions), bool only_internals)
		{
			using namespace TagGroups;

			ScriptingBlockClear(script_block);

			s_scripting_definitions* element = script_block.add_and_get_element();

			ScriptingBlockAddFunctionDefinitions(element->new_functions, only_internals);
			ScriptingBlockAddGlobalDefinitions(element->new_globals, only_internals);
		}
	};
};