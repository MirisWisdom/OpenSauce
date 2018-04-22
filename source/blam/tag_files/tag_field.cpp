#include <blam/blam.h>
#include <blam/tag_files/tag_field.h>

#include <yelo/tag_files/string_id_yelo.hpp>
#include <blam/tag_files/tag_groups.hpp>

namespace Yelo
{
	#if PLATFORM_IS_EDITOR
	enum
	{
		k_maximum_field_byte_swap_codes = 1024,

		// these chars should all match the TAG_FIELD_MARKUP_* defines in tag_groups_structures_macros.hpp

		k_tag_field_markup_character_advanced = '!',
		k_tag_field_markup_character_help_prefix = '#',
		k_tag_field_markup_character_read_only = '*',
		k_tag_field_markup_character_units_prefix = ':',
		k_tag_field_markup_character_block_name = '^',
	};

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
				field_size = static_cast<int32>(TagGroups::k_tag_field_definitions[e_field_type::tag_reference].size);

				if (runtime_size && TagGroups::TagFieldIsStringId(this))
				{
					*runtime_size = field_size - string_id_yelo::k_debug_data_size;
				}
				break;

			case e_field_type::pad:
			case e_field_type::skip:
				field_size = get_definition_as<int32>();
				break;

			default:
				field_size = TagGroups::k_tag_field_definitions[type].size;
				break;
		}

		return field_size;
	}

	bool tag_field::is_read_only() const
	{
		return name && strchr(name, k_tag_field_markup_character_read_only); // NOTE: engine uses strrchr
	}

	bool tag_field::is_advanced() const
	{
		return name && strchr(name, k_tag_field_markup_character_advanced); // NOTE: engine uses strrchr
	}

	bool tag_field::is_block_name() const
	{
		return name && strchr(name, k_tag_field_markup_character_block_name); // NOTE: engine uses strrchr
	}

	bool tag_field::is_invisible() const
	{
		return name && *name == '\0'; // yes, a field with no name wouldn't be considered 'invisible', according to engine code
	}
	#endif
}
