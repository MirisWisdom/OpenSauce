/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/shaders/shader_definitions.hpp>

namespace Yelo
{
	namespace Render
	{
		// stored in a C-array
		// array has a max size of 0x180 ?
		// everything that's transparent gets put into this array for rendering
		struct s_render_transparent_instance : TStructImpl(0xA8)
		{
			// Yelo::TagGroups::s_shader_definition* shader;			// 0xC 
			// real distance;											// 0x78 distance from camera
		}; BOOST_STATIC_ASSERT( sizeof(s_render_transparent_instance) == 0xA8 );

	}
}