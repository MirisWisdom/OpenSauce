/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once
#include <blamlib/memory/byte_swapping_base.hpp>

namespace Yelo
{
	namespace blam
	{
		void PLATFORM_API byte_swap_data_explicit(cstring name, int32 size, 
			byte_swap_code_t* codes, int data_count, void *address);
	};
};