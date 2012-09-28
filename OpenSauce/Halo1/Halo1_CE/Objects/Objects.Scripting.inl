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

		ObjectDataGetRealByName(object, args->data_name, args->subdata_name, result);
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
		s_object_header_datum* object = (*Objects::ObjectHeader())[args->object_index];

		ObjectDataSetRealByName(object, args->data_name, args->subdata_name, args->data_value);
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

	if(!args->object_index.IsNull())
	{
		s_object_header_datum* object = (*Objects::ObjectHeader())[args->object_index];

		real_vector3d* obj_vector = ObjectDataGetVectorByName(object, args->data_name);
		const real_vector3d* vector = GameState::RuntimeData::VectorValueGet(args->vector_index);
		if(obj_vector != NULL && vector != NULL)
		{
			*obj_vector = *vector;
			result.boolean = true;
		}
	}

	return result.pointer;
}
static void* scripting_object_data_save_vector_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index object_index;
		cstring data_name;
		int16 vector_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	if(!args->object_index.IsNull())
	{
		s_object_header_datum* object = (*Objects::ObjectHeader())[args->object_index];

		real_vector3d* obj_vector = ObjectDataGetVectorByName(object, args->data_name);
		real_vector3d* vector = GameState::RuntimeData::VectorValueGetForModify(args->vector_index);
		if(obj_vector != NULL && vector != NULL)
		{
			*vector = *obj_vector;
			result.boolean = true;
		}
	}

	return result.pointer;
}


//////////////////////////////////////////////////////////////////////////
// WEAPONS

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

			WeaponDataGetRealByName(weapon, args->data_name, result);
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

			WeaponDataSetRealByName(weapon, args->data_name, args->data_value);
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// WEAPONS - TRIGGERS

static void* scripting_weapon_data_trigger_set_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index weapon_index;
		int32 trigger_index;
		cstring data_name;
		cstring subdata_name;
		real data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	// IF ANY OF YOUR DESIGNERS USE THIS FUCKING SCRIPT FUNCTION, THEN YOU ALL DESERVE TO BE FUCKING SHOT.

	// SRSLY.

	// We don't support modifying trigger data in anything but local games because it writes to tag memory
	if(Networking::IsLocal() && !args->weapon_index.IsNull())
	{
		s_object_header_datum* header = (*Objects::ObjectHeader())[args->weapon_index];

		if(header->object_type == Enums::_object_type_weapon)
		{
			s_weapon_datum* weapon = header->Type._weapon;

			WeaponTagDataTriggerSetRealByName(weapon, args->trigger_index, args->data_name, args->subdata_name, args->data_value);
		}
	}

	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// UNITS

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

		UnitDataGetObjectIndexByName(unit, args->data_name, result);
	}

	return result.pointer;
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

		UnitDataGetIntegerByName(unit, args->data_name, result);
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

		UnitDataSetIntegerByName(unit, args->data_name, args->data_value);
	}

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

		UnitDataGetRealByName(unit, args->data_name, result);
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

		UnitDataSetRealByName(unit, args->data_name,args->data_value);
	}

	return NULL;
}