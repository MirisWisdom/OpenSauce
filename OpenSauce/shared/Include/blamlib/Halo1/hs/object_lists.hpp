/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>

namespace Yelo
{
	namespace Scripting
	{
		struct s_object_list_header_datum : Memory::s_datum_base_aligned
		{
			int16 reference_count;	// how many references there are to this list
			int16 count;			// how many objects are in the list
			datum_index first;		// handle to the first list object reference
		}; BOOST_STATIC_ASSERT( sizeof(s_object_list_header_datum) == 0xC );
		typedef Memory::DataArray<s_object_list_header_datum, 48> object_list_header_data_t;


		struct s_list_object_reference_datum : Memory::s_datum_base_aligned
		{
			datum_index object_index;	// handle to the object datum
			datum_index next_reference;	// handle to the next list object reference in the list
		}; BOOST_STATIC_ASSERT( sizeof(s_list_object_reference_datum) == 0xC );
		typedef Memory::DataArray<s_list_object_reference_datum, 128> list_object_reference_data_t;
	};

	namespace blam
	{
		datum_index object_list_new();

		// Add [object_index] to [object_list_index]
		void PLATFORM_API object_list_add(datum_index object_list_index, datum_index object_index);

		// Get the object_index of the first object in a list of objects
		// [list_reference] - handle to the object_list's first s_list_object_reference_datum
		datum_index object_list_get_first(datum_index list_index, __out datum_index& list_reference);

		// Get the next object_index in a list of objects
		// [list_reference]:
		// __in  - handle to the current s_list_object_reference_datum in the object_list we're referencing
		// __out - handle to the next s_list_object_reference_datum
		datum_index object_list_get_next(datum_index list_index, __inout datum_index& list_reference);


		Scripting::s_object_list_header_datum* object_list_get(datum_index object_list_index);

		Scripting::s_list_object_reference_datum* object_list_object_get(datum_index list_object_reference_index);
	};
};