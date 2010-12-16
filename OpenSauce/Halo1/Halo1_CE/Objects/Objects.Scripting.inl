/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

static real* object_data_get_real_by_name(s_object_data* object, cstring data_name, cstring subdata_name, Enums::hs_type& out_type)
{
	cstring s = data_name; // alias for keeping the code width down

	real* value_ptr = NULL;

	out_type = HS_TYPE(real);
		 if( !strcmp(s,"position") )				value_ptr = &object->GetNetworkDatumData()->position.x;
	else if( !strcmp(s,"transitional_velocity") )	value_ptr = &object->GetNetworkDatumData()->transitional_velocity.i;
	else if( !strcmp(s,"forward") )					value_ptr = &object->GetNetworkDatumData()->forward.i;
	else if( !strcmp(s,"up") )						value_ptr = &object->GetNetworkDatumData()->up.i;
	else if( !strcmp(s,"angular_velocity") )		value_ptr = &object->GetNetworkDatumData()->angular_velocity.i;
	//else if( !strcmp(s,"") )	value_ptr = *object-;

	if(value_ptr != NULL)
	{
		s = subdata_name; // alias for keeping the code width down

		int32 field_index = 0; // default to the first field ie: 'x', 'i', etc

			 if( !strcmp(s,"y") || !strcmp(s,"j") ) field_index = 1;
		else if( !strcmp(s,"z") || !strcmp(s,"k") ) field_index = 2;
//		else if( !strcmp(s,"w") || !strcmp(s,"d") )	field_index = 3;

		return &value_ptr[field_index];
	}

	return NULL;
}
static void* scripting_object_data_get_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index object_index;
		cstring data_name;
		cstring subdata_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.real = -1.0f;

	if(!args->object_index.IsNull())
	{
		s_object_data* object = (*Objects::ObjectHeader())[args->object_index]->_object;

		Enums::hs_type result_type;
		result.ptr.real = object_data_get_real_by_name(object, args->data_name, args->subdata_name, result_type);
		Scripting::UpdateTypeHolderFromPtrToData(result, result_type);
	}

	return result.pointer;
}
static void* scripting_object_data_set_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index object_index;
		cstring data_name;
		cstring subdata_name;
		real data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	if(!args->object_index.IsNull())
	{
		s_object_data* object = (*Objects::ObjectHeader())[args->object_index]->_object;

		TypeHolder result;
		Enums::hs_type result_type;
		result.ptr.real = object_data_get_real_by_name(object, args->data_name, args->subdata_name, result_type);
		Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &args->data_value);
	}

	return NULL;
}


static real* weapon_data_get_real_by_name(s_weapon_datum* weapon, cstring data_name, Enums::hs_type& out_type)
{
	cstring s = data_name; // alias for keeping the code width down

	out_type = HS_TYPE(real);
		 if( !strcmp(s,"heat") )		return weapon->weapon.GetHeat();
	else if( !strcmp(s,"age") )			return weapon->weapon.GetAge();
	else if( !strcmp(s,"light_power") )	return weapon->weapon.GetIntegratedLightPower();

	return NULL;
}
static void* scripting_weapon_data_get_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index weapon_index;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.real = -1.0f;

	if(!args->weapon_index.IsNull())
	{
		s_weapon_datum* weapon = (*Objects::ObjectHeader())[args->weapon_index]->Type._weapon;

		Enums::hs_type result_type;
		result.ptr.real = weapon_data_get_real_by_name(weapon, args->data_name, result_type);
		Scripting::UpdateTypeHolderFromPtrToData(result, result_type);
	}

	return result.pointer;
}
static void* scripting_weapon_data_set_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index weapon_index;
		cstring data_name;
		real data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	if(!args->weapon_index.IsNull())
	{
		s_weapon_datum* weapon = (*Objects::ObjectHeader())[args->weapon_index]->Type._weapon;

		TypeHolder result;
		Enums::hs_type result_type;
		result.ptr.real = weapon_data_get_real_by_name(weapon, args->data_name, result_type);
		Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &args->data_value);
	}

	return NULL;
}


