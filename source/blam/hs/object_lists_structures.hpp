/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blam/hs/object_lists.hpp>

namespace Yelo
{
	namespace Scripting
	{
		struct s_object_list_header_datum : Memory::s_datum_base_aligned
		{
			int16 reference_count;	// how many references there are to this list
			int16 count;			// how many objects are in the list
			datum_index first;		// handle to the first list object reference
		}; ASSERT_SIZE(s_object_list_header_datum, 0xC);

		struct s_list_object_reference_datum : Memory::s_datum_base_aligned
		{
			datum_index object_index;	// handle to the object datum
			datum_index next_reference;	// handle to the next list object reference in the list
		}; ASSERT_SIZE(s_list_object_reference_datum, 0xC);
	};
};