/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/yelo_definitions.hpp"

#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>
#include <YeloLib/Halo1/tag_files/tag_database_definitions.hpp>

#include "Engine/EngineFunctions.hpp"
#include "Engine/GrenadeTypesUpgrade.hpp"
#include "Engine/Scripting.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		//////////////////////////////////////////////////////////////////////////
		// scripting_block
		static cstring PLATFORM_API scripting_block_construct_format(datum_index tag_index, tag_block* block, int32 element, char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			s_script_construct_definition* elem = 
				CAST_PTR(s_script_construct_definition*, tag_block_get_element(block, element));

			if(elem->name[0][0] != '\0')
				strncpy_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, elem->name[0], Enums::k_tag_string_length);
			if(elem->name[1][0] != '\0')
				strncat_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, elem->name[1], Enums::k_tag_string_length);

			return formatted_buffer;
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// project_yellow_globals
		static bool PLATFORM_API py_globals_group_postprocess(Yelo::datum_index tag_index, bool for_runtime)
		{
			project_yellow_globals* def = Yelo::tag_get<project_yellow_globals>(tag_index);

			def->version = project_yellow_globals::k_version;

			Scripting::ScriptingBlockClear(def->yelo_scripting);
			if(for_runtime)
			{
				Scripting::ScriptingBlockAddDefinitions(def->yelo_scripting, true);
			}

			return true;
		}

#if FALSE
		static cstring PLATFORM_API py_globals_preprocess_maplist_format(datum_index tag_index, tag_block* block, int32 element, char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			s_yelo_preprocess_maplist_entry* elem = 
				CAST_PTR(s_yelo_preprocess_maplist_entry*, tag_block_get_element(block, element));

			cstring value = elem->name;
			if( !strcmp(value, "") )
			{
				if( elem->scenario.name_length > 0 && elem->scenario.name != NULL)
					value = elem->scenario.name;
				else
					return formatted_buffer; // if name is empty and there is no scenario reference, no formatting is done
			}

			strncpy_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, value, Enums::k_tag_string_length);

			return formatted_buffer;
		}
