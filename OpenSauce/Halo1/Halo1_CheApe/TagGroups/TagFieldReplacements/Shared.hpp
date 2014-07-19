/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace Shared
	{
		static cstring k_reserved_flag_string = "reserved"
			TAG_FIELD_MARKUP_IS_READONLY;

		static cstring k_never_share_resources_flag_string = "never share resources"
			TAG_FIELD_MARKUP_PREFIX_HELP
			"";

		template<Enums::field_type kFieldType>
		static string_list* FindFlagsField(tag_block_definition* definition, cstring flags_field_name = "flags")
		{
			static_assert(kFieldType == Enums::_field_byte_flags || kFieldType == Enums::_field_word_flags || kFieldType == Enums::_field_long_flags,
				"Expected a flags-based field type");
			assert(definition);

			int flags_field_index = definition->FindFieldIndex(kFieldType, flags_field_name);
			assert(flags_field_index != NONE);

			auto flags_list = definition->fields[flags_field_index].Definition<string_list>();

			return flags_list;
		}

		template<size_t kStartIndex, size_t kEndIndex, rsize_t kStringsCount>
		static void PopulateWithReservedString(cstring (& strings)[kStringsCount])
		{
			BOOST_STATIC_ASSERT(kStartIndex < kStringsCount);
			BOOST_STATIC_ASSERT(kEndIndex < kStringsCount);

			for (size_t x = kStartIndex; x <= kEndIndex; x++)
				strings[x] = k_reserved_flag_string;
		}
	};};}
};