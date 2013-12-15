/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>
#include <blamlib/Halo1/game/game_configuration.hpp>
#include <blamlib/Halo1/hs/hs.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_hs_thread_stack_size = 0x200,

			k_maximum_number_of_hs_globals = 1024,

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
			struct stack_frame
			{
				stack_frame* previous;
				datum_index source;
				TypeHolder* result_reference;
				uint16 size;
				uint16 offset;
			}; BOOST_STATIC_ASSERT( sizeof(stack_frame) == 0x10 );

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

		}; BOOST_STATIC_ASSERT( sizeof(s_hs_thread_datum) == 0x218 );
		typedef Memory::DataArray<	s_hs_thread_datum, 
									256>
			hs_thread_data_t;


		struct s_hs_globals_datum : Memory::s_datum_base
		{
			word_flags yelo_flags;

			TypeHolder value;
		}; BOOST_STATIC_ASSERT( sizeof(s_hs_globals_datum) == 0x8 );
		typedef Memory::DataArray<	s_hs_globals_datum, 
									Enums::k_maximum_number_of_hs_globals> 
			hs_globals_data_t;
	};

	namespace blam
	{
		Scripting::s_hs_thread_datum* hs_thread_get(datum_index thread_index);
	};
};