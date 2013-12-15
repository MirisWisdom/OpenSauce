/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/hs/hs_runtime.hpp>

#include <blamlib/Halo1/scenario/scenario.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

namespace Yelo
{
	namespace Scripting
	{
		bool s_hs_thread_datum::ValidThread(const Memory::s_data_array& threads) const
		{
			const byte* base_address = CAST_PTR(const byte*, threads.base_address);

			// validate the thread is within the bounds of the data array
			if (CAST(const void*, this) < base_address)
				return false;
			else if (CAST(const void*, this) > base_address + (threads.last_datum*threads.datum_size))
				return false;

			// validate the stack pointer is within the thread's stack buffer
			else if (CAST(const void*, stack_data) < &stack_buffer[0])
				return false;
			else if (CAST(const void*, stack_data) > this+1)
				return false;

			// validate the stack hasn't overflowed
			else if (&stack_buffer[stack_data->offset+stack_data->size] > CAST(const void*, this+1))
				return false;

			return true;
		}

		cstring s_hs_thread_datum::GetDescriptionString() const
		{
			switch (type)
			{
			case Enums::_hs_thread_type_script:
				return Scenario::Scenario()->scripts[script_index].name;

			case Enums::_hs_thread_type_global_initialize:
				return "[global initialize]";

			case Enums::_hs_thread_type_runtime_evaluate:
				return "[runtime evaluate]";

			default:
				return "[unknown thread type]";
			}
		}
	};
};