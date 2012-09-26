/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

static void WeaponTagDataFieldTriggerSetReal(const s_object_field_definition& field, TagGroups::weapon_trigger_definition& trigger, 
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

	default: result.ptr.real = NULL;  break;
	}
}

static void WeaponDataFieldGetReal(const s_object_field_definition& field, s_weapon_data& weapon, 
								   TypeHolder& result, void*)
{
	using namespace Enums;

	switch(field.definition_index)
	{
	case _weapon_field_real_heat:		result.ptr.real = weapon.GetHeat(); break;
	case _weapon_field_real_age:		result.ptr.real = weapon.GetAge(); break;
	case _weapon_field_real_light_power:result.ptr.real = weapon.GetIntegratedLightPower(); break;
	
	default: result.ptr.real = NULL;  break;
	}
}