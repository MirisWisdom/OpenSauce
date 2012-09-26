/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

static void* scripting_objects_distance_to_object_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index object_list;
		datum_index dest_object;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.real = -1.0f;

	if(!args->dest_object.IsNull())
	{
		real min_dist = FLT_MAX;

		// Get the destination object's origin so that we can compare it, relative to each object in the list
		real_vector3d dest_object_origin;
		Engine::Objects::GetOrigin(args->dest_object, CAST_PTR(real_point3d*, &dest_object_origin));

		// Enumerate the object list, testing each object's origin with dest
		for(datum_index curr_list_reference, curr_object_index = Scripting::ObjectListGetFirst(args->object_list, curr_list_reference); 
			!curr_object_index.IsNull(); 
			curr_object_index = Scripting::ObjectListGetNext(args->object_list, curr_list_reference))
		{
			// Compare the current object from the list to the destination object
			real dist = GetObjectDistanceFromPoint(curr_object_index, dest_object_origin);

			// We want the smallest distance of all the objects
			if(min_dist > dist) min_dist = dist;
		}

		if(min_dist != FLT_MAX) result.real = min_dist;
	}

	return result.pointer;
}


static real_vector3d* object_data_get_vector_by_name(s_object_header_datum* obj, cstring data_name)
{
	cstring s = data_name; // alias for keeping the code width down

	real_vector3d* value_ptr = NULL;

		 if( !strcmp(s,"position") )				value_ptr = CAST_PTR(real_vector3d*, &obj->_object->GetNetworkDatumData()->position);
	else if( !strcmp(s,"transitional_velocity") )	value_ptr = &obj->_object->GetNetworkDatumData()->transitional_velocity;
	else if( !strcmp(s,"forward") )					value_ptr = &obj->_object->GetNetworkDatumData()->forward;
	else if( !strcmp(s,"up") )						value_ptr = &obj->_object->GetNetworkDatumData()->up;
	else if( !strcmp(s,"angular_velocity") )		value_ptr = &obj->_object->GetNetworkDatumData()->angular_velocity;
	//else if( !strcmp(s,"") )	value_ptr = &obj->_object-;

	if(obj->object_type == Enums::_object_type_biped || obj->object_type == Enums::_object_type_vehicle)
	{
		s_unit_data* unit = &obj->Type._unit->unit;

			 if( !strcmp(s,"desired_facing") )	value_ptr = unit->GetDesiredFacingVector();
		else if( !strcmp(s,"desired_aiming") )	value_ptr = unit->GetDesiredAimingVector();
		else if( !strcmp(s,"aiming") )			value_ptr = unit->GetAimingVector();
		else if( !strcmp(s,"aiming_velocity") )	value_ptr = unit->GetAimingVelocity();
		else if( !strcmp(s,"looking") )			value_ptr = unit->GetLookingVector();
		else if( !strcmp(s,"looking_angles") )	value_ptr = CAST_PTR(real_vector3d*, unit->GetLookingAngles());
		else if( !strcmp(s,"looking_velocity") )value_ptr = unit->GetLookingVelocity();
	}

	return value_ptr;
}
static real* object_data_get_real_by_name(s_object_header_datum* obj, cstring data_name, cstring subdata_name, Enums::hs_type& out_type)
{
	cstring s = data_name; // alias for keeping the code width down

	real* value_ptr = NULL;

	out_type = HS_TYPE(real);
	value_ptr = CAST_PTR(real*, object_data_get_vector_by_name(obj, data_name));

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
		s_object_header_datum* object = (*Objects::ObjectHeader())[args->object_index];

#if 0
		Enums::hs_type result_type;
		result.ptr.real = object_data_get_real_by_name(object, args->data_name, args->subdata_name, result_type);
		Scripting::UpdateTypeHolderFromPtrToData(result, result_type);
#else
		ObjectDataGetRealByName(object, args->data_name, args->subdata_name, result);
#endif
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

	// FIXME: I believe some of the unit-based properties aren't sync'd...need to disable setting them when !Networking::IsLocal()
	if(!args->object_index.IsNull())
	{
		s_object_header_datum* object = (*Objects::ObjectHeader())[args->object_index];

#if 0
		TypeHolder result;
		Enums::hs_type result_type;
		result.ptr.real = object_data_get_real_by_name(object, args->data_name, args->subdata_name, result_type);
		Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &args->data_value);
#else
		ObjectDataSetRealByName(object, args->data_name, args->subdata_name, args->data_value);
#endif
	}

	return NULL;
}
static void* scripting_object_data_set_vector_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index object_index;
		cstring data_name;
		int16 vector_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	// FIXME: I believe some of the unit-based properties aren't sync'd...need to disable setting them when !Networking::IsLocal()
	if(!args->object_index.IsNull())
	{
		s_object_header_datum* object = (*Objects::ObjectHeader())[args->object_index];

#if 0
		real_vector3d* obj_vector = object_data_get_vector_by_name(object, args->data_name);
#else
		real_vector3d* obj_vector = ObjectDataGetVectorByName(object, args->data_name);
#endif
		const real_vector3d* vector = GameState::RuntimeData::VectorValueGet(args->vector_index);
		if(obj_vector != NULL && vector != NULL)
		{
			*obj_vector = *vector;
			result.boolean = true;
		}
	}

	return result.pointer;
}


