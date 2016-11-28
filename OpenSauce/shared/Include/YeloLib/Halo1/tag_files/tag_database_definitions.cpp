/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/tag_files/tag_database_definitions.hpp>

#if PLATFORM_IS_EDITOR
	#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#endif

namespace Yelo
{
	namespace TagGroups
	{
#if PLATFORM_IS_EDITOR
		void s_tag_database_entry::NameToBlockNameBuffer(char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			if(this->name.Size > 0)
			{
				tag_group* group_definition = blam::tag_group_get(this->group_tag);
				if(group_definition != nullptr)
				{
					sprintf_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, 
						"%s.%s", this->name.Definitions, group_definition->name);
				}
			}
		}

		void s_tag_database::Initialize()
		{
			tag_group* definition = blam::tag_group_get<s_tag_database>();
			if(definition == nullptr)
			{
				YELO_ERROR_CRITICAL("CheApe: tag_database not found!");
			}
			else if(definition->version == k_version)
			{
				//////////////////////////////////////////////////////////////////////////
				{// s_tag_database_entry block
					auto* tag_database_entry_block_def = definition->header_block_definition->fields[0].Definition<tag_block_definition>();
					tag_database_entry_block_def->format_proc = &TagGroups::tag_database_entry_block_format;

					// child ids
					auto* entry_reference_block_def = tag_database_entry_block_def->fields[4].Definition<tag_block_definition>();
					entry_reference_block_def->format_proc = &TagGroups::tag_database_entry_reference_block_format;

					// reference ids
					entry_reference_block_def = tag_database_entry_block_def->fields[5].Definition<tag_block_definition>();
					entry_reference_block_def->format_proc = &TagGroups::tag_database_entry_reference_block_format;
				}
			}
		}

		void s_tag_database::Dispose()
		{
		}


		cstring PLATFORM_API tag_database_entry_reference_block_format(
			datum_index tag_index, 
			tag_block* block, 
			int32 element, 
			char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			int32 entry = 
				*CAST_PTR(int32*, blam::tag_block_get_element(block, element));

			formatted_buffer[0] = '\0';

			if(entry != NONE)
			{
				auto* db = blam::tag_get<s_tag_database>(tag_index);

				db->entries[ entry ].NameToBlockNameBuffer(formatted_buffer);
			}


			return formatted_buffer;
		}

		cstring PLATFORM_API tag_database_entry_block_format(
			datum_index tag_index, 
			tag_block* block, 
			int32 element, 
			char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			auto* elem = CAST_PTR(s_tag_database_entry*, blam::tag_block_get_element(block, element));

			formatted_buffer[0] = '\0';

			elem->NameToBlockNameBuffer(formatted_buffer);

			return formatted_buffer;
		}
#endif
	};
};