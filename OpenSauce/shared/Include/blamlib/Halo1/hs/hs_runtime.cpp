/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/hs/hs_runtime.hpp>

#include <blamlib/Halo1/hs/hs_runtime_structures.hpp>
#include <blamlib/Halo1/scenario/scenario.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

#define YELO_HS_RUNTIME_ASSERT(expression, thread, explanation)										\
	YELO_ASSERT_DISPLAY(expression, "a problem occurred while executing the script %s: %s (%s)",	\
		thread->GetDescriptionString(), explanation, #expression)

namespace Yelo
{
	namespace Scripting
	{
		bool s_hs_thread_datum::stack_frame::AllocationWouldOverflow(const byte* stack_base_address, size_t size, size_t alignment) const
		{
			return	(this->data + this->size + alignment + size) >
					(stack_base_address + Enums::k_hs_thread_stack_size);
		}

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
			else if (stack_data->data+stack_data->size > CAST(const void*, this+1))
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

		void* s_hs_thread_datum::StackAllocate(const Memory::s_data_array& threads,
			size_t size, long_flags alignment_bit, _Out_opt_ int16* stack_offset)
		{
			YELO_HS_RUNTIME_ASSERT(ValidThread(threads), this,
				"corrupted stack.");
			YELO_HS_RUNTIME_ASSERT(size, this,
				"attempt to allocate zero space from the stack.");

			// TODO: calc alignment of stack
			size_t alignment = 0;

			YELO_HS_RUNTIME_ASSERT(stack_data->AllocationWouldOverflow(stack_buffer, size, alignment), this,
				"stack overflow.");

			// TODO: ZeroMemory alignment bytes

			// TODO: populate the start offset
			if (stack_offset)
				*stack_offset = NONE;

			return nullptr;
		}
	};
};

#undef YELO_HS_RUNTIME_ASSERT