//////////////////////////////////////////////////////////////////////////
// WEAPONS

static real* weapon_data_get_real_by_name(s_weapon_datum* weapon, cstring data_name, Enums::hs_type& out_type, bool& out_is_networked)
{
	cstring s = data_name; // alias for keeping the code width down
	out_is_networked = false; // by default, we assume the properties aren't sync'd

	out_type = HS_TYPE(real);
		 if( !strcmp(s,"heat") )		return weapon->weapon.GetHeat();
	else if( !strcmp(s,"age") )
	{
										out_is_networked = true;
										return weapon->weapon.GetAge();
	}
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
		s_object_header_datum* header = (*Objects::ObjectHeader())[args->weapon_index];

		if(header->object_type == Enums::_object_type_weapon)
		{
			s_weapon_datum* weapon = header->Type._weapon;

#if 0
			Enums::hs_type result_type;
			bool is_networked;
			result.ptr.real = weapon_data_get_real_by_name(weapon, args->data_name, result_type, is_networked);
			Scripting::UpdateTypeHolderFromPtrToData(result, result_type);
#else
			WeaponDataGetRealByName(weapon, args->data_name, result);
#endif
		}
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

	if(Networking::IsLocal() && !args->weapon_index.IsNull())
	{
		s_object_header_datum* header = (*Objects::ObjectHeader())[args->weapon_index];

		if(header->object_type == Enums::_object_type_weapon)
		{
			s_weapon_datum* weapon = header->Type._weapon;

#if 0
			TypeHolder result;
			Enums::hs_type result_type;
			bool is_networked;
			result.ptr.real = weapon_data_get_real_by_name(weapon, args->data_name, result_type, is_networked);
			// Only set properties in local games, or if the properties are sync'd
			if(Networking::IsLocal() || is_networked)
				Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &args->data_value);
#else
			WeaponDataSetRealByName(weapon, args->data_name, args->data_value);
#endif
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// WEAPONS - TRIGGERS

static real* weapon_data_trigger_set_real_by_name(s_weapon_datum* weapon, int32 trigger_index, cstring data_name, cstring subdata_name, Enums::hs_type& out_type)
{
	// IF ANY OF YOUR DESIGNERS USE THIS FUCKING SCRIPT FUNCTION, THEN YOU ALL DESERVE TO BE FUCKING SHOT.

	// SRSLY.

	cstring s = data_name; // alias for keeping the code width down

	out_type = HS_TYPE(real);

	datum_index definition_index = *weapon->object.GetTagDefinition();

	const TagGroups::s_weapon_definition* definition = TagGroups::Instances()[ definition_index.index ]
		.Definition<TagGroups::s_weapon_definition>();

	if (trigger_index >= 0 && trigger_index < definition->weapon.triggers.Count)
	{
		// We're fucking with the tag definition...at runtime :|
		TagGroups::weapon_trigger_definition* trigger = CAST_QUAL(TagGroups::weapon_trigger_definition*, 
			&definition->weapon.triggers[trigger_index]);

			 if( !strcmp(s,"spew_time") )		return &trigger->spew_time;
		else if( !strcmp(s,"rounds_per_second") )
		{
			s = subdata_name; // alias for keeping the code width down

				 if( !strcmp(s,"lower") )		return &trigger->rounds_per_second.lower;
			else if( !strcmp(s,"upper") )		return &trigger->rounds_per_second.upper;
		}
	}

	return NULL;
}

static void* scripting_weapon_data_trigger_set_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index weapon_index;
		int32 trigger_index;
		cstring data_name;
		cstring subdata_name;
		real data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	// We don't support modifying trigger data in anything but local games because it writes to tag memory
	if(Networking::IsLocal() && !args->weapon_index.IsNull())
	{
		s_object_header_datum* header = (*Objects::ObjectHeader())[args->weapon_index];

		if(header->object_type == Enums::_object_type_weapon)
		{
			s_weapon_datum* weapon = header->Type._weapon;

#if 0
			TypeHolder result;
			Enums::hs_type result_type;
			result.ptr.real = weapon_data_trigger_set_real_by_name(weapon, args->trigger_index, args->data_name, args->subdata_name, result_type);
			Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &args->data_value);
#else
			WeaponTagDataTriggerSetRealByName(weapon, args->trigger_index, args->data_name, args->subdata_name, args->data_value);
#endif
		}
	}

	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// UNITS

static datum_index scripting_unit_data_get_object_by_name(s_unit_datum* unit, cstring data_name)
{
	cstring s = data_name; // alias for keeping the code width down

	if( _HS_UTIL_STRNCMP(s, "weapon") )
	{
			 if( !strcmp(s,"weapon0") ) return unit->unit.GetWeaponObjectIndices()[0];
		else if( !strcmp(s,"weapon1") ) return unit->unit.GetWeaponObjectIndices()[1];
		else if( !strcmp(s,"weapon2") ) return unit->unit.GetWeaponObjectIndices()[2];
		else if( !strcmp(s,"weapon3") ) return unit->unit.GetWeaponObjectIndices()[3];
	}
	else if( _HS_UTIL_STRNCMP(s, "recent_damage.unit") )
	{
			 if( !strcmp(s,"recent_damage.unit0") ) return unit->unit.GetRecentDamage()[0].responsible_unit;
		else if( !strcmp(s,"recent_damage.unit1") ) return unit->unit.GetRecentDamage()[1].responsible_unit;
		else if( !strcmp(s,"recent_damage.unit2") ) return unit->unit.GetRecentDamage()[2].responsible_unit;
		else if( !strcmp(s,"recent_damage.unit3") ) return unit->unit.GetRecentDamage()[3].responsible_unit;
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

#if 0
		result.datum = scripting_unit_data_get_object_by_name(unit, args->data_name);
#else
		UnitDataGetObjectIndexByName(unit, args->data_name, result);
#endif
	}

	return result.pointer;
}

static void* scripting_unit_data_get_integer_by_name(s_unit_datum* unit, cstring data_name, 
													 Enums::hs_type& out_type, bool& out_is_networked, bool& out_readonly)
{
	cstring s = data_name; // alias for keeping the code width down
	out_is_networked = true;
	out_readonly = false;

	out_type = HS_TYPE(bool);
	// designers should use 'unit_get_total_grenade_count' for overall grenade count
		 if( !strcmp(s,"total_grenade_count[frag]") )	return unit->unit.GetGrenadeFragCount();
	else if( !strcmp(s,"total_grenade_count[plasma]") )	return unit->unit.GetGrenadePlasmaCount();
	else if( !strcmp(s,"total_grenade_count[custom2]") )return unit->unit.GetYeloGrenade2Count();
	else if( !strcmp(s,"total_grenade_count[custom3]") )return unit->unit.GetYeloGrenade3Count();
	// Everything below this isn't exactly sync'd...so fuck it
	out_is_networked = false;
		 if( !strcmp(s,"current_grenade_index") )		return unit->unit.GetCurrentGrenadeIndex();
	// designers should NOT set the zoom this way...
	else if( !strcmp(s,"zoom_level") )					return unit->unit.GetZoomLevel();
	// ...they should set it via this
	else if( !strcmp(s,"desired_zoom_level") )			return unit->unit.GetDesiredZoomLevel();

	out_type = HS_TYPE(short);
	out_readonly = true;
		 if( !strcmp(s,"vehicle_seat_index") )		return unit->unit.GetVehicleSeatIndex();
	else if( !strcmp(s,"current_weapon_index") )	return unit->unit.GetCurrentWeaponIndex();
	out_readonly = false;
		 if( !strcmp(s,"feign_death_timer") )		return unit->unit.GetFeignDeathTimer();

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
	result.int32 = NONE;

	if(!args->unit_index.IsNull())
	{
		s_unit_datum* unit = (*Objects::ObjectHeader())[args->unit_index]->Type._unit;

#if 0
		Enums::hs_type result_type;
		bool is_networked;
		bool is_readonly;
		result.pointer = scripting_unit_data_get_integer_by_name(unit, args->data_name, result_type, is_networked, is_readonly);
		Scripting::UpdateTypeHolderFromPtrToData(result, result_type);
#else
		UnitDataGetIntegerByName(unit, args->data_name, result);
#endif
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

#if 0
		TypeHolder result;
		Enums::hs_type result_type;
		bool is_networked;
		bool is_readonly;
		result.pointer = scripting_unit_data_get_integer_by_name(unit, args->data_name, result_type, is_networked, is_readonly);
		// Only set properties in local games, or if the properties are sync'd
		if(Networking::IsLocal() || is_networked)
			Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &args->data_value);
#else
		UnitDataSetIntegerByName(unit, args->data_name, args->data_value);
#endif
	}

	return NULL;
}

