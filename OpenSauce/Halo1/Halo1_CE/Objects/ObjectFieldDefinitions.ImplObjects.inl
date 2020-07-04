/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

static real* ObjectDataFieldGetRealImpl(const s_object_field_definition& field, s_object_data& object,
										cstring subdata_name)
{
	using namespace Enums;

	switch(field.definition_index)
	{
	case _object_field_real_function_out:
		{
			int function_index = ObjectFunctionNameToIndex(subdata_name);

			if(function_index >= 0 && function_index < k_number_of_outgoing_object_functions && 
				TEST_FLAG(object.valid_outgoing_functions, function_index))
				return &object.outgoing_function_values[function_index];
		}
		// fall through to default. if we got here, something in the function code didn't compute
	default: return nullptr;

	case _object_field_real_radius:				return &object.radius; break;
	case _object_field_real_scale:				return &object.scale; break;
	case _object_field_real_maximum_vitality: // Keep for backwards compatibility ONLY, value is incorrect
	case _object_field_real_maximum_health:		return &object.damage.maximum_health; break;
	case _object_field_real_current_vitality: // Keep for backwards compatibility ONLY, value is incorrect
	case _object_field_real_maximum_shield:		return &object.damage.maximum_shield; break;
	case _object_field_real_current_health:		return &object.damage.health; break;
	case _object_field_real_current_shield:		return &object.damage.shield; break;
	}
}

static real_vector3d* UnitDataFieldGetVector(const s_object_field_definition& field, s_unit_data& unit)
{
	using namespace Enums;

	switch(field.definition_index)
	{
	case _unit_field_vector_desired_facing:		return &unit.desired_facing_vector;
	case _unit_field_vector_desired_aiming:		return &unit.desired_aiming_vector;
	case _unit_field_vector_aiming:				return &unit.aiming_vector;
	case _unit_field_vector_aiming_velocity:	return &unit.aiming_velocity;
	case _unit_field_vector_looking:			return &unit.looking_vector;
	case _unit_field_vector_looking_angles:		return CAST_PTR(real_vector3d*, &unit.looking_angles);
	case _unit_field_vector_looking_velocity:	return &unit.looking_velocity;
	default: return nullptr;
	}
}
static real_vector3d* ObjectDataFieldGetVector(const s_object_field_definition& field, s_object_header_datum& obj)
{
	using namespace Enums;

	s_object_data* object = obj._object;
	switch(field.definition_index)
	{
	case _object_field_vector_position:				return CAST_PTR(real_vector3d*, &object->position);
	case _object_field_vector_transitional_velocity:return &object->transitional_velocity;
	case _object_field_vector_forward:				return &object->forward;
	case _object_field_vector_up:					return &object->up;
	case _object_field_vector_angular_velocity:		return &object->angular_velocity;
	case _object_field_vector_center:				return CAST_PTR(real_vector3d*, &object->center);
	}

	if(obj.object_type == Enums::_object_type_biped || obj.object_type == Enums::_object_type_vehicle)
		return UnitDataFieldGetVector(field, obj._unit->unit);

	return nullptr;
}

static bool ObjectDataFieldGetReal(const s_object_field_definition& field, s_object_header_datum& obj, 
								   TypeHolder& result, cstring subdata_name)
{
	// NOTE: The only 'real' fields we allow interfacing with are vectors
	real* value_ptr;

	int subfield_index = 0;

	if(ObjectFieldIndexIsVector(field.definition_index))
	{
		subfield_index = VectorFieldNameToIndex(subdata_name, sizeof(real_vector3d));
		value_ptr = CAST_PTR(real*, ObjectDataFieldGetVector(field, obj));
	}
	else
	{
		value_ptr = ObjectDataFieldGetRealImpl(field, *obj._object, subdata_name);
	}

	if(value_ptr != nullptr)
		result.ptr.real = &value_ptr[subfield_index];

	return value_ptr != nullptr;
}