/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/cutscene/recorded_animations.hpp>
#include <blamlib/Halo1/hs/hs.hpp>
#include <blamlib/Halo1/hs/hs_runtime.hpp>
#include <blamlib/Halo1/hs/object_lists.hpp>
#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace Scripting
	{
		t_recorded_animations_data*		RecordedAnimations();

		t_hs_syntax_data*				HSSyntax();

		t_object_list_header_data*		ObjectListHeader();
		t_list_object_reference_data*	ListObjectReference();

		t_hs_thread_data*				HSThreads();
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