static real* scripting_unit_data_get_real_by_name(s_unit_datum* unit, cstring data_name, Enums::hs_type& out_type)
{
	cstring s = data_name; // alias for keeping the code width down

	out_type = HS_TYPE(real);
		 if( !strcmp(s,"camo_power") )		return unit->unit.GetCamoPower();
	else if( !strcmp(s,"driver_power") )	return unit->unit.GetDriverPower();
	else if( !strcmp(s,"gunner_power") )	return unit->unit.GetGunnerPower();
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

#if 0
		Enums::hs_type result_type;
		result.ptr.real = scripting_unit_data_get_real_by_name(unit, args->data_name, result_type);
		Scripting::UpdateTypeHolderFromPtrToData(result, result_type);
#else
		UnitDataGetRealByName(unit, args->data_name, result);
#endif
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

	if(Networking::IsLocal() && !args->unit_index.IsNull())
	{
		s_unit_datum* unit = (*Objects::ObjectHeader())[args->unit_index]->Type._unit;

#if 0
		TypeHolder result;
		Enums::hs_type result_type;
		result.ptr.real = scripting_unit_data_get_real_by_name(unit, args->data_name, result_type);
		Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &args->data_value);
#else
		UnitDataSetRealByName(unit, args->data_name,args->data_value);
#endif
	}

	return NULL;
}