/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blam/cseries/cseries_base.hpp>

namespace Yelo
{
	namespace AI
	{
		namespace e_secondary_look_type
		{
			typedef enum : _enum
			{
				none,
				environment,
				moving_prop,
				impact,
				new_prop,
				bumped,
				detonation,
				shooting_prop,
				comm_prop,
				comm_direction,
				combat_prop,
				damage,
				danger,
				scripted,

				k_count
			} type_t;
		}
	}
}
