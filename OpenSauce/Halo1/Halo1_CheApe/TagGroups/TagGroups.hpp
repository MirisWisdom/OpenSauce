/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

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

	namespace Enums
	{
		enum field_type : _enum
		{
			_field_string,
			_field_char_integer,
			_field_short_integer,
			_field_long_integer,
			_field_angle,
			_field_tag,
			_field_enum,
			_field_long_flags,
			_field_word_flags,
			_field_byte_flags,
			_field_point_2d,
			_field_rectangle_2d,
			_field_rgb_color,
			_field_argb_color,
			_field_real,
			_field_real_fraction,
			_field_real_point_2d,
			_field_real_point_3d,
			_field_real_vector_2d,
			_field_real_vector_3d,
			_field_real_quaternion,
			_field_real_euler_angles_2d,
			_field_real_euler_angles_3d,
			_field_real_plane_2d,
			_field_real_plane_3d,
			_field_real_rgb_color,
			_field_real_argb_color,
			_field_real_hsv_color,
			_field_real_ahsv_color,
			_field_short_bounds,
			_field_angle_bounds,
			_field_real_bounds,
			_field_real_fraction_bounds,
			_field_tag_reference,
			_field_block,
			_field_short_block_index,
			_field_long_block_index,
			_field_data,
			_field_array_start,
			_field_array_end,
			_field_pad,
			_field_skip,
			_field_explanation,
			_field_custom,
			_field_terminator,

			_field_type,
		};

		enum {
			k_tag_block_format_buffer_size = 512,
		};
	};

	// NOTE: when loading for 'verification', it may be an implicit nod to 
	// the code that the editor is loading the tag.
	// OR
	// I have my namings wrong and what I have as 'verification' should
	// actually be termed as 'loading for game runtime'

	namespace Flags
	{
		enum /*tag_field_flags*/
		{
			// Never streamed unless the tag is loaded for verification purposes
			_tag_data_flag_never_streamed =			FLAG(0),
			// the sound board is available for data blobs of this type
			//_tag_data_flag_playable = FLAG(0),
			_tag_data_flag_is_text_data =			FLAG(1),
			// ie, 'debug data'
			_tag_data_flag_not_streamed_to_cache =	FLAG(2),

			// checked in the tag reference solving code.
			// last condition checked after an assortment of conditionals
			// and if this is TRUE, it won't resolve
			_tag_reference_flag_1 =				FLAG(0),
			_tag_reference_flag_non_resolving =	FLAG(1),

			_tag_block_flag_1 =	FLAG(0),

			_tag_group_flag_initialized =	FLAG(0),
			_tag_group_flag_2 =				FLAG(1),
			_tag_group_flag_3 =				FLAG(2),
			_tag_group_flag_4 =				FLAG(3),

			_tag_load_verify =					FLAG(0),
			_tag_load_verify_exist_first =		FLAG(1),
			_tag_load_non_resolving_references =FLAG(2),
		};
	};


	struct tag_field
	{
		_enum field_type; PAD16;
		cstring name;
		void* definition;

		template<typename T> API_INLINE T* Definition() const { return CAST_PTR(T*, definition); }
	}; BOOST_STATIC_ASSERT( sizeof(tag_field) == 0xC );

	typedef bool (PLATFORM_API* proc_tag_block_postprocess)(void* block, byte_flags flags);
	// if [formatted_buffer] returns empty, the default block formatting is done
	typedef cstring (PLATFORM_API* proc_tag_block_format)(datum_index tag_index, tag_block* block, int32 element, char formatted_buffer[Enums::k_tag_block_format_buffer_size]);
	struct tag_block_definition
	{
		cstring name;
		long_flags flags;
		int32 max_elements;
		uint32 element_size;
		PAD32;
		tag_field* fields;
		void* add_proc;
		proc_tag_block_postprocess postprocess_proc;
		proc_tag_block_format format_proc;
		void* delete_proc;
		int32* byteswap_codes;
	}; BOOST_STATIC_ASSERT( sizeof(tag_block_definition) == 0x2C );

	struct tag_data_definition
	{
		cstring name;
		long_flags flags;
		uint32 max_size;
		void* byte_swap_proc;
	}; BOOST_STATIC_ASSERT( sizeof(tag_data_definition) == 0x10 );

	struct tag_reference_definition
	{
		long_flags flags;
		tag group_tag;
		tag* group_tags;
	}; BOOST_STATIC_ASSERT( sizeof(tag_reference_definition) == 0xC );

	// Postprocess a tag definition (eg, automate the creation of fields, etc)
	// [for_runtime] - if true, prepare the tag for use in-game (Sapien and when building a cache)
	typedef bool (PLATFORM_API* proc_tag_group_postprocess)(datum_index tag_index, bool for_runtime);
	struct tag_group_definition
	{
		cstring name;
		long_flags flags;
		tag group_tag;
		tag parent_group_tag;
		int16 version; PAD16;
		proc_tag_group_postprocess postprocess_proc;
		tag_block_definition* definition;
		tag child_group_tags[16];
		int16 childs_count; PAD16;
	}; BOOST_STATIC_ASSERT( sizeof(tag_group_definition) == 0x60 );

	struct s_tag_instance
	{
		datum_index header;			// 0x0
		char filename[256];			// 0x4
		tag group_tag;				// 0x104
		tag parent_group_tags[2];	// 0x108 0x10C
		PAD8;						// 0x110
		bool is_read_only;			// 0x111
		bool is_orphan;				// 0x112
		bool null_definition;		// 0x113
		PAD32;						// 0x114
		uint32 file_checksum;		// 0x118
		tag_block root_block;		// 0x11C, 0x120, 0x124
	}; BOOST_STATIC_ASSERT( sizeof(s_tag_instance) == 0x128 );
	// Note: the datum count below is the 'stock' count used by the engine. Yelo can override this.
	typedef Memory::DataArray<s_tag_instance, 5120> t_tag_instance_data;

	namespace TagGroups
	{
		// just an endian swap
		inline void TagSwap(tag& x)
		{
			x = (x>>24) | 
				((x<<8) & 0x00FF0000) |
				((x>>8) & 0x0000FF00) |
				(x<<24);
		}

		// Union hack to use a group tag as a string
		union group_tag_to_string
		{
			struct {
				tag group;
				PAD8; // null terminator
			};
			char str[sizeof(tag)+1];

			inline void Terminate() { str[4] = '\0'; }
			inline void TagSwap()	{ TagGroups::TagSwap(group); }
		};

		t_tag_instance_data*	TagInstances();


		void Initialize();

		// If I haven't taken the time to find the address of a tag function 
		// or if the function I need doesn't exist to my knowledge
		// I'll add an implementation or declaration here

		API_INLINE bool tag_is_read_only(datum_index tag_index)
		{
			if( tag_index.IsNull() ) return false;

			return (*TagInstances())[tag_index]->is_read_only;
		}

		// Searches [def] for a field of type [field_type] with a name which starts 
		// with [name] characters. Optionally starts at a specific field index.
		// Returns NONE if this fails.
		int32 tag_block_find_field(const tag_block_definition* def, 
			_enum field_type, cstring name, int32 start_index = NONE);

		// Clear the values of a tag reference so that it references no tag
		void tag_reference_clear(tag_reference& reference);

		// Convenience function to handle deleting all of the data in tag_data field.
		// Use [terminator_size] for tag_data which HAS to have a specific amount of 
		// bytes no matter what. IE, text data requires 1 or 2 bytes (ascii or unicode) 
		// for the null terminator.
		API_INLINE void tag_data_delete(tag_data& data, size_t terminator_size = 0)
		{
			data.address = YELO_REALLOC(data.address, data.size = terminator_size);
		}
	};



	// Get the tag definition's address by it's expected group tag and 
	// it's tag handle [tag_index]
	void* tag_get(tag group_tag, datum_index tag_index);
	template<typename T>
	T* tag_get(datum_index tag_index)
	{
		return CAST_PTR(T*, tag_get(T::k_group_tag, tag_index));
	}

	// Get the group definition based on a four-character code
	tag_group_definition* tag_group_get(tag group_tag);
	template<typename T>
	tag_group_definition* tag_group_get()
	{
		return tag_group_get(T::k_group_tag);
	}

	// Rename the tag definition [tag_index] to [new_name]
	void tag_rename(datum_index tag_index, cstring new_name);

	tag_block* tag_block_index_resolve(datum_index tag_index, tag_field* block_index_field, int32 index);

	// Get the size in bytes of how much memory the tag definition [tag_index] 
	// consumes with all of its child data too
	uint32 tag_size(datum_index tag_index);



	int32 tag_reference_set(tag_reference* reference, tag group_tag, cstring name);
	template<typename T>
	int32 tag_reference_set(tag_reference* reference, cstring name)
	{
		return tag_reference_set(reference, T::k_group_tag, name);
	}

	// Get the size in bytes of how much memory [block] consumes with all 
	// of its child data too
	uint32 tag_block_size(tag_block* block);

	// Get the address of a block element which exists at [element]
	void* tag_block_get_element(tag_block* block, int32 element);
	template<typename T>
	T* tag_block_get_element(TagBlock<T>& block, int32 element)
	{
		YELO_ASSERT(sizeof(T) == block.BlockDefinition->element_size);

		return CAST_PTR(T*, tag_block_get_element(block.to_tag_block(), element));
	}

	datum_index tag_new(tag group_name, cstring name);
	template<typename T>
	datum_index tag_new(cstring name)
	{
		return tag_new(T::k_group_tag, name);
	}

	bool tag_save(datum_index tag_index);

	// Delete the block element at [element]
	void tag_block_delete_element(void* block, int32 element);
	template<typename T>
	void tag_block_delete_element(TagBlock<T>& block, int32 element)
	{
		tag_block_delete_element(block.to_tag_block(), element);
	}

	// Add a new block element and return the index which 
	// represents the newly added element
	int32 tag_block_add_element(tag_block* block);
	template<typename T>
	int32 tag_block_add_element(TagBlock<T>& block)
	{
		return tag_block_add_element(block.to_tag_block());
	}

	// Resize the block to a new count of elements, returning the 
	// success result of the operation
	bool tag_block_resize(tag_block* block, int32 element_count);
	template<typename T>
	bool tag_block_resize(TagBlock<T>& block, int32 element_count)
	{
		return tag_block_resize(block.to_tag_block(), element_count);
	}

	// Insert a new block element at [index] and return the address 
	// of the inserted element
	void* tag_block_insert_element(tag_block* block, int32 index);
	template<typename T>
	T* tag_block_insert_element(TagBlock<T>& block, int32 index)
	{
		YELO_ASSERT(sizeof(T) == block.BlockDefinition->element_size);

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

	// Load a tag definition into memory.
	// Returns the tag handle of the loaded tag definition
	datum_index tag_load(tag group_tag, cstring name, long_flags file_flags);
	template<typename T>
	datum_index tag_load(cstring name, long_flags file_flags)
	{
		return tag_load(T::k_group_tag, name, file_flags);
	}

	// Unload a tag definition from memory.
	// [tag_index] will resolve to an invalid index after this returns.
	void tag_unload(datum_index tag_index);


	// Use [NULL_HANDLE] for [group_tag_filter] to iterate all tag groups
	void tag_iterator_new(TagGroups::tag_iterator& iter, const tag group_tag_filter = NULL_HANDLE);
	template<typename T>
	void tag_iterator_new(TagGroups::tag_iterator& iter)
	{
		tag_iterator_new(iter, T::k_group_tag);
	}

	// Returns [datum_index::null] when finished iterating
	datum_index tag_iterator_next(TagGroups::tag_iterator& iter);
};