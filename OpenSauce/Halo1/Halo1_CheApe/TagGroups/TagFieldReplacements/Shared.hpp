/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>

#include <blamlib/Halo1/camera/camera_definitions.hpp>
#include <blamlib/Halo1/effects/effect_definitions.hpp>
#include <blamlib/Halo1/effects/damage_effect_definitions.hpp>
#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
#include <blamlib/Halo1/scenario/sky_definitions.hpp>

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace Shared
	{
		static cstring k_reserved_flag_string = "reserved"
			TAG_FIELD_MARKUP_IS_READONLY;

		static cstring k_never_share_resources_flag_string = "never share resources"
			TAG_FIELD_MARKUP_PREFIX_HELP
			"";

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a tag field using a delimited block.field path. </summary>
		///
		/// <param name="source_definition">	[in] The tag definition to search in. </param>
		/// <param name="field_type">			Type of the field to get. </param>
		/// <param name="field_path">			Full path of the field. </param>
		///
		/// <returns>	The found tag field. </returns>
		static tag_field& GetTagField(tag_block_definition& source_definition, Enums::field_type field_type, const std::string& field_path)
		{
			// Split the field path into block names
			// The last name is the field name so is not added to the list
			std::vector<std::string> block_names;
			std::string field_name(field_path);

			int seperator_index = 0;
			while(field_name.length() != 0)
			{
				int seperator_index = field_name.find(":");
				if(seperator_index == std::string::npos)
				{
					break;
				}

				block_names.push_back(field_name.substr(0, seperator_index));
				field_name.replace(0, seperator_index + 1, "");
			}

			// Navigate through the block definitions
			auto* current_block = &source_definition;
			for(auto& block_field_name : block_names)
			{
				int found_block_field = current_block->FindFieldIndex(Enums::_field_block, block_field_name.c_str());
				assert(found_block_field != NONE);

				current_block = current_block->fields[found_block_field].Definition<tag_block_definition>();
			}

			// Get the targeted field
			int found_field_index = current_block->FindFieldIndex(field_type, field_name.c_str());
			assert(found_field_index != NONE);

			return current_block->fields[found_field_index];
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a flag fields string list </summary>
		///
		/// <tparam name="kFieldType">	Type of the flags field. </tparam>
		/// <param name="definition">	   	[in] The definition to find the flags in. </param>
		/// <param name="flags_field_name">	(Optional) The path to the flags field. </param>
		///
		/// <returns>	The flag fields string list. </returns>
		template<Enums::field_type kFieldType>
		static string_list* FindFlagsField(tag_block_definition* definition, cstring flags_field_name = "flags")
		{
			static_assert(kFieldType == Enums::_field_byte_flags || kFieldType == Enums::_field_word_flags || kFieldType == Enums::_field_long_flags,
				"Expected a flags-based field type");

			auto& flags_field = GetTagField(*definition, kFieldType, flags_field_name);
			return flags_field.Definition<string_list>();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Populate a list of strings with the reserved string. </summary>
		///
		/// <tparam name="kStartIndex">  	The start index. </tparam>
		/// <tparam name="kEndIndex">	 	The end index. </tparam>
		/// <tparam name="kStringsCount">	The strings count. </tparam>
		/// <param name="strings)">	[in] The strings array to populate </param>
		template<size_t kStartIndex, size_t kEndIndex, rsize_t kStringsCount>
		static void PopulateWithReservedString(cstring (& strings)[kStringsCount])
		{
			BOOST_STATIC_ASSERT(kStartIndex < kStringsCount);
			BOOST_STATIC_ASSERT(kEndIndex < kStringsCount);

			for (size_t x = kStartIndex; x <= kEndIndex; x++)
				strings[x] = k_reserved_flag_string;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Links a block index field to a block definition </summary>
		///
		/// <tparam name="kFieldType">	Type of the index field. </tparam>
		/// <param name="source_block_definition">	[in] Block definition in which to find the block field. </param>
		/// <param name="block_field_path">		  	Field path to the block field. </param>
		/// <param name="target_block_definition">	[in] Block definition in which to find the block index field. </param>
		/// <param name="block_index_field_name"> 	Field path to the block index field. </param>
		template<Enums::field_type kFieldType>
		static void LinkBlockIndex(tag_block_definition& source_block_definition
			, cstring block_field_path
			, tag_block_definition& target_block_definition
			, cstring block_index_field_name)
		{
			static_assert((kFieldType == Enums::_field_long_block_index) || (kFieldType == Enums::_field_short_block_index)
				, "LinkBlockIndex can only be used with short and long block indices fields");

			// Get the source block field and the target block index
			auto& source_field = GetTagField(source_block_definition, Enums::_field_block, block_field_path);
			auto& target_field = GetTagField(target_block_definition, kFieldType, block_index_field_name);
			
			// Set the index fields definition
			target_field.definition = source_field.Definition<tag_block_definition>();
		}

#pragma region Tag Group Reference Definitions
		TAG_GROUP_REFERENCE_DEFINE(camera_track, 0,
			s_camera_track_definition::k_group_tag
		);

		TAG_GROUP_REFERENCE_DEFINE(effect, 0,
			effect_definition::k_group_tag
		);

		TAG_GROUP_REFERENCE_DEFINE(damage_effect, 0,
			s_damage_effect_definition::k_group_tag
		);

		TAG_GROUP_REFERENCE_DEFINE(bitmap, 0,
			s_bitmap_group::k_group_tag
		);

		TAG_GROUP_REFERENCE_DEFINE(sky, 0,
			s_sky_definition::k_group_tag
		);
#pragma endregion
	};};}
};