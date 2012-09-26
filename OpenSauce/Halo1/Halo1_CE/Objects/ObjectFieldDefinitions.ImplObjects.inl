/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

static real_vector3d* UnitDataFieldGetVector(const s_object_field_definition& field, s_unit_data& unit)
{
	using namespace Enums;

	switch(field.definition_index)
	{
	case _unit_field_vector_desired_facing:		return unit.GetDesiredFacingVector();
	case _unit_field_vector_desired_aiming:		return unit.GetDesiredAimingVector();
	case _unit_field_vector_aiming:				return unit.GetAimingVector();
	case _unit_field_vector_aiming_velocity:	return unit.GetAimingVelocity();
	case _unit_field_vector_looking:			return unit.GetLookingVector();
	case _unit_field_vector_looking_angles:		return CAST_PTR(real_vector3d*, unit.GetLookingAngles());
	case _unit_field_vector_looking_velocity:	return unit.GetLookingVelocity();
	default: return NULL;
	}
}
static real_vector3d* ObjectDataFieldGetVector(const s_object_field_definition& field, s_object_header_datum& obj)
{
	using namespace Enums;

	s_object_data* object = obj._object;
	switch(field.definition_index)
	{
	case _object_field_vector_position:				return CAST_PTR(real_vector3d*, &object->GetNetworkDatumData()->position);
	case _object_field_vector_transitional_velocity:return &object->GetNetworkDatumData()->transitional_velocity;
	case _object_field_vector_forward:				return &object->GetNetworkDatumData()->forward;
	case _object_field_vector_up:					return &object->GetNetworkDatumData()->up;
	case _object_field_vector_angular_velocity:		return &object->GetNetworkDatumData()->angular_velocity;
	}

	if(obj.object_type == Enums::_object_type_biped || obj.object_type == Enums::_object_type_vehicle)
		return UnitDataFieldGetVector(field, obj.Type._unit->unit);

	return NULL;
}

static void ObjectDataFieldGetReal(const s_object_field_definition& field, s_object_header_datum& obj, 
								   TypeHolder& result, int subfield_index)
{
	// NOTE: The only 'real' fields we allow interfacing with are vectors
	real* value_ptr = CAST_PTR(real*, ObjectDataFieldGetVector(field, obj));

	if(!ObjectFieldIndexIsVector(field.definition_index))
		subfield_index = 0;

	result.ptr.real = &value_ptr[subfield_index];
}