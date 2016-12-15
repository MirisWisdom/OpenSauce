/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

API_FUNC_NAKED bool HeaderBlockAllocate(datum_index object_index, size_t block_reference_offset, size_t _size)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_HEADER_BLOCK_ALLOCATE);

	API_FUNC_NAKED_START()
		push	_size
		push	block_reference_offset
		mov		eax, object_index
		call	FUNCTION
		add		esp, 4 * 2
	API_FUNC_NAKED_END(3)
}

API_FUNC_NAKED Yelo::Objects::s_object_data* IteratorNext(Yelo::Objects::s_object_iterator& iter)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_ITERATOR_NEXT);

	API_FUNC_NAKED_START()
		push	iter
		call	FUNCTION
	API_FUNC_NAKED_END_CDECL(1)
}

void PlacementDataNew(Yelo::Objects::s_object_placement_data& data, datum_index object_definition_index, datum_index owner_object_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_PLACEMENT_DATA_NEW);

	if(object_definition_index.IsNull()) return;

	__asm {
		push	owner_object_index
		push	object_definition_index
		mov		eax, data
		call	FUNCTION
		add		esp, 4 * 2
	}
}

datum_index New(Yelo::Objects::s_object_placement_data& data)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_NEW);

	__asm {
		mov		ecx, data
		call	FUNCTION
	}
}

datum_index NewWithRole(Yelo::Objects::s_object_placement_data& data, long_enum /*Enums::networked_datum*/ role)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_NEW_WITH_ROLE);

	__asm {
		push	role
		push	data
		call	FUNCTION
		add		esp, 4 * 2
	}
}

API_FUNC_NAKED void Delete(datum_index object_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_DELETE);

	API_FUNC_NAKED_START()
		mov		eax, object_index
		call	FUNCTION
	API_FUNC_NAKED_END(1)
}

API_FUNC_NAKED void Attach(datum_index target_object_index, cstring target_marker_name, datum_index object_index, cstring marker_name)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_ATTACH_TO_MARKER);

	API_FUNC_NAKED_START()
		push	esi
		push	edi

		mov     edi, marker_name
		mov		esi, object_index
		mov     ecx, target_marker_name
		push    ecx
		mov		edx, target_object_index
		push    edx
		call	FUNCTION
		add		esp, 4 * 2

		pop		edi
		pop		esi
	API_FUNC_NAKED_END(4)
}

API_FUNC_NAKED void Detach(datum_index object_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_DETACH);

	API_FUNC_NAKED_START()
		push	object_index
		call	FUNCTION
	API_FUNC_NAKED_END_CDECL(1)
}

API_FUNC_NAKED int16 FindInSphere(Flags::objects_find_flags find_flags, long_flags object_type_flags, 
					const s_scenario_location& location, const real_point3d& center, real radius, 
					datum_index object_indices[], int16 maximum_object_indices)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECTS_FIND_IN_SPHERE);

	API_FUNC_NAKED_START()
		push	maximum_object_indices
		push	object_indices
		push	radius
		push	center
		push	location
		push	object_type_flags
		push	find_flags
		call	FUNCTION
	API_FUNC_NAKED_END_CDECL(7)
}


void StartInterpolation(datum_index object_index, int32 interpolation_ticks) // Until PY's netcode is finalized, this function don't mean shit for MP games (read: doesn't sync)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_START_INTERPOLATION);

	if(object_index.IsNull()) return;

	__asm {
		mov		eax, object_index
		mov		edx, interpolation_ticks
		call	FUNCTION
	}
}

void Reset(datum_index object_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_RESET);

	if(object_index.IsNull()) return;

	__asm {
		push	ebx

		mov		ebx, object_index
		call	FUNCTION

		pop		ebx
	}
}

void ReconnectToMap(datum_index object_index, s_scenario_location* location_reference)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_RECONNECT_TO_MAP);

	if(object_index.IsNull()) return;

	__asm {
		push	location_reference
		push	object_index
		call	FUNCTION
		add		esp, 4 * 2
	}
}

void DisconnectFromMap(datum_index object_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_DISCONNECT_FROM_MAP);

	if(object_index.IsNull()) return;

	__asm {
		push	object_index
		call	FUNCTION
		add		esp, 4 * 1
	}
}

