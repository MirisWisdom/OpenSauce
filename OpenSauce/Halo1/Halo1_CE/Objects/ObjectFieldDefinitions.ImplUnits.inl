/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

static void UnitDataFieldGetObjectIndex(const s_object_field_definition& field, s_unit_data& unit, 
											   TypeHolder& result, cstring data_name)
{
	using namespace Enums;
	result.ptr.datum = NULL;

	int subfield_index = SubscriptStringToIndex(data_name);

	switch(field.definition_index)
	{
	case _unit_field_object_index_weapon:				if(subfield_index >= 0 && subfield_index < Enums::k_maximum_weapons_per_unit)
		result.ptr.datum = &unit.GetWeaponObjectIndices()[subfield_index];
		break;
	case _unit_field_object_index_recent_damage_unit:	if(subfield_index >= 0 && subfield_index < 4)
		result.ptr.datum = &unit.GetRecentDamage()[subfield_index].responsible_unit;
		break;
	}
}

static void* UnitDataFieldGetIntegerImpl(const s_object_field_definition& field, s_unit_data& unit)
{
	using namespace Enums;

	switch(field.definition_index)
	{
	case _unit_field_integer_total_grenade_count_frag:		return unit.GetGrenadeFragCount();
	case _unit_field_integer_total_grenade_count_plasma:	return unit.GetGrenadePlasmaCount();
	case _unit_field_integer_total_grenade_count_custom2:	return unit.GetYeloGrenade2Count();
	case _unit_field_integer_total_grenade_count_custom3:	return unit.GetYeloGrenade3Count();

	case _unit_field_integer_current_grenade_index:			return unit.GetCurrentGrenadeIndex();
	case _unit_field_integer_zoom_level:					return unit.GetZoomLevel();
	case _unit_field_integer_desired_zoom_level:			return unit.GetDesiredZoomLevel();

	case _unit_field_integer_vehicle_seat_index:			return unit.GetVehicleSeatIndex();
	case _unit_field_integer_current_weapon_index:			return unit.GetCurrentWeaponIndex();
	case _unit_field_integer_feign_death_timer:				return unit.GetFeignDeathTimer();

	default: return NULL;
	}
}
static void UnitDataFieldGetInteger(const s_object_field_definition& field, s_unit_data& unit, 
									TypeHolder& result, void*)
{
	result.pointer = UnitDataFieldGetIntegerImpl(field, unit);
}


static void UnitDataFieldGetReal(const s_object_field_definition& field, s_unit_data& unit, 
								 TypeHolder& result, void*)
{
	using namespace Enums;

	switch(field.definition_index)
	{
	case _unit_field_real_camo_power:	result.ptr.real = unit.GetCamoPower(); break;
	case _unit_field_real_driver_power:	result.ptr.real = unit.GetDriverPower(); break;
	case _unit_field_real_gunner_power:	result.ptr.real = unit.GetGunnerPower(); break;

	default: result.ptr.real = NULL;  break;
	}
}