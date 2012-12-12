/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>
#include <blamlib/Halo1/hs/hs.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_hs_thread_stack_size = 0x200,

			k_maximum_number_of_hs_globals = 1024,
		};

		enum hs_thread_type : _enum
		{
			_hs_thread_type_script,
			_hs_thread_type_global_initialize, // script global initializer thunk
			_hs_thread_type_console_command,
			
			k_number_of_hs_thread_types,
		};
	};

	namespace Scripting
	{
		struct s_hs_thread_datum : Memory::s_datum_base
		{
			struct stack_frame
			{
				stack_frame* previous;
				datum_index source;
				void* result_reference;
				uint16 size;
				uint16 offset;
			};

			Enums::hs_thread_type type;
			word_flags flags;
			int16 type_index; // or NONE if type doesn't require an index
			uint32 sleep_until;
			UNKNOWN_TYPE(uint32); // game time related
			stack_frame* stack_data;
			TypeHolder result;

			byte stack_buffer[Enums::k_hs_thread_stack_size];
		}; BOOST_STATIC_ASSERT( sizeof(s_hs_thread_datum) == 0x218 );
		typedef Memory::DataArray<s_hs_thread_datum, 256> t_hs_thread_data;


		struct s_hs_globals_datum : Memory::s_datum_base_aligned
		{
			union {
				void* address;

				union {
					bool _bool;
					int16 _short;
					int32 _long;
					real _real;
					datum_index _datum;
					char* _string;
				}Value;
			};
		}; BOOST_STATIC_ASSERT( sizeof(s_hs_globals_datum) == 0x8 );
		typedef Memory::DataArray<	s_hs_globals_datum, 
									Enums::k_maximum_number_of_hs_globals> 
			t_hs_globals_data;
	};
};