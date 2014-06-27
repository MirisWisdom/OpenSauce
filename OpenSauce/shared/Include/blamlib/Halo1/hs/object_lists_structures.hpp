/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>
#include <blamlib/Halo1/hs/object_lists.hpp>

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

		struct s_list_object_reference_datum : Memory::s_datum_base_aligned
		{
			datum_index object_index;	// handle to the object datum
			datum_index next_reference;	// handle to the next list object reference in the list
		}; BOOST_STATIC_ASSERT( sizeof(s_list_object_reference_datum) == 0xC );

		typedef Memory::DataArray<	s_object_list_header_datum, 
									48>
			object_list_header_data_t;

		typedef Memory::DataArray<	s_list_object_reference_datum, 
									128>
			list_object_reference_data_t;

		object_list_header_data_t&		ObjectListHeader();
		list_object_reference_data_t&	ListObjectReference();
	};
};