#endif
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// project_yellow
		static bool PLATFORM_API py_group_postprocess(datum_index tag_index, bool for_runtime)
		{
			project_yellow* def = Yelo::tag_get<project_yellow>(tag_index);

			def->version = project_yellow::k_version;

			Scripting::ScriptingBlockClear(def->user_scripting);
			if(for_runtime)
			{
				if(!def->physics.IsGravityScaleValid() || def->physics.gravity_scale == 0.0f)			def->physics.ResetGravityScale();
				if(!def->physics.IsPlayerSpeedScaleValid() || def->physics.player_speed_scale == 0.0f)	def->physics.ResetPlayerSpeedScale();

				Scripting::ScriptingBlockAddDefinitions(def->user_scripting, false);
			}

			return true;
		}
		//////////////////////////////////////////////////////////////////////////

		s_yelo_definition_globals _yelo_definition_globals = {
			false,
		};

		static void YeloDefinitionsInitialize(tag_group* py_globals_definition, tag_group* py_definition)
		{
			int32 field_index = NONE;

			//////////////////////////////////////////////////////////////////////////
			{// project_yellow_globals
				py_globals_definition->postprocess_proc = &TagGroups::py_globals_group_postprocess;

#if FALSE
				//////////////////////////////////////////////////////////////////////////
				{// preprocess_block
					field_index = TagGroups::tag_block_find_field(py_globals_definition->definition, Enums::_field_block, "preprocess");
					if(field_index == NONE)
					{
						YELO_ERROR(_error_message_priority_assert, 
							"CheApe: preprocess_block not found!");
					}

					tag_block_definition* preprocess_block_def = py_globals_definition->definition->fields[field_index].Definition<tag_block_definition>();
					field_index = TagGroups::tag_block_find_field(preprocess_block_def, Enums::_field_block, "campaign");
					if(field_index == NONE)
					{
						YELO_ERROR(_error_message_priority_assert, 
							"CheApe: preprocess_maplist_block not found!");
					}

					tag_block_definition* preprocess_map_block_def = preprocess_block_def->fields[field_index].Definition<tag_block_definition>();
					preprocess_map_block_def->format_proc = &TagGroups::py_globals_preprocess_maplist_format;
				}
#endif


				//////////////////////////////////////////////////////////////////////////
				{// scripting_block
					// NOTE: this will also affect project_yellow's script block as it's the same definition
					field_index = TagGroups::tag_block_find_field(py_globals_definition->header_block_definition, Enums::_field_block, "yelo scripting");
					if(field_index == NONE)
					{
						YELO_ERROR(_error_message_priority_assert, 
							"CheApe: scripting_block not found!");
					}

					tag_block_definition* scripting_block_def = py_globals_definition->header_block_definition->fields[field_index].Definition<tag_block_definition>();

					field_index = TagGroups::tag_block_find_field(scripting_block_def, Enums::_field_block, "new functions");
					if(field_index == NONE)
					{
						YELO_ERROR(_error_message_priority_assert, 
							"CheApe: script_function_block not found!");
					}
					tag_block_definition* script_function_block_def = scripting_block_def->fields[field_index].Definition<tag_block_definition>();
					script_function_block_def->format_proc = &TagGroups::scripting_block_construct_format;

					field_index = TagGroups::tag_block_find_field(scripting_block_def, Enums::_field_block, "new globals");
					if(field_index == NONE)
					{
						YELO_ERROR(_error_message_priority_assert, 
							"CheApe: script_global_block not found!");
					}
					tag_block_definition* script_global_block_def = scripting_block_def->fields[field_index].Definition<tag_block_definition>();
					script_global_block_def->format_proc = &TagGroups::scripting_block_construct_format;
				}
			}
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			{// project_yellow
				py_definition->postprocess_proc = &TagGroups::py_group_postprocess;
			}
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			{// make the first tag_reference field (which is unused) in the scenario to reference the yelo tag
				static tag_reference_definition reference_definition = {
					0,
					project_yellow::k_group_tag,
					NULL
				};

				tag_group* scnr = Yelo::tag_group_get(TagGroups::scenario::k_group_tag);
				tag_field* field = &scnr->header_block_definition->fields[0];
				field->name = "project yellow definitions";
				field->definition = &reference_definition;
			}
			//////////////////////////////////////////////////////////////////////////

			_yelo_definition_globals.initialized = true;
		}

		static bool YeloDefinitionsValidateGlobals(tag_group* py_globals_definition)
		{
			return py_globals_definition->version == project_yellow_globals::k_version && 
				py_globals_definition->header_block_definition->element_size == sizeof(project_yellow_globals);
		}
		static bool YeloDefinitionsValidateScenario(tag_group* py_definition)
		{
			return py_definition->version == project_yellow::k_version && 
				py_definition->header_block_definition->element_size == sizeof(project_yellow);
		}
		static bool YeloDefinitionsValidate(tag_group* py_globals_definition, tag_group* py_definition)
		{
			return YeloDefinitionsValidateGlobals(py_globals_definition) && YeloDefinitionsValidateScenario(py_definition);
		}

		void YeloDefinitionsInitialize()
		{
			Yelo::tag_group* py_globals_definition = Yelo::tag_group_get<project_yellow_globals>();
			Yelo::tag_group* py_definition = Yelo::tag_group_get<project_yellow>();

			if(!py_globals_definition || !py_definition)
			{
				YELO_ERROR(_error_message_priority_none, 
					"CheApe: Yelo not found!");
			}
			else if(YeloDefinitionsValidate(py_globals_definition, py_definition))
				YeloDefinitionsInitialize(py_globals_definition, py_definition);
			else
			{
				YELO_ERROR(_error_message_priority_none, 
					"CheApe: Yelo definitions mismatch!");
			}

			//////////////////////////////////////////////////////////////////////////
			// Now initialize the complex subsystems
			s_tag_database::Initialize();
			Objects::Items::GrenadeTypesUpgradeInitializeTagDefinitions();
		}

		void YeloDefinitionsDispose()
		{
			Objects::Items::GrenadeTypesUpgradeDisposeTagDefinitions();
			s_tag_database::Dispose();

			if(_yelo_definition_globals.initialized)
			{
			}

			_yelo_definition_globals.initialized = false;
		}
		//////////////////////////////////////////////////////////////////////////

#if PLATFORM_ID == PLATFORM_TOOL
		static bool GameGlobalsRequiresYeloGameStateUpgrades(datum_index game_globals_index)
		{
			TagGroups::s_game_globals* game_globals = tag_get<TagGroups::s_game_globals>(game_globals_index);

			return false;
		}

		bool YeloToolCheckTagsForGameStateUpgradeRequirements()
		{
			bool result = false;

	// no tags currently need to be checked
	#if FALSE
			TagGroups::tag_iterator iter;
			tag_iterator_new(iter);

			datum_index tag_index;
			while( !(tag_index = tag_iterator_next(iter)).IsNull() )
			{
				switch((*TagGroups::TagInstances())[tag_index.index].group_tag)
				{
				case TagGroups::s_game_globals::k_group_tag:
					result |= GameGlobalsRequiresYeloGameStateUpgrades(tag_index); 
				}
			}
	#endif

			return result;
		}
#endif
	};
};