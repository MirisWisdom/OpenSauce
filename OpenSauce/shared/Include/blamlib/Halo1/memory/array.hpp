/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Memory
	{
		struct s_dynamic_array
		{
			size_t element_size;
			int32 count;
			void* elements;
		}; BOOST_STATIC_ASSERT( sizeof(s_dynamic_array) == 0xC );
	};
};

#include <YeloLib/memory/array_yelo.hpp>