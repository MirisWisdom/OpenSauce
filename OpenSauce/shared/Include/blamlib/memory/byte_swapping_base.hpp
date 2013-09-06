/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	typedef int32 byte_swap_code_t;

	namespace Enums
	{
		enum {
			k_byte_swap_signature = 'bysw',
		};

		enum /*e_bs_code*/ : byte_swap_code_t
		{
			_bs_code_array_end = -101,
			_bs_code_array_start = -100,

			_bs_code_8byte = 0 - (byte_swap_code_t)sizeof(uint64),
			_bs_code_4byte = 0 - (byte_swap_code_t)sizeof(uint32),
			_bs_code_2byte = 0 - (byte_swap_code_t)sizeof(uint16),

			_bs_code_1byte = sizeof(byte),
		};
	};

	namespace Memory
	{
		struct s_byte_swap_definition
		{
			cstring name;
			size_t size;
			byte_swap_code_t* codes;
			tag signature;
			bool initialized;
			PAD24;
		}; BOOST_STATIC_ASSERT(sizeof(s_byte_swap_definition) == 0x14);
	};
};