/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>

#include <blamlib/Halo1/objects/object_lights.hpp>
#include <blamlib/Halo1/render/render_constants.hpp>
#include <YeloLib/Halo1/render/render_constants_yelo.hpp>

namespace Yelo
{
	namespace Render
	{
		struct s_render_object_globals
		{
			int16 rendered_objects_count;
			PAD16;
			datum_index rendered_objects[Enums::k_maximum_rendered_objects];
		};


		struct s_cached_object_render_state_datum : Memory::s_datum_base_aligned
		{
			datum_index object_index;
			UNKNOWN_TYPE(int32); // frame counter
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32); // frame counter
			Objects::s_object_lighting lighting;
			Objects::s_object_lighting desired_lighting;
			UNKNOWN_TYPE(real);
		}; BOOST_STATIC_ASSERT( sizeof(s_cached_object_render_state_datum) == 0x100 );
		typedef Memory::DataArray<	s_cached_object_render_state_datum, 
									Enums::k_maximum_rendered_objects,
									Enums::k_maximum_rendered_objects_upgrade> 
			cached_object_render_states_data_t;
	};
};