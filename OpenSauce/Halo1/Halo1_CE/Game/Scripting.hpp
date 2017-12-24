/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/cutscene/recorded_animations.hpp>
#include <blamlib/hs/hs.hpp>
#include <blamlib/hs/hs_runtime.hpp>
#include <blamlib/hs/object_lists.hpp>

namespace Yelo
{
	namespace Scripting
	{
		recorded_animations_data_t&		RecordedAnimations();


		void Initialize();
		void Dispose();

		void PLATFORM_API Update();
	};
};