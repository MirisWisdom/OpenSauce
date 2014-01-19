/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>
#include <blamlib/Halo1/game/game_configuration.hpp>
#include <blamlib/Halo1/hs/hs_runtime.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_hs_thread_stack_size = 0x200,

			_hs_thread_datum_sleep_forever = NONE,
			_hs_thread_datum_sleep_dormant = NONE - 1,
		};

		enum hs_thread_type : _enum
		{
			_hs_thread_type_script,
			_hs_thread_type_global_initialize, // script global initializer thunk
			_hs_thread_type_runtime_evaluate, // console_command in stock engine
			
			k_number_of_hs_thread_types,
		};
	};

	namespace Flags
	{
		enum {
			_hs_thread_datum_unknown0_bit,
			_hs_thread_datum_sleeping_bit,
		};
	};

	namespace Scripting
	{
		struct s_hs_thread_datum : Memory::s_datum_base
		{
// 32bit packing only for stock code computability.
#include <PshPack4.h>
			struct stack_frame
			{
				stack_frame* previous;
				datum_index source_expression;
				TypeHolder* result_reference;
				uint16 size;
#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
				byte data[];
#pragma warning( pop )

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Test whether the requested allocation would overflow. </summary>
				///
				/// <param name="stack_base_address">	The base address of the actual stack. </param>
				/// <param name="alignment">		 	The number of alignment bytes (if any) before [size]. </param>
				/// <param name="size">				 	The size of the allocation. </param>
				///
				/// <returns>	true if it allocation parameters would cause an overflow, false if they're good. </returns>
				bool AllocationWouldOverflow(const byte* stack_base_address, size_t size, size_t alignment) const;
			}; BOOST_STATIC_ASSERT( sizeof(stack_frame) == 0x10 );
#include <PopPack.h>

			Enums::hs_thread_type type;
			word_flags flags;
			int16 script_index; // only when type==_hs_thread_type_script
			game_ticks_t sleep_time;
			game_ticks_t prev_sleep_time;
			stack_frame* stack_data;
			TypeHolder result;

			byte stack_buffer[Enums::k_hs_thread_stack_size];

			bool ValidThread(const Memory::s_data_array& threads) const;
			cstring GetDescriptionString() const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Allocate memory from the thread's stack for script memory. </summary>
			///
			/// <param name="threads">			The root threads array. </param>
			/// <param name="size">				The size of the allocation. </param>
			/// <param name="alignment_bit">	The alignment for the allocation. </param>
			/// <param name="stack_offset">
			/// 	[out] (Optional) If non-null, (Optional) the stack offset where the allocation begins.
			/// </param>
			///
			/// <returns>	null if it fails, else a pointer to the requested memory. </returns>
			void* StackAllocate(const Memory::s_data_array& threads,
				size_t size, long_flags alignment_bit, _Out_opt_ int16* stack_offset = nullptr);

		}; BOOST_STATIC_ASSERT( sizeof(s_hs_thread_datum) == 0x218 );


		struct s_hs_globals_datum : Memory::s_datum_base
		{
			word_flags yelo_flags;

			TypeHolder value;
		}; BOOST_STATIC_ASSERT( sizeof(s_hs_globals_datum) == 0x8 );
	};
};