static datum_index scripting_unit_data_get_object_by_name(s_unit_datum* unit, cstring data_name)
{
	cstring s = data_name; // alias for keeping the code width down

	if( !strncmp(s,"weapon",6) )
	{
			 if( !strcmp(s,"weapon0") ) return unit->unit.GetWeaponObjectIndices()[0];
		else if( !strcmp(s,"weapon1") ) return unit->unit.GetWeaponObjectIndices()[1];
		else if( !strcmp(s,"weapon2") ) return unit->unit.GetWeaponObjectIndices()[2];
		else if( !strcmp(s,"weapon3") ) return unit->unit.GetWeaponObjectIndices()[3];
	}
	else if( !strncmp(s,"recent_damage.unit",6) )
	{
			 if( !strcmp(s,"recent_damage.unit0") ) return unit->unit.GetRecentDamage()[0].responsible_unit;
		else if( !strcmp(s,"recent_damage.unit1") ) return unit->unit.GetRecentDamage()[0].responsible_unit;
		else if( !strcmp(s,"recent_damage.unit2") ) return unit->unit.GetRecentDamage()[0].responsible_unit;
		else if( !strcmp(s,"recent_damage.unit3") ) return unit->unit.GetRecentDamage()[0].responsible_unit;
	}

	return datum_index::null;
}
static void* scripting_unit_data_get_object_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.datum = datum_index::null;

	if(!args->unit_index.IsNull())
	{
		s_unit_datum* unit = (*Objects::ObjectHeader())[args->unit_index]->Type._unit;

		result.datum = scripting_unit_data_get_object_by_name(unit, args->data_name);
	}

	return result.pointer;
}

static void* scripting_unit_data_get_integer_by_name(s_unit_datum* unit, cstring data_name, Enums::hs_type& out_type)
{
	cstring s = data_name; // alias for keeping the code width down

	out_type = HS_TYPE(bool);
	// designers should use 'unit_get_total_grenade_count' for overall grenade count
		 if( !strcmp(s,"total_grenade_count[plasma]") )	return unit->unit.GetGrenadePlasmaCount();
	else if( !strcmp(s,"total_grenade_count[frag]") )	return unit->unit.GetGrenadeFragCount();

	out_type = HS_TYPE(short);
		 if( !strcmp(s,"vehicle_seat_index") )		return unit->unit.GetVehicleSeatIndex();
	else if( !strcmp(s,"current_weapon_index") )	return unit->unit.GetCurrentWeaponIndex();
	else if( !strcmp(s,"current_grenade_index") )	return unit->unit.GetCurrentGrenadeIndex();
	else if( !strcmp(s,"feign_death_timer") )		return unit->unit.GetFeignDeathTimer();

	out_type = HS_TYPE(void);
	return NULL;
}
static void* scripting_unit_data_get_integer_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.int32 = 0;

	if(!args->unit_index.IsNull())
	{
		s_unit_datum* unit = (*Objects::ObjectHeader())[args->unit_index]->Type._unit;

		Enums::hs_type result_type;
		result.pointer = scripting_unit_data_get_integer_by_name(unit, args->data_name, result_type);
		Scripting::UpdateTypeHolderFromPtrToData(result, result_type);
	}

	return result.pointer;
}
static void* scripting_unit_data_set_integer_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		cstring data_name;
		int32 data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	if(!args->unit_index.IsNull())
	{
		s_unit_datum* unit = (*Objects::ObjectHeader())[args->unit_index]->Type._unit;

		TypeHolder result;
		Enums::hs_type result_type;
		result.pointer = scripting_unit_data_get_integer_by_name(unit, args->data_name, result_type);
		Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &args->data_value);
	}

	return NULL;
}

static real* scripting_unit_data_get_real_by_name(s_unit_datum* unit, cstring data_name, Enums::hs_type& out_type)
{
	cstring s = data_name; // alias for keeping the code width down

	out_type = HS_TYPE(real);
		 if( !strcmp(s,"camo_power") )		return unit->unit.GetCamoPower();
	//else if( !strcmp(s,"") )	return unit->Get();

	return NULL;
}
static void* scripting_unit_data_get_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.real = -1.0f;

	if(!args->unit_index.IsNull())
	{
		s_unit_datum* unit = (*Objects::ObjectHeader())[args->unit_index]->Type._unit;

		Enums::hs_type result_type;
		result.ptr.real = scripting_unit_data_get_real_by_name(unit, args->data_name, result_type);
		Scripting::UpdateTypeHolderFromPtrToData(result, result_type);
	}

	return result.pointer;
}
static void* scripting_unit_data_set_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		cstring data_name;
		real data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	if(!args->unit_index.IsNull())
	{
		s_unit_datum* unit = (*Objects::ObjectHeader())[args->unit_index]->Type._unit;

		TypeHolder result;
		Enums::hs_type result_type;
		result.ptr.real = scripting_unit_data_get_real_by_name(unit, args->data_name, result_type);
		Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &args->data_value);
	}

	return NULL;
}