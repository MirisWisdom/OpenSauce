/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/ai/ai_yelo.hpp>

#include <blamlib/Halo1/ai/actor_structures.hpp>
#include <blamlib/Halo1/ai/prop_structures.hpp>
#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/object_structures.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>

#include <YeloLib/Halo1/units/units_yelo.hpp>

namespace Yelo
{
	namespace AI
	{
		bool PLATFORM_API ActorPropShouldCauseExitVehicle(const s_actor_datum* actor_datum, const s_prop_datum* prop_datum, const bool is_enemy)
		{
			// If the prop is not an enemy is shouldn't cause the ai to exit
			if(!is_enemy)
			{
				return false;
			}

			// If the actor isn't in a vehicle...there's no vehicle to exit
			auto* unit_datum = blam::object_get_and_verify_type<Objects::s_unit_datum>(actor_datum->meta.unit_index);
			if(unit_datum->object.parent_object_index.IsNull() || unit_datum->unit.vehicle_seat_index == NONE)
			{
				return false;
			}

			// Don't try and exit the vehicle if there is a unit in a seat that target's the actor's
			auto* seat_extension_definition = Objects::GetSeatExtensionDefinition(unit_datum->object.parent_object_index, unit_datum->unit.vehicle_seat_index);
			if(seat_extension_definition && seat_extension_definition->seat_targeting_seats.Count)
			{
				for(auto seat_index : seat_extension_definition->seat_targeting_seats)
				{
					if(Objects::GetUnitInSeat(unit_datum->object.parent_object_index, seat_index) == prop_datum->unit_index)
					{
						return false;
					}
				}
			}

			return true;
		}
	};
};