/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Objects
	{
		struct s_animation_state
		{
			int16 animation_index;
			int16 frame_index;
		}; BOOST_STATIC_ASSERT( sizeof(s_animation_state) == 0x4 );
	};
};