/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/hs/hs_threads.hpp>

#include <blamlib/Halo1/game/game_time.hpp>
#include <blamlib/Halo1/hs/hs_library_internal.hpp>
#include <blamlib/Halo1/hs/hs_structures.hpp>
#include <blamlib/Halo1/hs/hs_runtime_structures.hpp>
#include <blamlib/Halo1/scenario/scenario.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

namespace Yelo
{
	namespace Scripting
	{
		void s_hs_thread_datum::s_stack_frame::Initialize()
		{
			previous = nullptr;
			source_expression = datum_index::null;
			result_reference = nullptr;
			size = 0;
		}

		bool s_hs_thread_datum::s_stack_frame::AllocationWouldOverflow(const byte* stack_base_address, size_t size, size_t alignment) const
		{
			return	(this->data + this->size + alignment + size) >
					(stack_base_address + Enums::k_hs_thread_stack_size);
		}

		void s_hs_thread_datum::Initialize(Enums::hs_thread_type type)
		{
			this->type = type;
			this->flags = 0;
			this->script_index = NONE;
			this->sleep_until = this->prev_sleep_state = 0;

			this->stack_frame = CAST_PTR(s_stack_frame*, this->stack_data);
			this->stack_frame->Initialize();

			result = k_none_value_union;
		}
		void s_hs_thread_datum::InitializeForScript(hs_script_index_t script)
		{
			// TODO: update code when containers come online
			
			auto* scenario = blam::global_scenario_get();
			const auto& script_definition = scenario->scripts[script.script_index].script;

			this->script_index = script.script_index;

			if (script_definition.type == Enums::_hs_script_dormant)
				this->sleep_until = Enums::_hs_thread_datum_sleep_dormant;
			else
				this->sleep_until = 0;
		}

		const hs_syntax_node* s_hs_thread_datum::GetSyntax(datum_index expression_index) const
		{
			// TODO: whenever we support hsc containers, this will abstract which container's nodes are to be referenced
			return blam::hs_syntax_get(expression_index);
		}
		const hs_syntax_node* s_hs_thread_datum::GetFrameSyntax() const
		{
			YELO_ASSERT(!StackIsEmpty());

			return GetSyntax(stack_frame->source_expression);
		}
		const hs_syntax_node* s_hs_thread_datum::TryGetFrameSyntax() const
		{
			return !stack_frame->source_expression.IsNull()
				? GetSyntax(stack_frame->source_expression)
				: nullptr;
		}
		const hs_syntax_node* s_hs_thread_datum::TryGetPrevFrameSyntax() const
		{
			if (stack_frame->previous == nullptr)
				return nullptr;

			auto expression_index = stack_frame->previous->source_expression;
			if (expression_index.IsNull())
				return nullptr;

			return GetSyntax(expression_index);
		}

		bool s_hs_thread_datum::ScriptError(cstring reason, cstring expression) const
		{
			YELO_ERROR_FAILURE("script %s needs to be recompiled. (%s: %s)",
				GetDescriptionString(),
				reason != nullptr ? reason : "no reason given",
				expression);

			return false;
		}

		bool s_hs_thread_datum::StackIsEmpty() const
		{
			return CAST_PTR(const void*, stack_frame) == stack_data;
		}
		bool s_hs_thread_datum::NotKilledOrDormant() const
		{
			return sleep_until > Enums::_hs_thread_datum_sleep_forever;
		}
		bool s_hs_thread_datum::NotSleeping(game_ticks_t relative_to_game_time) const
		{
			return sleep_until <= relative_to_game_time;
		}
		bool s_hs_thread_datum::NotSleeping() const
		{
			return NotSleeping(blam::game_time_get());
		}

		void s_hs_thread_datum::Wake()
		{
			if (sleep_until == Enums::_hs_thread_datum_sleep_forever)
				return;

			sleep_until = 0;

			if (TEST_FLAG(flags, Flags::_hs_thread_datum_sleeping_bit))
			{
				SET_FLAG(flags, Flags::_hs_thread_datum_sleeping_bit, false);
				sleep_until = prev_sleep_state;
				return;
			}

			auto* call_expression = TryGetFrameSyntax();
			if (call_expression != nullptr &&
				call_expression->function_index == Enums::_hs_function_sleep_until)
			{
				StackPop();
				return;
			}

			// IDK what case this is suppose to handle
			call_expression = TryGetPrevFrameSyntax();
			if (call_expression != nullptr &&
				call_expression->function_index == Enums::_hs_function_sleep_until)
			{
				StackPop();
				StackPop();
				SET_FLAG(flags, Flags::_hs_thread_datum_has_new_stack_frame_bit, false);
			}
		}

