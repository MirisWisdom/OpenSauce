/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

static bool WeaponTagDataFieldTriggerSetReal(const s_object_field_definition& field, TagGroups::weapon_trigger_definition& trigger, 
											 TypeHolder& result, cstring subdata_name)
{
	using namespace Enums;

	switch(field.definition_index)
	{
	case _weapon_tag_field_real_trigger_spew_time:
		result.ptr.real = &trigger.spew_time;
		break;
	case _weapon_tag_field_real_trigger_rounds_per_second:
			 if( !strcmp(subdata_name,"lower") ) result.ptr.real = &trigger.rounds_per_second.lower;
		else if( !strcmp(subdata_name,"upper") ) result.ptr.real = &trigger.rounds_per_second.upper;
		break;

	default: result.ptr.real = nullptr;  break;
	}

	return result.ptr.real != nullptr;
}

static bool WeaponDataFieldGetReal(const s_object_field_definition& field, s_weapon_data& weapon, 
								   TypeHolder& result, void*)
{
	using namespace Enums;

	switch(field.definition_index)
	{
	case _weapon_field_real_heat:		result.ptr.real = &weapon.heat; break;
	case _weapon_field_real_age:		result.ptr.real = &weapon.age; break;
	case _weapon_field_real_light_power:result.ptr.real = &weapon.integrated_light_power; break;
	
	default: result.ptr.real = nullptr;  break;
	}

	return result.ptr.real != nullptr;
}

static void* WeaponDataFieldMagazineGetIntegerImpl(const s_object_field_definition& field, Objects::s_weapon_data::s_magazine_state& magazine, 
											 TypeHolder& result, cstring subdata_name)
{
	using namespace Enums;

	switch(field.definition_index)
	{
	case _weapon_field_integer_magazine_state:				return CAST_PTR(int16*, &magazine.state); // state is an _enum
	case _weapon_field_integer_magazine_reload:
			 if( !strcmp(subdata_name,"time_remaining") )	return &magazine.reload_time_remaining;
		else if( !strcmp(subdata_name,"time") )				return &magazine.reload_time;
		return nullptr;
	case _weapon_field_integer_magazine_rounds:
			 if( !strcmp(subdata_name,"unloaded") )			return &magazine.rounds_unloaded;
		else if( !strcmp(subdata_name,"loaded") )			return &magazine.rounds_loaded;
		else if( !strcmp(subdata_name,"left_to_recharge") )	return &magazine.rounds_left_to_recharge;
		return nullptr;

	default: return nullptr;
	}
}

static bool WeaponDataFieldMagazineGetInteger(const s_object_field_definition& field, Objects::s_weapon_data::s_magazine_state& magazine, 
											 TypeHolder& result, cstring subdata_name)
{
	result.pointer = WeaponDataFieldMagazineGetIntegerImpl(field, magazine, 
		result, subdata_name);

	return result.pointer != nullptr;
}