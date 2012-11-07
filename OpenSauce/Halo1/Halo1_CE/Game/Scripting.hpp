/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/cutscene/recorded_animations.hpp>
#include <blamlib/Halo1/hs/hs.hpp>
#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_hs_thread_stack_size = 0x200,
		};

		enum hs_thread_type : _enum
		{
			_hs_thread_type_script,
			_hs_thread_type_global_initialize, // script global initializer thunk
			_hs_thread_type_console_command,
			_hs_thread_type,
		};
	};

	namespace Scripting
	{
		typedef Memory::DataArray<s_recorded_animations_datum, 64> t_recorded_animations_data;
		t_recorded_animations_data*		RecordedAnimations();

		// Note: the max count provided here is the stock count, not the OS "upgrade" count (which allows more)
		typedef Memory::DataArray<hs_syntax_node, 19001> t_hs_syntax_data;
		t_hs_syntax_data*				HSSyntax();


		struct s_object_list_header_datum : Memory::s_datum_base_aligned
		{
			int16 reference_count;	// how many references there are to this list
			int16 count;			// how many objects are in the list
			datum_index first;		// handle to the first list object reference
		}; BOOST_STATIC_ASSERT( sizeof(s_object_list_header_datum) == 0xC );
		typedef Memory::DataArray<s_object_list_header_datum, 48> t_object_list_header_data;
		t_object_list_header_data*		ObjectListHeader();


		struct s_list_object_reference_datum : Memory::s_datum_base_aligned
		{
			datum_index object_index;	// handle to the object datum
			datum_index next_reference;	// handle to the next list object reference in the list
		}; BOOST_STATIC_ASSERT( sizeof(s_list_object_reference_datum) == 0xC );
		typedef Memory::DataArray<s_list_object_reference_datum, 128> t_list_object_reference_data;
		t_list_object_reference_data*	ListObjectReference();


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
		t_hs_thread_data*				HSThreads();


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
		typedef Memory::DataArray<s_hs_globals_datum, 1024> t_hs_globals_data;
		t_hs_globals_data*				HSGlobals();


		void Initialize();
		void Dispose();

		void PLATFORM_API Update();

		datum_index ObjectListNew();
		// Get the object_index of the first object in a list of objects
		// [list_reference] - handle to the object_list's first s_list_object_reference_datum
		datum_index ObjectListGetFirst(datum_index list_index, __out datum_index& list_reference);
		// Get the next object_index in a list of objects
		// [list_reference]:
		// __in  - handle to the current s_list_object_reference_datum in the object_list we're referencing
		// __out - handle to the next s_list_object_reference_datum
		datum_index ObjectListGetNext(datum_index list_index, __inout datum_index& list_reference);
	};
};