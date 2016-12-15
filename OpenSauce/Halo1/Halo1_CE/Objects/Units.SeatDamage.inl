/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <YeloLib/Halo1/units/c_unit_seat_damage_manager.hpp>

namespace Yelo 
{
	namespace Objects { namespace Units { namespace SeatDamage
	{
		static c_unit_seat_damage_manager g_unit_seat_damage_manager;

		void UnitCauseSeatedPlayerMelee(const datum_index unit_index)
		{
			g_unit_seat_damage_manager.UnitCauseSeatedPlayerMelee(unit_index);
		}

		void UnitThrowSeatedPlayerGrenade(const datum_index unit_index)
		{
			g_unit_seat_damage_manager.UnitThrowSeatedPlayerGrenade(unit_index);
		}

		bool PreventSeatedPlayerGrenade(const datum_index unit_index, const datum_index weapon_index)
		{
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			if(unit_datum->unit.vehicle_seat_index == NONE)
			{
				return blam::weapon_prevents_grenade_throwing(weapon_index);
			}
			else
			{
				return g_unit_seat_damage_manager.PreventSeatedPlayerGrenade(unit_index, weapon_index);
			}
		}
	};};};
};