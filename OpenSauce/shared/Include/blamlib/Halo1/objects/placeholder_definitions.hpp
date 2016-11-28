/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		// NOTE: if we're going to use the placeholder type for our own deeds,
		// Sapien's code will need to be changed, specifically editor_render.
		// Change the group tag at 48F8F4 to something else, and change the
		// object tags in ui/editor/markers/ to that object definition
		// We'll obviously need to ship (installer) with those new object defs too

		struct _placeholder_definition
		{
			TAG_PAD(int32, 32);
		}; BOOST_STATIC_ASSERT( sizeof(_placeholder_definition) == 0x80 );

		struct s_placeholder_definition : s_object_definition
		{
			enum { k_group_tag = 'plac' };

			_placeholder_definition placeholder;
		};
	};
};