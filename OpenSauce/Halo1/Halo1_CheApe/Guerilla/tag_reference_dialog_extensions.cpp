/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Guerilla/tag_reference_dialog_extensions.hpp"

#if PLATFORM_TYPE == PLATFORM_GUERILLA
#include <BlamLib/Halo1/tag_files/tag_files.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Guerilla
	{
		HBRUSH tag_reference_dialog_extensions::OnCtlColor__field_name(void* CDC_, void* CWnd_, field_information* field_info,
			HBRUSH brush)
		{
			auto* reference = CAST_PTR(tag_reference*, field_info->address);
			auto* definition = field_info->field->Definition<tag_reference_definition>();

			if (definition->group_tag == TagGroups::s_string_id_yelo_definition::k_group_tag)
				OnCtlColor__string_id_field_name(CDC_);

			return brush;
		}

		HBRUSH tag_reference_dialog_extensions::OnCtlColor__tag_name(void* CDC_, void* CWnd_, field_information* field_info,
			HBRUSH brush)
		{
			auto* definition = field_info->field->Definition<tag_reference_definition>();
			COLORREF color = 0;

			if (definition->group_tag == TagGroups::s_string_id_yelo_definition::k_group_tag)
				OnCtlColor__string_id_field_name(CDC_);

			tag_reference* reference = CAST_PTR(tag_reference*, field_info->address);
			if (reference == nullptr || !blam::tag_file_exists(reference->group_tag, reference->name))
			{
				// Set the tag-name control text to red
				CDC__SetBkColor(CDC_, RGB(0xFF, 0x00, 0x00) | color);
			}

			return brush;
		}
	};
};
#endif