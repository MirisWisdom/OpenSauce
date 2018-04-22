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
		namespace e_actor_perception_type
		{
			typedef enum : _enum
			{
				none,
				partial,
				full,
				unmistakable,

				k_count
			} type_t;
		}

		namespace e_actor_target_type
		{
			typedef enum : _enum
			{
				k_count = 12
			} type_t;
		}

		namespace e_actor_danger_zone
		{
			typedef enum : _enum
			{
				none,
				suicide,
				projectile,
				vehicle,

				k_count
			} type_t;
		}
	}
}
