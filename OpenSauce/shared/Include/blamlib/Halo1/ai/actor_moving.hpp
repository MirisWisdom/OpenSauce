/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>

namespace Yelo
{
	namespace AI
	{
		namespace e_actor_movement_state
		{
			typedef enum : _enum
			{
				noncombat,
				asleep,
				combat,
				flee,
				flaming,

				k_count
			} type_t;
		}
	}
}
