/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/cutscene/recorded_animation_playback.hpp>
#include <blamlib/Halo1/memory/data.hpp>
#include <blamlib/Halo1/units/unit_control.hpp>

namespace Yelo
{
	namespace Scripting
	{
		struct s_recorded_animations_datum : Memory::s_datum_base_aligned
		{
			datum_index unit_index;
			int16 time_left;
			word_flags flags;
			int32 relative_ticks;
			void* event_stream;
			Objects::s_unit_control_data control_data;
			s_recorded_animation_controller controller;
			_enum codec; // see Enums::e_recorded_animation_version
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_recorded_animations_datum) == 0x64 );
	};
};