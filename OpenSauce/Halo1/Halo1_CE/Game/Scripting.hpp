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

namespace Yelo
{
	namespace Scripting
	{
		recorded_animations_data_t&		RecordedAnimations();

		hs_syntax_data_t&				HSSyntax();

		object_list_header_data_t&		ObjectListHeader();
		list_object_reference_data_t&	ListObjectReference();

		hs_thread_data_t&				HSThreads();
		hs_globals_data_t&				HSGlobals();


		void Initialize();
		void Dispose();

		void PLATFORM_API Update();
	};
};