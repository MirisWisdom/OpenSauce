/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/simple_circular_queue.hpp>

namespace Yelo
{
	namespace Game
	{
		struct s_position_queue_entry : TStructImpl(20)
		{
		};
		struct s_position_queue
		{
			Memory::s_simple_circular_queue queue;
			s_position_queue_entry* entries;		// 0x14, #30, user_data
		}; BOOST_STATIC_ASSERT( sizeof(s_position_queue) == 0x18 );
	};
};