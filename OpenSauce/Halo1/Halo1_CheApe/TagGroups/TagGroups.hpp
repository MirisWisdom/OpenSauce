/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#if PLATFORM_ID != PLATFORM_TOOL
#include <blamlib/Halo1/guerilla/tags_interface.hpp>
#endif

#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	//////////////////////////////////////////////////////////////////////////
	// cache interface

	struct s_data_file_header
	{
		int32 type;
		uint32 file_table_offset;
		uint32 file_index_table_offset;
		int32 tag_count;
	};

	struct s_data_file_item
	{
		uint32 name_offset;
		uint32 size;
		uint32 data_offset;
	};

	struct s_data_file
	{
		s_data_file_header header;

		struct {
			s_data_file_item* instances;
			int32 count;
		}file_index;

		struct {
			uint32 total_size;
			uint32 used_size;
			char* buffer;
		};

		bool writable;
		PAD24;

		struct {
			int32 count;
			uint32 size;
		}unreferenced_items, referenced_items;

		cstring name;
		HANDLE file_handle;
	}; BOOST_STATIC_ASSERT( sizeof(s_data_file) == 0x40 );

	//////////////////////////////////////////////////////////////////////////
	// tag interface

	namespace TagGroups
	{
		tag_instance_data_t*	TagInstances();


		void Initialize();

		// Patches stock tag groups for various *fixes* (eg, to correct behavior), not additions or the like
		// Should be called after the engine's and Yelo's TagGroups initializer are ran
		void InitializeFixes();

		// If I haven't taken the time to find the address of a tag function 
		// or if the function I need doesn't exist to my knowledge
		// I'll add an implementation or declaration here

		// Searches [def] for a field of type [field_type] with a name which starts 
		// with [name] characters. Optionally starts at a specific field index.
		// Returns NONE if this fails.
		int32 tag_block_find_field(const tag_block_definition* def, 
			_enum field_type, cstring name, int32 start_index = NONE);

		// Convenience function to handle deleting all of the data in tag_data field.
		// Use [terminator_size] for tag_data which HAS to have a specific amount of 
		// bytes no matter what. IE, text data requires 1 or 2 bytes (ascii or unicode) 
		// for the null terminator.
		void tag_data_delete(tag_data* data, size_t terminator_size = 0);
		template<typename T>
		void tag_data_delete(TagData<T>& data, size_t terminator_size = 0)
		{
			tag_data_delete(data.to_tag_data(), terminator_size);
		}

		bool tag_block_delete_all_elements(tag_block* block);
		template<typename T>
		bool tag_block_delete_all_elements(TagBlock<T>& block)
		{
			return tag_block_delete_all_elements(block.to_tag_block());
		}

		s_tag_field_scan_state& field_scan_state_new(s_tag_field_scan_state& state, const tag_field* fields, void* fields_address);
		void field_scan_state_add_field_type(s_tag_field_scan_state& state, Enums::field_type field_type);
		bool field_scan(s_tag_field_scan_state& state);
	};


	// Get the group definition based on a four-character code
	tag_group* tag_group_get(tag group_tag);
	template<typename T>
	tag_group* tag_group_get()
	{
		return tag_group_get(T::k_group_tag);
	}

	// Rename the tag definition [tag_index] to [new_name]
	void tag_rename(datum_index tag_index, cstring new_name);

	tag_block* tag_block_index_resolve(datum_index tag_index, tag_field* block_index_field, int32 index);

	// Get the size in bytes of how much memory the tag definition [tag_index] 
	// consumes with all of its child data too
	uint32 tag_size(datum_index tag_index);



	// Get the size in bytes of how much memory [block] consumes with all 
	// of its child data too
	uint32 tag_block_size(tag_block* block);

	// Insert a new block element at [index] and return the index 
	// of the inserted element
	int32 tag_block_insert_element(tag_block* block, int32 index);
	template<typename T>
	T* tag_block_insert_element(TagBlock<T>& block, int32 index)
	{
		return CAST_PTR(T*, tag_block_insert_element(block.to_tag_block(), index));
	}
	// Duplicate the block element at [element] and return the index which 
	// represents the duplicated element
	int32 tag_block_duplicate_element(tag_block* block, int32 element);
	template<typename T>
	int32 tag_block_duplicate_element(TagBlock<T>& block, int32 element)
	{
		return tag_block_duplicate_element(block.to_tag_block(), element);
	}


	// Use [NULL_HANDLE] for [group_tag_filter] to iterate all tag groups
	void tag_iterator_new(TagGroups::tag_iterator& iter, const tag group_tag_filter = NULL_HANDLE);
	template<typename T>
	void tag_iterator_new(TagGroups::tag_iterator& iter)
	{
		tag_iterator_new(iter, T::k_group_tag);
	}

	// Returns [datum_index::null] when finished iterating
	datum_index tag_iterator_next(TagGroups::tag_iterator& iter);

	// Is the tag file read only?
	bool tag_file_read_only(tag group_tag, cstring name);
	// Does the tag file exist?
	bool tag_file_exists(tag group_tag, cstring name);
};