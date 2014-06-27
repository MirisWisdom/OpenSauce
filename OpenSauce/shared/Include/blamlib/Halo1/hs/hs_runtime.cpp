/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/hs/hs_runtime.hpp>

#include <blamlib/Halo1/game/game_time.hpp>
#include <blamlib/Halo1/hs/hs_structures.hpp>
#include <blamlib/Halo1/hs/hs_runtime_structures.hpp>
#include <blamlib/Halo1/hs/object_lists.hpp>
#include <blamlib/Halo1/main/editor.hpp>
#include <blamlib/Halo1/scenario/scenario.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

#define YELO_HS_RUNTIME_ASSERT(expression, thread, explanation)										\
	YELO_ASSERT_DISPLAY(expression, "a problem occurred while executing the script %s: %s (%s)",	\
		thread->GetDescriptionString(), explanation, #expression)
#define YELO_HS_THREAD_VALID_STACK(thread)															\
	YELO_HS_RUNTIME_ASSERT(thread->ValidThread(Scripting::HsThreads().Header), thread,				\
		"corrupted stack.")

#include <blamlib/Halo1/hs/hs_library_internal_runtime.inl>

namespace Yelo
{
	namespace blam
	{
		using namespace Scripting;

		static void hs_runtime_garbage_collect_nodes()
		{
			// only try to gc twice a second
			const auto k_update_rate = K_TICKS_PER_SECOND / 2;

			auto game_time = blam::game_time_get();

			if (game_time / k_update_rate)
				hs_nodes_garbage_collect();
		}

		static TypeHolder hs_evaluate(datum_index thread_index, datum_index expression_index, TypeHolder& destination)
		{
			s_hs_thread_datum* thread = hs_thread_get(thread_index);
			hs_syntax_node* expression = hs_syntax_get(expression_index);

			YELO_HS_THREAD_VALID_STACK(thread);

			// TODO: this is a WIP
			if (TEST_FLAG(expression->flags, Flags::_hs_syntax_node_primitive_bit))
			{
				if (TEST_FLAG(expression->flags, Flags::_hs_syntax_node_global_index_bit))
				{
					//hs_global_evaluate(expression->value.int16)
				}
				else
				{

				}
			}
			return destination;
		}

		static void hs_thread_main(datum_index thread_index)
		{
			auto hs_runtime_globals = HsRuntimeGlobals();
			hs_runtime_globals->executing_thread_index = thread_index.index;

			s_hs_thread_datum* thread = hs_thread_get(thread_index);

			const TagGroups::hs_script* script = nullptr;
			if (thread->type == Enums::_hs_thread_type_script)
			{
				script = &(Scenario::Scenario()->scripts[thread->script_index]);
				const auto& _script = script->script;

				YELO_HS_RUNTIME_ASSERT(_script.type!=Enums::_hs_script_static && _script.type!=Enums::_hs_script_stub, 
					thread,
					"found a static script at toplevel.");
			}

			YELO_HS_THREAD_VALID_STACK(thread);
			thread->sleep_until = 0;

			if (CAST_PTR(const void*,thread->stack_data) == thread->stack_buffer)
			{
				YELO_ASSERT(script);

				thread->stack_data->size = 0;
				void** result = thread->StackAllocate<void*>(HsThreads().Header);

				//hs_evaluate(thread_index, script->script.root_expression_index, result);
			}
			else
			{
			}
		}

		void hs_runtime_update()
		{
			auto hs_runtime_globals = HsRuntimeGlobals();
			if (!hs_runtime_globals->enabled)
				return;

			auto game_time = blam::game_time_get();
			bool gc_nodes = false;

			// NOTE: engine seems to use a for(;;) using data_next_index() on threads
			for (auto thread : HsThreads())
			{
				// execute all threads related to console commands
				if (thread->type == Enums::_hs_thread_type_runtime_evaluate)
				{
					gc_nodes = true;
				}
				// by default, the editor doesn't run game scripts
				else if (!hs_runtime_globals->run_game_scripts || game_in_editor())
					continue;
					
				if (thread->sleep_until >= 0 && thread->sleep_until <= game_time)
				{
					hs_thread_main(thread.index);
				}

				if (!hs_runtime_globals->enabled)
					break;
			}

			object_lists_garbage_collect();
			if (gc_nodes)
				hs_runtime_garbage_collect_nodes();
		}

		void hs_return(datum_index thread_index, TypeHolder value)
		{
			s_hs_thread_datum* thread = hs_thread_get(thread_index);
			// engine did this after it the hs_syntax_get call, but that makes no sense!
			YELO_HS_THREAD_VALID_STACK(thread);

			hs_syntax_node* call_expression = hs_syntax_get(thread->stack_data->source_expression);

			Enums::hs_type actual_type;
			if (TEST_FLAG(call_expression->flags, Flags::_hs_syntax_node_script_index_bit))
			{
				const auto& script = Scenario::Scenario()->scripts[call_expression->script_index];
				actual_type = script.script.return_type;
			}
			else // hs_function call
			{
				const hs_function_definition* function = hs_function_get(call_expression->function_index);
				actual_type = function->return_type;
			}

			*thread->stack_data->previous->result_reference =
				hs_cast(thread_index, actual_type, call_expression->type, value);

			thread->StackPop();
		}

		static bool hs_object_type_can_cast(Enums::hs_object_type actual_type, Enums::hs_object_type desired_type)
		{
			YELO_ASSERT(actual_type >= 0 && actual_type < Enums::k_number_of_hs_object_types);
			YELO_ASSERT(desired_type >= 0 && desired_type < Enums::k_number_of_hs_object_types);

			auto actual_type_mask = k_hs_object_type_masks[actual_type];
			auto desired_type_mask = k_hs_object_type_masks[desired_type];

			return (actual_type_mask & desired_type) == desired_type;
		}
		bool hs_can_cast(Enums::hs_type actual_type, Enums::hs_type desired_type)
		{
			YELO_ASSERT(actual_type==Enums::_hs_passthrough || hs_type_valid(actual_type));
			YELO_ASSERT(hs_type_valid(desired_type));

			if (actual_type == Enums::_hs_passthrough || actual_type == desired_type)
				return true;
			// desired object is an object reference
			else if (hs_type_is_object(desired_type))
			{
				auto desired_object_type = CAST(Enums::hs_object_type, desired_type - Enums::_hs_type_object__first);

				// can the desired object type be casted to the actual object type?
				if (hs_type_is_object(actual_type))
				{
					auto actual_object_type = CAST(Enums::hs_object_type, actual_type - Enums::_hs_type_object__first);

					return hs_object_type_can_cast(actual_object_type, desired_object_type);
				}
				// can the desired object type be casted to the actual object type via an object name?
				else if (hs_type_is_object_name(actual_type))
				{
					auto actual_object_type = CAST(Enums::hs_object_type, actual_type - Enums::_hs_type_object_name__first);

					return hs_object_type_can_cast(actual_object_type, desired_object_type);
				}

				// no other means of converting an hs object
				return false;
			}
			// desired type is an object name
			else if (hs_type_is_object_name(desired_type))
			{
				auto desired_object_type = CAST(Enums::hs_object_type, desired_type - Enums::_hs_type_object_name__first);

				// can the desired named object type be casted to the actual object type via an object name?
				if (hs_type_is_object_name(actual_type))
				{
					auto actual_object_type = CAST(Enums::hs_object_type, actual_type - Enums::_hs_type_object_name__first);

					return hs_object_type_can_cast(actual_object_type, desired_object_type);
				}

				// no other means of converting an hs object
				return false;
			}

			return k_hs_typecasting_procedures[actual_type][desired_type] != nullptr;
		}

		TypeHolder hs_cast(datum_index thread_index, Enums::hs_type actual_type, Enums::hs_type desired_type, TypeHolder value)
		{
			YELO_HS_RUNTIME_ASSERT(hs_can_cast(actual_type, desired_type),
				hs_thread_get(thread_index),
				"bad typecast.");

			if (actual_type == desired_type || actual_type == Enums::_hs_passthrough)
				return value;
			else if (!hs_type_is_object_name(desired_type) && 
					hs_type_is_object(desired_type) && hs_type_is_object_name(actual_type))
			{
				value.datum = object_index_from_name_index(value.int16);
				return value;
			}

			return k_hs_typecasting_procedures[actual_type][desired_type](value);
		}
	};

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

		void s_hs_thread_datum::StackPop()
		{
			stack_data = stack_data->previous;
		}

		void* s_hs_thread_datum::StackAllocate(const Memory::s_data_array& threads,
			size_t size, long_flags alignment_bit, _Out_opt_ int16* stack_offset)
		{
			YELO_HS_THREAD_VALID_STACK(this);
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
#undef YELO_HS_THREAD_VALID_STACK