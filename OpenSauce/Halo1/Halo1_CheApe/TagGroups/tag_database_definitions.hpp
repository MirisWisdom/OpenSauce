/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		struct s_tag_database_entry // tag_database_entry_block
		{
			TagData<char> name;
			union {
				ULARGE_INTEGER handle_data;

				tag group_tag; // the group_tag is actually the low-word of the handle.
			};
			uint32 flags;
			// Indexes to entries which this entry references
			TAG_TBLOCK(child_ids, int32);		// tag_database_entry_reference_block - long_block_index
			// Entries which reference this entry
			TAG_TBLOCK(reference_ids, int32);	// tag_database_entry_reference_block - long_block_index

			TAG_PAD(int32, 3); // 12

			void NameToBlockNameBuffer(char formatted_buffer[Enums::k_tag_block_format_buffer_size]);
		}; BOOST_STATIC_ASSERT( sizeof(s_tag_database_entry) == 0x44 );

		struct s_tag_database
		{
			enum {
				k_version = 1,
				k_group_tag = 'tag+',
			};

			TAG_TBLOCK(entries, s_tag_database_entry);

			TAG_PAD(int32, 6); // 24


			static void Initialize();
			static void Dispose();
		};

		cstring PLATFORM_API tag_database_entry_reference_block_format(
			datum_index tag_index, 
			tag_block* block, 
			int32 element, 
			char formatted_buffer[Enums::k_tag_block_format_buffer_size]);

		cstring PLATFORM_API tag_database_entry_block_format(
			datum_index tag_index, 
			tag_block* block, 
			int32 element, 
			char formatted_buffer[Enums::k_tag_block_format_buffer_size]);
	};
};