		bool s_hs_thread_datum::ValidThread() const
		{
			const Memory::s_data_array& threads = HsThreads().Header;

			const byte* base_address = CAST_PTR(const byte*, threads.base_address);

			// validate the thread is within the bounds of the data array
			if (CAST(const void*, this) < base_address)
				return false;
			else if (CAST(const void*, this) > base_address + (threads.last_datum*threads.datum_size))
				return false;

			// validate the stack pointer is within the thread's stack buffer
			else if (CAST(const void*, stack_frame) < &stack_data[0])
				return false;
			else if (CAST(const void*, stack_frame) > this+1)
				return false;

			// validate the stack hasn't overflowed
			else if (stack_frame->data+stack_frame->size > CAST(const void*, this+1))
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

		void s_hs_thread_datum::StackPush()
		{
			auto* new_frame = stack_frame+1;
			YELO_HS_RUNTIME_ASSERT(CAST_PTR(byte*,new_frame+1)<stack_data+Enums::k_hs_thread_stack_size,
				this,
				"stack overflow.");

			new_frame->previous = stack_frame;
			new_frame->size = 0;
			this->stack_frame = new_frame;
		}
		void s_hs_thread_datum::StackPop()
		{
			stack_frame = stack_frame->previous;
		}

		void* s_hs_thread_datum::StackAllocate(
			size_t size, long_flags alignment_bit, _Out_opt_ int16* stack_offset)
		{
			YELO_HS_THREAD_VALID_STACK(this);
			YELO_HS_RUNTIME_ASSERT(size, this,
				"attempt to allocate zero space from the stack.");

			// TODO: calc alignment of stack
			size_t alignment = 0;

			YELO_HS_RUNTIME_ASSERT(stack_frame->AllocationWouldOverflow(stack_data, size, alignment), this,
				"stack overflow.");

			// TODO: ZeroMemory alignment bytes

			// TODO: populate the start offset
			if (stack_offset)
				*stack_offset = NONE;

			return nullptr;
		}

		void s_hs_thread_datum::MainPrologue(const s_main_state& state)
		{
			this->sleep_until = 0;

			if (this->StackIsEmpty())
			{
				YELO_ASSERT(state.script);

				this->stack_frame->size = 0;
				auto* result = this->StackAllocate<s_hs_value_union>();

				blam::hs_evaluate(state.thread_index, state.script->script.root_expression_index, result);
			}
		}
		void s_hs_thread_datum::MainLoop(const s_main_state& state)
		{
			if (!this->StackIsEmpty())
			{
				while (	this->NotKilledOrDormant() &&
						(!blam::game_in_progress() || this->NotSleeping()) &&
						state.runtime_is_enabled)
				{	
					blam::hs_call_evaluate(state.thread_index);
				}
			}
		}
		void s_hs_thread_datum::MainEpilogue(const s_main_state& state, bool& return_delete_thread)
		{
			if (this->StackIsEmpty())
			{
				if (this->type == Enums::_hs_thread_type_script)
				{
					const auto& _script = state.script->script;

					if (_script.type == Enums::_hs_script_startup || _script.type == Enums::_hs_script_dormant)
						this->sleep_until = Enums::_hs_thread_datum_sleep_forever;
				}
				else if (this->type == Enums::_hs_thread_type_runtime_evaluate)
				{
					return_delete_thread = true;
				}
			}
		}
		void s_hs_thread_datum::Main(datum_index this_thread_index, const bool& runtime_globals_enabled,
			bool& return_delete_thread)
		{
			s_main_state state(this_thread_index, runtime_globals_enabled);
			return_delete_thread = false;

			if (this->type == Enums::_hs_thread_type_script)
			{
				state.script = &(blam::global_scenario_get()->scripts[this->script_index]);

				const auto& _script = state.script->script;
				YELO_HS_RUNTIME_ASSERT(_script.type!=Enums::_hs_script_static && _script.type!=Enums::_hs_script_stub, 
					this,
					"found a static script at toplevel.");
			}

			YELO_HS_THREAD_VALID_STACK(this);

			MainPrologue(state);
			MainLoop(state);
			MainEpilogue(state, return_delete_thread);
		}
	};
};