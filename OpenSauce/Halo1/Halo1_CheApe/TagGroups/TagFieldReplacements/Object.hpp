/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_definitions.hpp>

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace Object
	{
		static void ObjectGroupUpdateFlags(tag_block_definition* object_block)
		{
			auto* flags_list = Shared::FindFlagsField<Enums::_field_word_flags>(object_block);
			assert(flags_list->count==Flags::k_number_of_object_definition_flags);

			static cstring object_definition_flags_yelo_strings[Flags::k_number_of_object_definition_flags_yelo];
			for (int x = 0; x < Flags::k_number_of_object_definition_flags; x++)
				object_definition_flags_yelo_strings[x] = flags_list->strings[x];

			object_definition_flags_yelo_strings[Flags::_object_definition_cast_shadow_by_default_yelo_bit] =
				"cast shadow by default";

			flags_list->strings = object_definition_flags_yelo_strings;
			flags_list->count = NUMBEROF(object_definition_flags_yelo_strings);
		}

		static void Initialize()
		{
			tag_group* group = blam::tag_group_get<TagGroups::s_object_definition>();
			assert(group);

			ObjectGroupUpdateFlags(group->header_block_definition);
		}
	};};};
};