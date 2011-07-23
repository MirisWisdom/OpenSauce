/*
    Yelo: Open Sauce SDK
		Halo 2 Edition

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <TagGroups/TagGroupDefinitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum predicted_resource : _enum {
			_predicted_resource_bitmap,
			_predicted_resource_sound,

			// TODO: Halo 2 defines more types of predicted resources (tho, not all of them are actually put to use)

			_predicted_resource
		};
	};

	namespace TagGroups
	{
		struct predicted_resource
		{
			_enum type;
			// index relative to the tag
			int16 resource_index;
			// index relative to the tag_index
			datum_index tag_index;
		};

		struct reference_block { tag_reference Reference; };

		struct real_rgb_color_block { real_rgb_color Color; };
	};
};