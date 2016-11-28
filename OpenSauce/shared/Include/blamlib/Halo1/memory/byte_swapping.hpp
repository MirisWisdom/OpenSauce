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
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Byte swap 1 or more structures at a given address. </summary>
		/// <remarks>	Initializes the definition if it hasn't been already. </remarks>
		///
		/// <param name="definition">	The structure byte swap definition. </param>
		/// <param name="address">   	Memory address of the structure instances. </param>
		/// <param name="data_count">	Number of structure instances at the address. </param>
		void PLATFORM_API byte_swap_data(Memory::s_byte_swap_definition* definition, void* address, 
			int32 data_count = 1);

		void PLATFORM_API byte_swap_data_explicit(cstring name, int32 size, byte_swap_code_t* codes,
			int data_count = 1, void *address = nullptr);
	};
};