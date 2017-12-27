#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/cseries/enum_templates.h>
#include <blamlib/memory/datum_index.hpp>
#include <blamlib/tag_files/tag_groups_base.hpp>
#include <yelolib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	struct tag_block_definition;

	namespace TagGroups
	{
		struct s_tag_field_set_runtime_data;
	}

	namespace e_tag_group_flags
	{
		typedef enum : long_flags
		{
			// tag_instance of the tag group will have their file_checksum CRC'd into the resulting cache tag header's crc field 
			include_in_tags_checksum_bit,
			// only seen preferences_network_game use this
			unknown1_bit,
			// haven't seen this used at all
			unknown2_bit,
			// majority of tags have this set
			reloadable_bit,

			// YELO ONLY! tag_group is not meant for use in runtime builds
			debug_only_yelo_bit,
			// YELO ONLY! tag_group is non-standard (ie, official). This is applied to those defined in CheApe.map
			// Set at startup.
			non_standard_yelo_bit,
			// YELO ONLY! tag_group is referenced in other groups via a tag_reference or as a parent group
			// doesn't consider tag_references which can reference ANYTHING (eg, tag_collection's field).
			// Set at startup.
			referenced_yelo_bit,

			k_count
		} type_t;

		typedef flags_type<type_t> flags_t;
	}

	// Postprocess a tag definition (eg, automate the creation of fields, etc)
	// Called once the tag has been fully loaded (header_block_definition's postprocess is called before this)
	typedef bool (PLATFORM_API* proc_tag_group_postprocess_t)(
		datum_index tag_index,
		Enums::tag_postprocess_mode mode);

	struct tag_group
	{
		cstring name;
		e_tag_group_flags::flags_t flags;
		tag group_tag;
		tag parent_group_tag;
		int16 version;
		PAD16;
		proc_tag_group_postprocess_t postprocess_proc;
		tag_block_definition* header_block_definition;
		tag child_group_tags[Enums::k_maximum_children_per_tag];
		int16 child_count;
		PAD16;

		#if PLATFORM_IS_EDITOR
		TagGroups::s_tag_field_set_runtime_data* get_header_runtime_info() const;

		bool is_debug_only() const;

		// tag_group* [] (ie, tag_group**) qsort procs
		static int __cdecl compare_by_name_proc(
			void*,
			const tag_group*const* lhs,
			const tag_group*const* rhs);
		static int __cdecl compare_by_group_tag_proc(
			void*,
			const tag_group*const* lhs,
			const tag_group*const* rhs);

		static int __cdecl search_by_name_proc(
			void*,
			cstring key,
			const tag_group*const* group);
		#endif
	};

	BOOST_STATIC_ASSERT(sizeof(tag_group) == 0x60);
}
