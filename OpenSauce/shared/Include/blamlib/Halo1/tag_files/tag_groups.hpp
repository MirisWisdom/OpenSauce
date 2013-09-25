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

		extern const s_tag_field_definition k_tag_field_definitions[];

		// Get the size, in characters, of a string field, inclusive of the null character
		int32 StringFieldGetSize(const tag_field* field);

		// Convenience function to handle deleting all of the data in tag_data field.
		// Use [terminator_size] for tag_data which HAS to have a specific amount of 
		// bytes no matter what. IE, text data requires 1 or 2 bytes (ascii or unicode) 
		// for the null terminator.
		void tag_data_delete(tag_data* data, size_t terminator_size = 0);
		template<typename T> inline
		void tag_data_delete(TagData<T>& data, size_t terminator_size = 0)
		{
			tag_data_delete(data.to_tag_data(), terminator_size);
		}

		bool tag_block_delete_all_elements(tag_block* block);
		template<typename T> inline
		bool tag_block_delete_all_elements(TagBlock<T>& block)
		{
			return tag_block_delete_all_elements(block.to_tag_block());
		}

		// Note: when used in range based for loops this will create an unnecessary copy operation, but with SSE2 it shouldn't be that bad
		class c_tag_iterator {
			s_tag_iterator m_state;
			datum_index m_tag_index;

			c_tag_iterator(const void* endHackDummy);
		public:
			c_tag_iterator(const tag group_tag_filter);
			template<typename T> inline
			c_tag_iterator() :
				m_tag_index(datum_index::null)
			{
				blam::tag_iterator_new<T>(m_state);
			}
			// Get an iterator that doesn't have any specific group_tag filter
			static inline c_tag_iterator all()
			{
				return c_tag_iterator(NULL_HANDLE);
			}

			datum_index Next();

			inline bool operator!=(const c_tag_iterator& other) const
			{
				// NOTE: we intentionally don't compare the group_tag filter
				return m_state.instances_iterator != other.m_state.instances_iterator;
			}
			inline c_tag_iterator& operator++()
			{
				Next();
				return *this;
			}
			inline datum_index operator*() const
			{
				return m_tag_index;
			}

			inline c_tag_iterator& begin() /*const*/
			{
				this->Next();
				return *this;
			}
			inline static const c_tag_iterator end() /*const*/
			{
				return c_tag_iterator(nullptr);
			}
		};
#endif
	};

	namespace blam
	{
		void PLATFORM_API tag_groups_initialize();
		void PLATFORM_API tag_groups_dispose();

#if PLATFORM_IS_EDITOR
		TagGroups::s_tag_field_scan_state& PLATFORM_API tag_field_scan_state_new(TagGroups::s_tag_field_scan_state& state, 
			const tag_field* fields, void* fields_address);
		void PLATFORM_API tag_field_scan_state_add_field_type(TagGroups::s_tag_field_scan_state& state, 
			Enums::field_type field_type);
		bool PLATFORM_API tag_field_scan(TagGroups::s_tag_field_scan_state& state);
#endif

		datum_index PLATFORM_API find_tag_instance(tag group_tag, cstring name);

		// Use [NULL_HANDLE] for [group_tag_filter] to iterate all tag groups
		void PLATFORM_API tag_iterator_new(TagGroups::s_tag_iterator& iter, const tag group_tag_filter = NULL_HANDLE);
		template<typename T> inline
		void tag_iterator_new(TagGroups::s_tag_iterator& iter)
		{
			tag_iterator_new(iter, T::k_group_tag);
		}

		// Returns [datum_index::null] when finished iterating
		datum_index PLATFORM_API tag_iterator_next(TagGroups::s_tag_iterator& iter);

#if PLATFORM_IS_EDITOR
		// Get the group definition that follows [group], or the first group if it is NULL
		tag_group* PLATFORM_API tag_group_get_next(const tag_group* group = nullptr);

		// Get the group definition based on a four-character code
		tag_group* PLATFORM_API tag_group_get(tag group_tag);
		template<typename T> inline
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
		template<typename T> inline
		T* tag_block_insert_element(TagBlock<T>& block, int32 index)
		{
			return CAST_PTR(T*, tag_block_insert_element(block.to_tag_block(), index));
		}

		// Duplicate the block element at [element_index] and return the index which 
		// represents the duplicated element
		int32 PLATFORM_API tag_block_duplicate_element(tag_block* block, int32 element_index);
		template<typename T> inline
		int32 tag_block_duplicate_element(TagBlock<T>& block, int32 element_index)
		{
			return tag_block_duplicate_element(block.to_tag_block(), element_index);
		}

		void PLATFORM_API tag_block_generate_default_element(const tag_block_definition *definition, void *address);
#endif
	};
};