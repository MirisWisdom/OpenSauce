/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/byte_swapping_base.hpp>
#include <blamlib/tag_files/tag_groups_base.hpp>
#include <blamlib/tag_files/tag_block.h>
#include <blamlib/tag_files/tag_field.h>
#include <blamlib/tag_files/tag_block_definition.h>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_tag_field_set_runtime_data; // yelo-based hacks, not blam
	};

	namespace Enums
	{
		enum {
			k_maximum_field_byte_swap_codes = 1024,

			// these chars should all match the TAG_FIELD_MARKUP_* defines in tag_groups_structures_macros.hpp
			
			k_tag_field_markup_character_advanced =		'!',
			k_tag_field_markup_character_help_prefix =	'#',
			k_tag_field_markup_character_read_only =	'*',
			k_tag_field_markup_character_units_prefix =	':',
			k_tag_field_markup_character_block_name =	'^',
		};
	};

	namespace Flags
	{
		enum {
			// Never streamed, unless the tag is loaded with _tag_load_for_editor_bit
			// Used by the sound tag for delay loading the actual sample data. So, eg, when tool goes to build a cache
			// it has to use tag_data_load on the sound samples. Maybe a better name is just 'lazy_loaded'
			_tag_data_never_streamed_bit = 0,
			_tag_data_is_text_data_bit,
			// ie, 'debug data'
			_tag_data_not_streamed_to_cache_bit,
			k_number_of_tag_data_definition_flags,

			// checked in the tag reference solving code.
			// last condition checked after an assortment of conditionals
			// and if this is FALSE, it won't resolve
			// NOTE: I think this a deprecated flag for loading the 'default' definition of a group.
			// This flag would cause a call of tag_load(group_tag, NULL, 0) to occur. However,
			// tag_load asserts name != NULL.
			// Flag isn't checked in H2EK's code (so more than likely deprecated)
			_tag_reference_unknown0_bit = 0,
			_tag_reference_non_resolving_bit,
			k_number_of_tag_group_tag_reference_flags,

			// set when block/data/reference fields are found during initialization
			_tag_block_has_children_bit = 0,
			k_number_of_tag_block_definition_flags,

			// tag_instance of the tag group will have their file_checksum CRC'd into the resulting cache tag header's crc field 
			_tag_group_include_in_tags_checksum_bit = 0,
			// only seen preferences_network_game use this
			_tag_group_unknown1_bit,
			// haven't seen this used at all
			_tag_group_unknown2_bit,
			// majority of tags have this set
			_tag_group_reloadable_bit,

			// YELO ONLY! tag_group is not meant for use in runtime builds
			_tag_group_debug_only_yelo_bit,
			// YELO ONLY! tag_group is non-standard (ie, official). This is applied to those defined in CheApe.map
			// Set at startup.
			_tag_group_non_standard_yelo_bit,
			// YELO ONLY! tag_group is referenced in other groups via a tag_reference or as a parent group
			// doesn't consider tag_references which can reference ANYTHING (eg, tag_collection's field).
			// Set at startup.
			_tag_group_referenced_yelo_bit,

			// When this is set, implies _tag_postprocess_mode_for_editor, else _for_runtime
			_tag_load_for_editor_bit = 0,
			// Load the tag from the file system, not from a cache/index
			_tag_load_from_file_system_bit,
			// If set: child references of the tag being loaded are not loaded themselves
			// Else, child references are loaded from disk
			_tag_load_non_resolving_references_bit,
		};
	};

	typedef void (PLATFORM_API* proc_tag_data_byte_swap)(void* block_element, void* address, int32 size);
	struct tag_data_definition
	{
		cstring name;
		long_flags flags;
		int32 maximum_size;
		proc_tag_data_byte_swap byte_swap_proc;

		bool IsConsideredDebugOnly() const
		{
			return
				// never streamed is not really debug, but it requires explicit tag_data_load before it is loaded into memory
				TEST_FLAG(flags, Flags::_tag_data_never_streamed_bit) ||
				TEST_FLAG(flags, Flags::_tag_data_not_streamed_to_cache_bit);
		}
	}; BOOST_STATIC_ASSERT( sizeof(tag_data_definition) == 0x10 );

	struct tag_reference_definition
	{
		long_flags flags;
		tag group_tag;
		tag* group_tags;

#if PLATFORM_IS_EDITOR
		struct s_group_tag_iterator
		{
		private:
			tag* m_group_tags;

			bool IsEndHack() const			{ return m_group_tags == nullptr; }

		public:
			s_group_tag_iterator() : m_group_tags(nullptr) {} // for EndHack
			s_group_tag_iterator(const tag_reference_definition* definition) : m_group_tags(definition->group_tags) {}

			bool operator!=(const s_group_tag_iterator& other) const;

			s_group_tag_iterator& operator++()
			{
				++m_group_tags;
				return *this;
			}
			tag& operator*() const
			{
				return *m_group_tags;
			}
		};

		s_group_tag_iterator begin() /*const*/
		{
			auto iter = s_group_tag_iterator(this);
			return iter;
		}
		static const s_group_tag_iterator end() /*const*/
		{
			return s_group_tag_iterator();
		}
#endif
	}; BOOST_STATIC_ASSERT( sizeof(tag_reference_definition) == 0xC );
};