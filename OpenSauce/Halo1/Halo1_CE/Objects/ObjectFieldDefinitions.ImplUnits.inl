/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

static bool UnitDataFieldGetObjectIndex(const s_object_field_definition& field, s_unit_data& unit, 
											   TypeHolder& result, cstring data_name)
{
	using namespace Enums;
	result.ptr.datum = nullptr;

	int subfield_index = SubscriptStringToIndex(data_name);

	switch(field.definition_index)
	{
	case _unit_field_object_index_throwing_grenade_projectile:		result.ptr.datum = &unit.throwing_grenade_projectile_index; break;
	case _unit_field_object_index_equipment:						result.ptr.datum = &unit.equipment_index; break;
	//case _unit_field_object_index_40C:								result.ptr.datum = &unit.responsible_unit_index; break;
	case _unit_field_object_index_damage_responsible_flamer_object:	result.ptr.datum = &unit.responsible_flamer_object_index; break;

	case _unit_field_object_index_weapon:				if(subfield_index >= 0 && subfield_index < Enums::k_maximum_weapons_per_unit)
		result.ptr.datum = &unit.weapon_object_indices[subfield_index];
		break;
	case _unit_field_object_index_recent_damage_unit:	if(subfield_index >= 0 && subfield_index < NUMBEROF(unit.recent_damage))
		result.ptr.datum = &unit.recent_damage[subfield_index].responsible_unit;
		break;
	}

	return result.ptr.datum != nullptr;
}

static void* UnitDataFieldGetIntegerImpl(const s_object_field_definition& field, s_unit_data& unit)
{
	using namespace Enums;

	switch(field.definition_index)
	{
	case _unit_field_integer_ticks_until_flame_to_death:	return &unit.ticks_until_flame_to_death;
	case _unit_field_integer_throwing_grenade_state:		return &unit.throwing_grenade_state;

	case _unit_field_integer_total_grenade_count_frag:		return &unit.grenade_counts[Enums::_unit_grenade_type_frag];
	case _unit_field_integer_total_grenade_count_plasma:	return &unit.grenade_counts[Enums::_unit_grenade_type_plasma];
	case _unit_field_integer_total_grenade_count_custom2:	return unit.GetYeloGrenade2Count();
	case _unit_field_integer_total_grenade_count_custom3:	return unit.GetYeloGrenade3Count();

	case _unit_field_integer_current_grenade_index:			return &unit.current_grenade_index;
	case _unit_field_integer_zoom_level:					return unit.GetZoomLevel();
	case _unit_field_integer_desired_zoom_level:			return unit.GetDesiredZoomLevel();

	case _unit_field_integer_vehicle_seat_index:			return &unit.vehicle_seat_index;
	case _unit_field_integer_current_weapon_index:			return &unit.current_weapon_index;
	case _unit_field_integer_feign_death_timer:				return &unit.feign_death_timer;
	case _unit_field_integer_killing_spree_count:			return &unit.killing_spree_count;

	default: return nullptr;
	}
}
static bool UnitDataFieldGetInteger(const s_object_field_definition& field, s_unit_data& unit, 
									TypeHolder& result, void*)
{
	result.pointer = UnitDataFieldGetIntegerImpl(field, unit);

	return result.pointer != nullptr;
}


static bool UnitDataFieldGetReal(const s_object_field_definition& field, s_unit_data& unit, 
								 TypeHolder& result, void*)
{
	using namespace Enums;

	switch(field.definition_index)
	{
	case _unit_field_real_driver_power:							result.ptr.real = &unit.powered_seats_power[Enums::_powered_seat_driver]; break;
	case _unit_field_real_gunner_power:							result.ptr.real = &unit.powered_seats_power[Enums::_powered_seat_gunner]; break;
	case _unit_field_real_integrated_light_power:				result.ptr.real = &unit.integrated_light_power; break;
	case _unit_field_real_integrated_light_toggle_power:		result.ptr.real = &unit.integrated_light_toggle_power; break;
	case _unit_field_real_integrated_night_vision_toggle_power:	result.ptr.real = &unit.integrated_night_vision_toggle_power; break;
	case _unit_field_real_camo_power:							result.ptr.real = &unit.camo_power; break;
	case _unit_field_real_full_spectrum_vision_power:			result.ptr.real = &unit.full_spectrum_vision_power; break;

	default: result.ptr.real = nullptr;  break;
	}

	return result.ptr.real != nullptr;
}