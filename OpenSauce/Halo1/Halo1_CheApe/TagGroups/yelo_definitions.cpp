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
#include "TagGroups/yelo_definitions.hpp"

#include <Blam/Halo1/project_yellow_shared_definitions.hpp>
#include <TagGroups/Halo1/scenario_definitions.hpp>

#include "Engine/EngineFunctions.hpp"
#include "Engine/Scripting.hpp"
#include "TagGroups/tag_database_definitions.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		//////////////////////////////////////////////////////////////////////////
		// scripting_block
		static cstring PLATFORM_API scripting_block_construct_format(datum_index tag_index, tag_block* block, int32 element, char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			script_construct_block* elem = 
				CAST_PTR(script_construct_block*, tag_block_get_element(block, element));

			if(elem->name[0][0] != '\0')
				strncpy_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, elem->name[0], Enums::k_tag_string_length);
			if(elem->name[1][0] != '\0')
				strncat_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, elem->name[1], Enums::k_tag_string_length);

			return formatted_buffer;
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// project_yellow_globals
		static bool PLATFORM_API py_globals_group_postprocess(Yelo::datum_index tag_index, bool verify_data)
		{
			project_yellow_globals* def = Yelo::tag_get<project_yellow_globals>(tag_index);

			def->version = project_yellow_globals::k_version;

			Scripting::ScriptingBlockAddDefinitions(def->yelo_scripting, true);

			return true;
		}

		static cstring PLATFORM_API py_globals_preprocess_maplist_format(datum_index tag_index, tag_block* block, int32 element, char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			project_yellow_globals::preprocess_maplist_block* elem = 
				CAST_PTR(project_yellow_globals::preprocess_maplist_block*, tag_block_get_element(block, element));

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

		static bool PLATFORM_API py_globals_gtp_block_postprocess(void* block, byte_flags flags)
		{
			project_yellow_globals::gameplay_game_type_player* elem = 
				CAST_PTR(project_yellow_globals::gameplay_game_type_player*,block);

			if(elem->settings.Count == 0)
				Yelo::tag_block_add_element(elem->settings.to_tag_block());

			return true;
		}

		static cstring PLATFORM_API py_globals_gtps_block_format(datum_index tag_index, tag_block* block, int32 element, char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			project_yellow_globals::gameplay_game_type_player_settings* elem = 
				CAST_PTR(project_yellow_globals::gameplay_game_type_player_settings*,tag_block_get_element(block, element));

			if(element == 0)
				strcpy_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, "global player");
			else
				sprintf_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, "player %d", element);

			return formatted_buffer;
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// project_yellow
		bool PLATFORM_API py_group_postprocess(datum_index tag_index, bool verify_data)
		{
			project_yellow* def = Yelo::tag_get<project_yellow>(tag_index);

			def->version = project_yellow::k_version;

			if(verify_data)
			{
				if(!def->physics.IsGravityScaleValid())		def->physics.ResetGravityScale();
				if(!def->physics.IsPlayerSpeedScaleValid())	def->physics.ResetPlayerSpeedScale();
			}

			Scripting::ScriptingBlockAddDefinitions(def->user_scripting, false);

			return true;
		}
		//////////////////////////////////////////////////////////////////////////

		s_yelo_definition_globals _yelo_definition_globals = {
			false,
		};

		void YeloDefinitionsInitialize()
		{
			Yelo::tag_group_definition* py_globals_definition = Yelo::tag_group_get<project_yellow_globals>();
			Yelo::tag_group_definition* py_definition = Yelo::tag_group_get<project_yellow>();

			if(!py_globals_definition || !py_definition)
			{
				YELO_ERROR(_error_message_priority_none, 
					"CheApe: Yelo not found!");
			}
			else if(py_globals_definition->version == project_yellow_globals::k_version &&
				py_definition->version == TagGroups::project_yellow::k_version)
			{
				int32 field_index;

				//////////////////////////////////////////////////////////////////////////
				{// project_yellow_globals
					py_globals_definition->postprocess_proc = &TagGroups::py_globals_group_postprocess;

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

					//////////////////////////////////////////////////////////////////////////
					{// gameplay_game_type_player
						field_index = TagGroups::tag_block_find_field(py_globals_definition->definition, Enums::_field_block, "game type players");
						if(field_index == NONE)
						{
							YELO_ERROR(_error_message_priority_assert, 
								"CheApe: gameplay_game_type_player not found!");
						}

						tag_block_definition* gtp_block_def = py_globals_definition->definition->fields[field_index].Definition<tag_block_definition>();
						gtp_block_def->postprocess_proc = &TagGroups::py_globals_gtp_block_postprocess;

						field_index = TagGroups::tag_block_find_field(gtp_block_def, Enums::_field_block, "settings");
						if(field_index == NONE)
						{
							YELO_ERROR(_error_message_priority_assert, 
								"CheApe: gameplay_game_type_player_settings not found!");
						}
						tag_block_definition* gtps_block_def = gtp_block_def->fields[field_index].Definition<tag_block_definition>();
						gtps_block_def->format_proc = &TagGroups::py_globals_gtps_block_format;
					}

					//////////////////////////////////////////////////////////////////////////
					{// scripting_block
					 // NOTE: this will also affect project_yellow's script block as it's the same definition
						field_index = TagGroups::tag_block_find_field(py_globals_definition->definition, Enums::_field_block, "yelo scripting");
						if(field_index == NONE)
						{
							YELO_ERROR(_error_message_priority_assert, 
								"CheApe: scripting_block not found!");
						}

						tag_block_definition* scripting_block_def = py_globals_definition->definition->fields[field_index].Definition<tag_block_definition>();

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

					tag_group_definition* scnr = Yelo::tag_group_get(TagGroups::scenario::k_group_tag);
					tag_field* field = &scnr->definition->fields[0];
					field->name = "project yellow definitions";
					field->definition = &reference_definition;
				}
				//////////////////////////////////////////////////////////////////////////

				_yelo_definition_globals.initialized = true;
			}
			else
			{
				YELO_ERROR(_error_message_priority_none, 
					"CheApe: Yelo definitions mismatch!");
			}

			//////////////////////////////////////////////////////////////////////////
			// Now initialize the complex subsystems
			s_tag_database::Initialize();
		}

		void YeloDefinitionsDispose()
		{
			s_tag_database::Dispose();

			if(_yelo_definition_globals.initialized)
			{
			}

			_yelo_definition_globals.initialized = false;
		}
		//////////////////////////////////////////////////////////////////////////
	};
};