/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum tag_allocation_type : _enum {
			_tag_allocation_type_block,
			_tag_allocation_type_data,

			k_number_of_tag_allocation_types
		};

		enum tag_block_child_type : _enum {
			_tag_block_child_type_tag_reference,
			_tag_block_child_type_block_index,
			_tag_block_child_type_string_id,

			k_number_of_tag_block_child_types
		};
	};

	namespace TagGroups
	{
		class c_cache_file_memory_layout_table
		{
		public:
			enum { k_group_tag = 'meml' };

			//////////////////////////////////////////////////////////////////////////
			// allocations
			struct s_tag_allocation_definition
			{
				typedef int16 block_index_t;

				TAG_FIELD(size_t, element_runtime_size);
				TAG_FIELD(word_flags, flags);
				TAG_FIELD(int16, alignment_bit);
			};
			struct s_tag_allocation
			{
				typedef int32 block_index_t;

				TAG_BLOCK_INDEX(s_tag_allocation_definition, allocation_index);
				PAD16;
				TAG_FIELD(size_t, size);
				TAG_FIELD(uintptr_t, rva);
			};

			struct s_tag_block_allocation
			{
				typedef int32 block_index_t;

				TAG_BLOCK_INDEX(s_tag_allocation, allocation_index);
				TAG_TBLOCK(child_allocations, s_tag_allocation::block_index_t);
			};

			struct s_tag_data_allocation
			{
				TAG_BLOCK_INDEX(s_tag_allocation, allocation_index);
			};

			//////////////////////////////////////////////////////////////////////////
			// child data locations
			struct s_tag_block_child_definition
			{
				typedef int16 block_index_t;

				TAG_BLOCK_INDEX(s_tag_allocation_definition, parent_definition_index);
				TAG_FIELD(uint16, parent_offset);
				TAG_ENUM(type, Enums::tag_block_child_type);
				PAD16;
			}; BOOST_STATIC_ASSERT( sizeof(s_tag_block_child_definition) == 8 );

			struct s_tag_block_child_instance
			{
				typedef int32 block_index_t;

				TAG_BLOCK_INDEX(s_tag_block_child_definition, definition_index);
				TAG_BLOCK_INDEX(s_tag_allocation_definition, owner_block_index);
			}; BOOST_STATIC_ASSERT( sizeof(s_tag_block_child_instance) == 4 );

			//////////////////////////////////////////////////////////////////////////
			typedef TagBlock<s_tag_block_child_instance::block_index_t>
				string_id_set_entry_location_t;
			struct s_string_id_set
			{
				TAG_FIELD(int32, set_index);
				TAG_TBLOCK(ids_bitvector, byte);
				TAG_TBLOCK(id_locations, string_id_set_entry_location_t);
			};

			struct s_tag_instance
			{
				TAG_BLOCK_INDEX(s_tag_block_allocation, header_block_index);
				TAG_TBLOCK(references, s_tag_block_child_instance::block_index_t); // indices to [tag_reference_locations]
				TAG_FIELD(uintptr_t, name_offset); // offset of the tag name from the start of the tag name buffer
			};

			TAG_FIELD(uint32, tags_checksum);
			TAG_FIELD(uintptr_t, virtual_base_address);
			PAD64;

			TAG_TBLOCK(allocation_definitions, s_tag_allocation_definition);
			TAG_TBLOCK(allocations, s_tag_allocation);
			TAG_TBLOCK(block_allocations, s_tag_block_allocation);
			TAG_TBLOCK(data_allocations, s_tag_data_allocation);

			PAD_TYPE(tag_block);

			TAG_TBLOCK(children_definitions, s_tag_block_child_definition);
			TAG_TBLOCK(tag_reference_locations, s_tag_block_child_instance);
			TAG_TBLOCK(string_id_locations, s_tag_block_child_instance);

			TAG_TBLOCK(string_id_sets, s_string_id_set);

			TAG_TBLOCK(tag_instances, s_tag_instance);
		};
	};
};