real_point3d& GetOrigin(datum_index object_index, real_point3d& out_origin)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_GET_ORIGIN);

	if(object_index.IsNull()) return out_origin;

	__asm {
		mov		eax, out_origin
		mov		ecx, object_index
		call	FUNCTION
	}
}

void GetOrientation(datum_index object_index, real_vector3d* out_forward, real_vector3d* out_up)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_GET_ORIENTATION);

	if(object_index.IsNull()) return;

	__asm {
		mov		ecx, object_index
		mov		eax, out_forward
		push	out_up
		call	FUNCTION
		add		esp, 4 * 1
	}
}

s_scenario_location& GetLocation(datum_index object_index, s_scenario_location& out_location)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_GET_LOCATION);

	if(object_index.IsNull()) return out_location;

	__asm {
		push	edi

		mov		edi, object_index
		mov		eax, out_location
		call	FUNCTION

		pop		edi
	}
}

void SetPosition(datum_index object_index, real_point3d* new_pos, real_vector3d* new_forward, real_vector3d* new_up)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_SET_POSITION);

	if(object_index.IsNull()) return;

	__asm {
		push	edi

		mov		edi, new_pos
		push	new_up
		push	new_forward
		push	object_index
		call	FUNCTION
		add		esp, 4 * 3

		pop		edi
	}
}

void SetPositionNetwork(datum_index object_index, real_point3d* new_pos)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_SET_POSITION_NETWORK);

	if(object_index.IsNull()) return;

	__asm {
		push	edi
		push	esi

		mov		edi, object_index
		mov		esi, new_pos
		call	FUNCTION

		pop		esi
		pop		edi
	}
}

bool RestoreBody(datum_index object_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_RESTORE_BODY);

	if(object_index.IsNull()) return false;

	__asm {
		mov		eax, object_index
		call	FUNCTION
	}
}

void DepleteBody(datum_index object_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_DEPLETE_BODY);

	if(object_index.IsNull()) return;

	__asm {
		mov		eax, object_index
		call	FUNCTION
	}
}

void DepleteShield(datum_index object_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_DEPLETE_SHIELD);

	if(object_index.IsNull()) return;

	__asm {
		push	edi

		mov		edi, object_index
		call	FUNCTION

		pop		edi
	}
}

void DoubleChargeShield(datum_index object_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_DOUBLE_CHARGE_SHIELD);

	if(object_index.IsNull()) return;

	__asm {
		mov		eax, object_index
		call	FUNCTION
	}
}

void ObjectCauseDamage(Yelo::Objects::s_damage_data& damage_data, datum_index object_index, int32 node_index, int32 region_index, int32 damage_materials_index, real_vector3d* normal)
{
	typedef void (PLATFORM_API *object_cause_damage_t)(Yelo::Objects::s_damage_data&, datum_index, int32, int32, int32, real_vector3d*);
	static const object_cause_damage_t FUNCTION = CAST_PTR(object_cause_damage_t, GET_FUNC_PTR(OBJECT_CAUSE_DAMAGE));

	FUNCTION(damage_data, object_index, node_index, region_index, damage_materials_index, normal);
}

void DefinitionPredict(datum_index object_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_GET_CAMERA_POSITION);

	if(object_index.IsNull()) return;

	__asm {
		mov		eax, object_index
		call	FUNCTION
	}
}

API_FUNC_NAKED void SetScale(datum_index object_index, real scale, int32 ticks)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_SET_SCALE);

	API_FUNC_NAKED_START()
		push    ticks
		push    scale
		mov		eax, object_index
		call	FUNCTION
		add     esp, 4 * 2
	API_FUNC_NAKED_END(3)
}

void ObjectOrient(datum_index unit_index, int32 cutscene_flag_index, bool teleport, bool set_facing)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(HS_OBJECT_ORIENT);

	if(unit_index.IsNull()) return;

	__asm {
		movzx	eax, set_facing
		push	eax
		movzx	eax, teleport
		push	eax
		mov		eax, cutscene_flag_index
		push	unit_index
		call	FUNCTION
		add		esp, 4 * 3
	}
}

void UnitGetCameraPosition(datum_index unit_index, real_point3d* out_position)
{
#if !PLATFORM_IS_DEDI
	static const uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_GET_CAMERA_POSITION);

	if(unit_index.IsNull()) return;

	__asm {
		push	edi

		mov		edi, out_position
		mov		ecx, unit_index
		call	FUNCTION

		pop		edi
	}
