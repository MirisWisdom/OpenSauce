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

static real object_data_get_real_by_name(s_object_data* object, cstring data_name, cstring subdata_name)
{
	cstring s = data_name; // alias for keeping the code width down

	real* value_ptr = NULL;

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

		return value_ptr[field_index];
	}

	return 0.0f;
}
static void* scripting_object_data_get_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index object_index;
		cstring data_name;
		cstring subdata_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.real = 0.0f;

	if(!args->object_index.IsNull())
	{
		s_object_data* object = (*Objects::ObjectHeader())[args->object_index]->_object;

		result.real = object_data_get_real_by_name(object, args->data_name, args->subdata_name);
	}

	return result.pointer;
}


static real weapon_data_get_real_by_name(s_weapon_datum* weapon, cstring data_name)
{
	cstring s = data_name; // alias for keeping the code width down

		 if( !strcmp(s,"heat") )		return *weapon->weapon.GetHeat();
	else if( !strcmp(s,"age") )			return *weapon->weapon.GetAge();
	else if( !strcmp(s,"light_power") )	return *weapon->weapon.GetIntegratedLightPower();

	return 0.0f;
}
static void* scripting_weapon_data_get_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index weapon_index;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.real = 0.0f;

	if(!args->weapon_index.IsNull())
	{
		s_weapon_datum* weapon = (*Objects::ObjectHeader())[args->weapon_index]->Type._weapon;

		result.real = weapon_data_get_real_by_name(weapon, args->data_name);
	}

	return result.pointer;
}