/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"

#include <blamlib/Halo1/cseries/errors.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/hs/hs_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		cstring project_yellow_globals::k_default_name = "there they are all standing in a row";

#if PLATFORM_IS_EDITOR
		void project_yellow_globals::CullInvalidNetworkPlayerUnits()
		{
			for(int32 x = networking.player_units.Count-1; x >= 0; x--)
			{
				const TagGroups::s_network_game_player_unit& player_unit = networking.player_units[x];
				bool remove_element = true;

				if( player_unit.name[0] == '\0' )
					YELO_WARN("CheApe: Culling unnamed network_game_player_unit element #%n\n", x);
				else if( player_unit.definition.tag_index.IsNull() )
					YELO_WARN("CheApe: Culling invalid network_game_player_unit element #%n (%s)\n", x, player_unit.name);
				else
					remove_element = false;

				if (remove_element)
					blam::tag_block_delete_element(networking.player_units, x);
			}
		}
		/*!
		 * \brief
		 * Removes "preprocess" related and otherwise invalid tag data from the PY globals tag
		 */
		void project_yellow_globals::Cull()
		{
	#if FALSE
			tag_block_delete_all_elements(preprocess);
	#endif

			CullInvalidNetworkPlayerUnits();
		}
#endif

#if PLATFORM_IS_EDITOR
		//////////////////////////////////////////////////////////////////////////
		// project_yellow_globals
#if FALSE
		cstring PLATFORM_API s_yelo_preprocess_maplist_entry::FormatBlockName(datum_index tag_index, tag_block* block, int32 element, char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			auto* elem = TAG_BLOCK_GET_ELEMENT(block, element, s_yelo_preprocess_maplist_entry);

			cstring value = elem->name;
			if( !strcmp(value, "") )
			{
				if( elem->scenario.name_length > 0 && elem->scenario.name != nullptr)
					value = elem->scenario.name;
				else
					return formatted_buffer; // if name is empty and there is no scenario reference, no formatting is done
			}

			strncpy_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, value, Enums::k_tag_string_length);

			return formatted_buffer;
		}
#endif

		bool PLATFORM_API project_yellow_globals::GroupPostprocess(datum_index tag_index, Enums::tag_postprocess_mode mode)
		{
			auto* def = blam::tag_get<project_yellow_globals>(tag_index);

			def->version = project_yellow_globals::k_version;

			Scripting::ScriptingBlockClear(def->yelo_scripting);
			if (mode == Enums::_tag_postprocess_mode_for_runtime)
			{
				Scripting::ScriptingBlockAddDefinitions(def->yelo_scripting, true);
			}

			return true;
		}
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// scripting_block
		cstring PLATFORM_API s_script_construct_definition::FormatBlockName(datum_index tag_index, tag_block* block, int32 element, char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			auto* elem = TAG_BLOCK_GET_ELEMENT(block, element, s_script_construct_definition);

			if (elem->name[0][0] != '\0')
				strncpy_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, elem->name[0], Enums::k_tag_string_length);
			if (elem->name[1][0] != '\0')
				strncat_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, elem->name[1], Enums::k_tag_string_length);

			return formatted_buffer;
		}
		//////////////////////////////////////////////////////////////////////////
#endif
	};
};