#else
	memset(out_position, 0, sizeof(*out_position));
#endif
}

void UnitSetAnimation(datum_index unit_index, datum_index animation_graph_index, int32 animation_index)
{
	// Code is in the units.c code region but only touches s_object_datum data, so technically unit_index 
	// *could* safely be an generic object_index.
	static const uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_SET_ANIMATION);

	if(unit_index.IsNull()) return;

	__asm {
		push	animation_index
		push	animation_graph_index
		mov		eax, unit_index
		call	FUNCTION
		add		esp, 4 * 2
	}
}

API_FUNC_NAKED int16 UnitGetCustomAnimationTime(datum_index unit_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_GET_CUSTOM_ANIMATION_TIME);

	API_FUNC_NAKED_START()
		mov		eax, unit_index
		call	FUNCTION
	API_FUNC_NAKED_END(1)
}

API_FUNC_NAKED bool UnitCanEnterSeat(datum_index unit_index, datum_index vehicle_index, int32 vehicle_seat_index, datum_index* unit_in_seat)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_CAN_ENTER_SEAT);

	API_FUNC_NAKED_START()
		push    unit_in_seat
		push    vehicle_seat_index
		mov		edx, vehicle_index
		mov     eax, unit_index
		call    FUNCTION
		add     esp, 4 * 2
	API_FUNC_NAKED_END(4)
}

API_FUNC_NAKED bool UnitEnterSeat(datum_index unit_index, datum_index vehicle_index, int32 vehicle_seat_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_ENTER_SEAT);

	API_FUNC_NAKED_START()
		push    vehicle_seat_index
		push    vehicle_index
		mov     eax, unit_index
		call    FUNCTION
		add     esp, 4 * 2
	API_FUNC_NAKED_END(3)
}

API_FUNC_NAKED void UnitExitVehicle(datum_index unit_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_EXIT_VEHICLE);

	API_FUNC_NAKED_START()
		mov     eax, unit_index
		call    FUNCTION
	API_FUNC_NAKED_END(1)
}

API_FUNC_NAKED void UnitOpen(datum_index unit_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_OPEN);

	API_FUNC_NAKED_START()
			mov     eax, unit_index
			call    FUNCTION
	API_FUNC_NAKED_END(1)
}

API_FUNC_NAKED void UnitClose(datum_index unit_index)
{
    static const uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_CLOSE);

	API_FUNC_NAKED_START()
            mov     eax, unit_index
            call    FUNCTION
	API_FUNC_NAKED_END(1)
}

API_FUNC_NAKED int16 UnitFindNearbySeat(datum_index unit_index, datum_index vehicle_index, int16 &seat_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_FIND_NEARBY_SEAT);

	API_FUNC_NAKED_START()
		push	seat_index
		push	vehicle_index
		push	unit_index
		call	FUNCTION
		add		esp, 4 * 3
	API_FUNC_NAKED_END(3)
}

API_FUNC_NAKED void UnitExitSeatEnd(datum_index unit_index, bool unk2, bool unk3, bool unk4)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_EXIT_SEAT_END);

	API_FUNC_NAKED_START()
		movzx	eax, unk4
		push	eax
		movzx	eax, unk3
		push	eax
		movzx	eax, unk2
		push	eax
		push	unit_index
		call    FUNCTION
		add		esp, 4 * 4
	API_FUNC_NAKED_END(4)
}

API_FUNC_NAKED real GetLevelOfDetailPixels(datum_index object_index)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_GET_LEVEL_OF_DETAIL_PIXELS);

	API_FUNC_NAKED_START()
		mov		eax, object_index
		call	FUNCTION
	API_FUNC_NAKED_END(1)
}

API_FUNC_NAKED void ObjectRenderStateRefresh(datum_index object_render_state_index, datum_index object_index, real level_of_detail_pixels, int arg4)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_RENDER_STATE_REFRESH);

	API_FUNC_NAKED_START()
		push	arg4
		push	level_of_detail_pixels
		push	object_index
		push	object_render_state_index
		call	FUNCTION
		add		esp, 4 * 4
	API_FUNC_NAKED_END(4)
}
