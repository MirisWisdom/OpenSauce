/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace AI
	{
		struct s_actor_datum;
		struct s_prop_datum;

		bool PLATFORM_API ActorPropShouldCauseExitVehicle(const s_actor_datum* actor_datum, const s_prop_datum* prop_datum, const bool is_enemy);

		bool PLATFORM_API ActorShouldIgnoreSeatedProp(const s_prop_datum* prop);
		
		bool PLATFORM_API ActorShouldPanicAboutMountedUnit(const datum_index unit_index);
	};
};