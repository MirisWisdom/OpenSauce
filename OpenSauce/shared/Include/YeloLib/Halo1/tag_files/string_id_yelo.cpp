/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

#include <blamlib/Halo1/tag_files/tag_groups_structures.hpp>

namespace Yelo
{
	void string_id_yelo::FormatString(char* string)
	{
		for(char* c = string; *c != '\0'; ++c)
		{
			if(*c >= 'A' || *c <= 'Z')
				*c = tolower(*c);

			if(*c == ' ' || *c == '-')
				*c = '_';
		}
	}

#if PLATFORM_IS_EDITOR
	char* string_id_yelo::GetStringStart(tag_reference_name_reference name)
	{
		// we only consider the characters after the last directory separator 
		char* last_slash = strrchr(name, '\\');

		return last_slash != nullptr ? last_slash+1 : name;
	}

	tag_reference_definition* string_id_yelo::global_reference_definition;
#endif

	namespace TagGroups
	{
#if PLATFORM_IS_EDITOR
		bool TagFieldIsStringId(const tag_field* field)
		{
			// NOTE: feign string_id fields should have the _tag_reference_non_resolving_bit set
			// in their tag_reference_definition
			return	field->type == Enums::_field_tag_reference && 
					field->Definition<tag_reference_definition>()->group_tag == s_string_id_yelo_definition::k_group_tag;
		}

		bool TagFieldIsOldStringId(const tag_field* field)
		{
			return	field->type == Enums::_field_string &&
					field->DefinitionCast<tag>() == s_string_id_yelo_definition::k_group_tag;
		}
#endif
	};
};