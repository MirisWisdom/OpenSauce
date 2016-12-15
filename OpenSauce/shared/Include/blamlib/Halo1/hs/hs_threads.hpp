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
			_hs_thread_datum_has_new_stack_frame_bit,
			_hs_thread_datum_sleeping_bit,
		};
	};

	namespace Scripting
	{
		struct hs_syntax_node;

		struct s_hs_thread_datum : Memory::s_datum_base
		{
// 32bit packing only for stock code computability.
#include <PshPack4.h>
			struct s_stack_frame
			{
				s_stack_frame* previous;
				datum_index source_expression;
				s_hs_value_union* result_reference;
				uint16 size;
#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
				byte data[];
#pragma warning( pop )

				void Initialize();

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Test whether the requested allocation would overflow. </summary>
				///
				/// <param name="stack_base_address">	The base address of the actual stack. </param>
				/// <param name="alignment">		 	The number of alignment bytes (if any) before [size]. </param>
				/// <param name="size">				 	The size of the allocation. </param>
				///
				/// <returns>	true if it allocation parameters would cause an overflow, false if they're good. </returns>
				bool AllocationWouldOverflow(const byte* stack_base_address, size_t size, size_t alignment) const;
			}; BOOST_STATIC_ASSERT( sizeof(s_stack_frame) == 0x10 );
#include <PopPack.h>

			Enums::hs_thread_type type;
			word_flags flags;
			int16 script_index; // only when type==_hs_thread_type_script
			game_ticks_t sleep_until;
			game_ticks_t prev_sleep_state; // when the thread is put to (sleep), this becomes sleep_until's old value, and then current value when (wake)'d
			s_stack_frame* stack_frame;
			// NOTE: the only place this is used/referenced is in hs_runtime_evaluate. We can potentially use the bits
			// here for our own purposes if we don't wish to change the size of this struct
			s_hs_value_union result;

			byte stack_data[Enums::k_hs_thread_stack_size];

			void Initialize(Enums::hs_thread_type type);
			void InitializeForScript(hs_script_index_t script);

			const hs_syntax_node* GetSyntax(datum_index expression_index) const;
			const hs_syntax_node* GetFrameSyntax() const;
			const hs_syntax_node* TryGetFrameSyntax() const;
			const hs_syntax_node* TryGetPrevFrameSyntax() const;

			bool ScriptError(cstring reason, cstring expression) const;

			bool StackIsEmpty() const;
			bool NotKilledOrDormant() const;
			bool NotSleeping(game_ticks_t relative_to_game_time) const;
			bool NotSleeping() const;

			void Wake();

			bool ValidThread() const;
			cstring GetDescriptionString() const;

			void StackPush();
			void StackPop();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Allocate memory from the thread's stack for script memory. </summary>
			///
			/// <param name="size">				The size of the allocation. </param>
			/// <param name="alignment_bit">	The alignment for the allocation. </param>
			/// <param name="stack_offset">
			/// 	[out] (Optional) If non-null, (Optional) the stack offset where the allocation begins.
			/// </param>
			///
			/// <returns>	null if it fails, else a pointer to the requested memory. </returns>
			void* StackAllocate(
				size_t size, long_flags alignment_bit = Flags::k_alignment_32bit, _Out_opt_ int16* stack_offset = nullptr);

			template<typename T>
			T* StackAllocate(size_t count = 1,
				long_flags alignment_bit = Flags::k_alignment_32bit, _Out_opt_ int16* stack_offset = nullptr)
			{
				return CAST_PTR(T*, StackAllocate(sizeof(T)* count, alignment_bit, stack_offset));
			}

			struct s_main_state
			{
				datum_index thread_index;
				const bool& runtime_is_enabled;

				const TagGroups::hs_script* script;

				s_main_state(datum_index this_thread_index, const bool& runtime_globals_enabled)
					: thread_index(this_thread_index)
					, runtime_is_enabled(runtime_globals_enabled)
					, script(nullptr)
				{
				}
			};
			void MainPrologue(const s_main_state& state);
			void MainLoop(const s_main_state& state);
			void MainEpilogue(const s_main_state& state, _Out_opt_ bool& return_delete_thread);
			void Main(datum_index this_thread_index, const bool& runtime_globals_enabled,
				_Out_opt_ bool& return_delete_thread);

		}; BOOST_STATIC_ASSERT( sizeof(s_hs_thread_datum) == 0x218 );
	};
};

#define YELO_HS_RUNTIME_ASSERT(expression, thread, explanation)										\
	YELO_ASSERT_DISPLAY(expression, "a problem occurred while executing the script %s: %s (%s)",	\
		thread->GetDescriptionString(), explanation, #expression)
#define YELO_HS_THREAD_VALID_STACK(thread)															\
	YELO_HS_RUNTIME_ASSERT(thread->ValidThread(), thread,											\
		"corrupted stack.")

#if PLATFORM_IS_EDITOR || YELO_ASSERT_ENABLED
	#define YELO_HS_THREAD_SCRIPT_ERROR(expression, thread, explanation)							\
		( !(expression) && !(thread)->ScriptError(explanation, #expression) )
#else
	#define YELO_HS_THREAD_SCRIPT_ERROR(expression, thread, explanation) false
#endif