/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/objects/object_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct _sound_scenery_definition
		{
			TAG_PAD(int32, 32);
		}; ASSERT_SIZE(_sound_scenery_definition, 0x80);

		struct s_sound_scenery_definition : s_object_definition
		{
			enum { k_group_tag = 'ssce' };

			_sound_scenery_definition sound_scenery;
		};
	};
};