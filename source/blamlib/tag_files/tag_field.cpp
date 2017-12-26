#include <Common/Precompile.hpp>
#include <blamlib/tag_files/tag_field.h>

#include <blamlib/tag_files/tag_groups.hpp>
#include <YeloLib/tag_files/string_id_yelo.hpp>

namespace Yelo
{
	#if PLATFORM_IS_EDITOR
	size_t tag_field::get_size(
		size_t* const runtime_size) const
	{
		size_t field_size;

		switch (type)
		{
			case e_field_type::string:
				field_size = TagGroups::string_field_get_size(this);
				break;

			case e_field_type::tag_reference:
				field_size = CAST(int, TagGroups::k_tag_field_definitions[e_field_type::tag_reference].size);

				if (runtime_size && TagGroups::TagFieldIsStringId(this))
				{
					*runtime_size = field_size - string_id_yelo::k_debug_data_size;
				}
				break;

			case e_field_type::pad:
			case e_field_type::skip:
				field_size = DefinitionCast<int32>();
				break;

			default:
				field_size = TagGroups::k_tag_field_definitions[type].size;
				break;
		}

		return field_size;
	}

	bool tag_field::is_read_only() const
	{
		return name && strchr(name, Enums::k_tag_field_markup_character_read_only); // NOTE: engine uses strrchr
	}

	bool tag_field::is_advanced() const
	{
		return name && strchr(name, Enums::k_tag_field_markup_character_advanced); // NOTE: engine uses strrchr
	}

	bool tag_field::is_block_name() const
	{
		return name && strchr(name, Enums::k_tag_field_markup_character_block_name); // NOTE: engine uses strrchr
	}

	bool tag_field::is_invisible() const
	{
		return name && *name == '\0'; // yes, a field with no name wouldn't be considered 'invisible', according to engine code
	}
	#endif
}
