/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cseries/enum_templates.h>
#include <blamlib/game/game_globals_definitions.hpp>
#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/tag_files/tag_groups_base.hpp>
#include <blamlib/memory/datum_index.hpp>
#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	struct tag_block;
	struct tag_data_definition;
	struct tag_reference;
	struct tag_data;

	namespace e_tag_load_flags
	{
		typedef enum : long_flags
		{
			// When this is set, implies _tag_postprocess_mode_for_editor, else _for_runtime
			for_editor_bit = 0,
			// Load the tag from the file system, not from a cache/index
			from_file_system_bit,
			// If set: child references of the tag being loaded are not loaded themselves
			// Else, child references are loaded from disk
			non_resolving_references_bit,

			k_count
		} type_t;

		typedef flags_type<type_t> flags_t;
	}

	namespace Enums
	{
		enum
		{
			k_max_tag_name_length = 255,

			k_tag_block_format_buffer_size = 512,

			k_maximum_tags_per_tag_chain = 4,
			k_maximum_children_per_tag = 16,
		};

		// Note: AFAICT, the engine code doesn't actually do the postprocess setup this way.
		// They have what is essentially a boolean parameter that could be considered as 'bool for_editor'
		enum tag_postprocess_mode : byte_enum
		{
			// In this mode, the tag is being postprocessed for runtime values (automatically fill fields, etc)
			_tag_postprocess_mode_for_runtime = FALSE,
			// In this mode we're opening for tag editing (eg, tool process or guerilla) and should skip the postprocessing
			// code which prepares the tag for use in-game (Sapien and when building a cache)
			_tag_postprocess_mode_for_editor = TRUE,
		};
	}

	namespace blam
	{
		// Clear the values of a tag reference so that it references no tag
		void PLATFORM_API tag_reference_clear(
			tag_reference& reference);

		void PLATFORM_API tag_reference_set(
			tag_reference& reference,
			const tag group_tag,
			cstring name);

		template <typename t_type>
		void tag_reference_set(
			tag_reference& reference,
			cstring name)
		{
			return tag_reference_set(reference, t_type::k_group_tag, name);
		}

		datum_index PLATFORM_API tag_reference_try_and_get(
			const tag_reference* const reference);

		bool PLATFORM_API tag_reference_resolve(
			tag_reference* const reference);

		// non-standard overload of the above resolve()
		bool tag_reference_resolve(
			tag_reference& reference,
			tag expected_group_tag);

		template <typename T>
		bool tag_reference_resolve(
			tag_reference& reference)
		{
			return tag_reference_resolve(reference, T::k_group_tag);
		}
	}

	namespace blam
	{
		// Get the address of a block element which exists at [element_index]
		void* PLATFORM_API tag_block_get_element(
			tag_block* block,
			int32 element_index);
		const void* PLATFORM_API tag_block_get_element(
			const tag_block* block,
			int32 element_index);
		// Add a new block element and return the index which 
		// represents the newly added element
		int32 PLATFORM_API tag_block_add_element(
			tag_block*const block);
		// Resize the block to a new count of elements, returning the 
		// success result of the operation
		bool PLATFORM_API tag_block_resize(
			tag_block*const block,
			const int32 element_count);
		// Delete the block element at [element_index]
		void PLATFORM_API tag_block_delete_element(
			tag_block*const block,
			int32 element_index);

		void* tag_block_add_and_get_element(
			tag_block* block);
	}

	namespace blam
	{
		bool PLATFORM_API tag_data_resize(
			tag_data*const data,
			const int32 new_size);

		void* PLATFORM_API tag_data_get_pointer(
			tag_data& data,
			const int32 offset,
			const int32 size);

		template <typename t_type>
		t_type* tag_data_get_pointer(
			tag_data& data,
			const int32 offset,
			const int32 index = 0)
		{
			return reinterpret_cast<t_type*>(tag_data_get_pointer(data, -offset + (sizeof(t_type) * index), -sizeof(t_type)));
		}
	}


	namespace blam
	{
		tag PLATFORM_API tag_get_group_tag(
			datum_index tag_index);

		tag_block* PLATFORM_API tag_get_root_block(
			datum_index tag_index);

		void PLATFORM_API tag_orphan(
			datum_index tag_index);

		datum_index PLATFORM_API tag_loaded(
			tag group_tag,
			cstring name);

		template <typename t_type>
		datum_index tag_loaded(
			const cstring name)
		{
			return tag_loaded(t_type::k_group_tag, name);
		}

		cstring PLATFORM_API tag_get_name(
			datum_index tag_index);

		inline cstring tag_try_get_name(
			datum_index tag_index)
		{
			return tag_index.IsNull()
				? "<unspecified tag>"
				: tag_get_name(tag_index);
		}

		bool PLATFORM_API tag_read_only(
			datum_index tag_index);

		#if PLATFORM_USES_CACHE_FILES
		#define TAG_GET_RETURN_MODIFIER const
		#else
		#define TAG_GET_RETURN_MODIFIER
		#endif

		// Get the tag definition's address by it's expected group tag and 
		// it's tag handle [tag_index]
		TAG_GET_RETURN_MODIFIER void* PLATFORM_API tag_get(
			tag group_tag,
			const datum_index tag_index);

		template <typename t_type>
		TAG_GET_RETURN_MODIFIER t_type* tag_get(
			datum_index tag_index)
		{
			return CAST_PTR(TAG_GET_RETURN_MODIFIER t_type*, tag_get(t_type::k_group_tag, tag_index));
		}
		#undef TAG_GET_RETURN_MODIFIER

		datum_index PLATFORM_API tag_new(
			tag group_name,
			cstring name);

		template <typename t_type>
		datum_index tag_new(
			const cstring name)
		{
			return tag_new(t_type::k_group_tag, name);
		}

		// Load a tag definition into memory.
		// Returns the tag handle of the loaded tag definition
		datum_index PLATFORM_API tag_load(
			tag group_tag,
			cstring name,
			e_tag_load_flags::flags_t file_flags);

		template <typename t_type>
		datum_index tag_load(
			const cstring name,
			const e_tag_load_flags::flags_t file_flags)
		{
			return tag_load(t_type::k_group_tag, name, file_flags);
		}

		datum_index PLATFORM_API tag_reload(
			tag group_tag,
			cstring name);

		template <typename t_type>
		datum_index tag_reload(
			const cstring name)
		{
			return tag_reload(t_type::k_group_tag, name);
		}

		void PLATFORM_API tag_load_children(
			datum_index tag_index);

		// Unload a tag definition from memory.
		// [tag_index] will resolve to an invalid index after this returns.
		void PLATFORM_API tag_unload(
			datum_index tag_index);

		bool PLATFORM_API tag_save(
			datum_index tag_index);
	}

	namespace TagGroups
	{
		// just an endian swap
		void TagSwap(
			tag& x);

		tag string_to_group_tag(
			cstring name);

		// Returns true if the tag is an instance of the group_tag or is a child group of it.
		// Returns false if not, or tag_index is invalid.
		bool TagIsInstanceOf(
			datum_index tag_index,
			tag group_tag);

		template <typename T>
		bool TagIsInstanceOf(
			datum_index tag_index)
		{
			return TagIsInstanceOf(tag_index, T::k_group_tag);
		}

		// Returns the tag as non-const. Are you sure you want to be writing to tags at runtime?
		template <typename T>
		T* TagGetForModify(
			datum_index tag_index)
		{
			return CAST_QUAL(T*, blam::tag_get<T>(tag_index));
		}

		// 'Unsafe' in that it returns the tag as non-const and doesn't do any bounds checking
		// Useful when you're using tag_iterator and known you're getting some good input
		template <typename T>
		T* TagGetUnsafe(
			datum_index tag_index)
		{
			extern void* TagGetUnsafeImpl(
				datum_index tag_index);

			return CAST_PTR(T*, TagGetUnsafeImpl(tag_index));
		}

		// Union hack to use a group tag as a string
		union group_tag_to_string
		{
			struct
			{
				tag group;
				PAD8; // null terminator
			};

			char str[sizeof(tag) + 1];

			group_tag_to_string& Terminate();
			group_tag_to_string& TagSwap();

			cstring ToString()
			{
				return Terminate().TagSwap().str;
			}
		};
	}
}
