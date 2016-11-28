/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum actor_perception_type : _enum
		{
			_actor_perception_type_none,
			_actor_perception_type_partial,
			_actor_perception_type_full,
			_actor_perception_type_unmistakable,

			k_number_of_actor_perception_types,
		};

		enum actor_target_type : _enum
		{
			k_number_of_actor_target_types = 12
		};

		enum actor_danger_zone : _enum
		{
			_actor_danger_zone_none,
			_actor_danger_zone_suicide,
			_actor_danger_zone_projectile,
			_actor_danger_zone_vehicle,
		};
	};
};