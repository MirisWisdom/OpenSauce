/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/tag_files/tag_groups_base.hpp>
#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

#if PLATFORM_IS_EDITOR
	#include <blamlib/Halo1/tag_files/tag_groups_structures.hpp>
#endif

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_protected_group_tag = 'prot', // HEK+: this overrides the scenario's group tag in the tag index.
		};
	};

	namespace TagGroups
	{
#if PLATFORM_IS_EDITOR
		typedef Memory::DataArray<	s_tag_instance, 
								Enums::k_maximum_simultaneous_tag_instances,
								Enums::k_maximum_simultaneous_tag_instances_upgrade> 
			tag_instance_data_t;
		tag_instance_data_t& TagInstances();

		extern s_tag_field_definition k_tag_field_definitions[];

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
#endif
	};

	namespace blam
	{
#if PLATFORM_IS_EDITOR
		TagGroups::s_tag_field_scan_state& PLATFORM_API tag_field_scan_state_new(TagGroups::s_tag_field_scan_state& state, 
			const tag_field* fields, void* fields_address);
		void PLATFORM_API tag_field_scan_state_add_field_type(TagGroups::s_tag_field_scan_state& state, 
			Enums::field_type field_type);
		bool PLATFORM_API tag_field_scan(TagGroups::s_tag_field_scan_state& state);
#endif

		// Use [NULL_HANDLE] for [group_tag_filter] to iterate all tag groups
		void PLATFORM_API tag_iterator_new(TagGroups::s_tag_iterator& iter, const tag group_tag_filter = NULL_HANDLE);
		template<typename T>
		void tag_iterator_new(TagGroups::s_tag_iterator& iter)
		{
			tag_iterator_new(iter, T::k_group_tag);
		}

		// Returns [datum_index::null] when finished iterating
		datum_index PLATFORM_API tag_iterator_next(TagGroups::s_tag_iterator& iter);

#if PLATFORM_IS_EDITOR
		// Get the group definition based on a four-character code
		tag_group* PLATFORM_API tag_group_get(tag group_tag);
		template<typename T>
		tag_group* tag_group_get()
		{
			return tag_group_get(T::k_group_tag);
		}

		// Rename the tag definition [tag_index] to [new_name]
		void PLATFORM_API tag_rename(datum_index tag_index, cstring new_name);

		tag_block* PLATFORM_API tag_block_index_resolve(datum_index tag_index, tag_field* block_index_field, int32 index);

		// Get the size in bytes of how much memory the tag definition [tag_index] 
		// consumes with all of its child data too
		uint32 PLATFORM_API tag_size(datum_index tag_index);

		// Get the size in bytes of how much memory [block] consumes with all 
		// of its child data too
		uint32 PLATFORM_API tag_block_size(tag_block* block);

		// Insert a new block element at [index] and return the index 
		// of the inserted element
		int32 PLATFORM_API tag_block_insert_element(tag_block* block, int32 index);
		template<typename T>
		T* tag_block_insert_element(TagBlock<T>& block, int32 index)
		{
			return CAST_PTR(T*, tag_block_insert_element(block.to_tag_block(), index));
		}

		// Duplicate the block element at [element_index] and return the index which 
		// represents the duplicated element
		int32 PLATFORM_API tag_block_duplicate_element(tag_block* block, int32 element_index);
		template<typename T>
		int32 tag_block_duplicate_element(TagBlock<T>& block, int32 element_index)
		{
			return tag_block_duplicate_element(block.to_tag_block(), element_index);
		}
#endif
	};
};