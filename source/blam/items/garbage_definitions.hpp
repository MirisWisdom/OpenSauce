/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blam/items/item_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct _garbage_definition
		{
			TAG_PAD(int32, 42);
		}; ASSERT_SIZE(_garbage_definition, 0xA8);

		struct s_garbage_definition : s_item_definition
		{
			enum { k_group_tag = 'garb' };

			_garbage_definition garbage;
		};
	};
};