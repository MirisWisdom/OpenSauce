/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

void PlacementDataNew(Yelo::Objects::s_object_placement_data& data, datum_index object_definition_index, datum_index owner_object_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_PLACEMENT_DATA_NEW);

	if(object_definition_index.IsNull()) return;

	__asm {
		push	owner_object_index
		push	object_definition_index
		mov		eax, data
		call	TEMP_CALL_ADDR
		add		esp, 4 * 2
	}
}

datum_index New(Yelo::Objects::s_object_placement_data& data)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_NEW);

	__asm {
		mov		ecx, data
		call	TEMP_CALL_ADDR
	}
}

datum_index NewWithRole(Yelo::Objects::s_object_placement_data& data, long_enum /*Enums::networked_datum*/ role)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_NEW_WITH_ROLE);

	__asm {
		push	role
		push	data
		call	TEMP_CALL_ADDR
		add		esp, 4 * 2
	}
}

void API_FUNC_NAKED Delete(datum_index object_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_DELETE);

	NAKED_FUNC_START()
		mov		eax, object_index
		call	TEMP_CALL_ADDR
	NAKED_FUNC_END(1)
}

void API_FUNC_NAKED Attach(datum_index target_object_index, cstring target_marker_name, datum_index object_index, cstring marker_name)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_ATTACH_TO_MARKER);

	NAKED_FUNC_START()
		push	esi
		push	edi

		mov     edi, marker_name
		mov		esi, object_index
		mov     ecx, target_marker_name
		push    ecx
		mov		edx, target_object_index
		push    edx
		call	TEMP_CALL_ADDR
		add		esp, 4 * 2

		pop		edi
		pop		esi
	NAKED_FUNC_END(4)
}

void API_FUNC_NAKED Detach(datum_index object_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_DETACH);

	NAKED_FUNC_START()
		push	object_index
		call	TEMP_CALL_ADDR
	NAKED_FUNC_END_CDECL(1)
}


void StartInterpolation(datum_index object_index, int32 interpolation_ticks) // Until PY's netcode is finalized, this function don't mean shit for MP games (read: doesn't sync)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_START_INTERPOLATION);

	if(object_index.IsNull()) return;

	__asm {
		mov		eax, object_index
		mov		edx, interpolation_ticks
		call	TEMP_CALL_ADDR
	}
}

void Reset(datum_index object_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_RESET);

	if(object_index.IsNull()) return;

	__asm {
		push	ebx

		mov		ebx, object_index
		call	TEMP_CALL_ADDR

		pop		ebx
	}
}

void ReconnectToMap(datum_index object_index, s_scenario_location* location_reference)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_RECONNECT_TO_MAP);

	if(object_index.IsNull()) return;

	__asm {
		push	location_reference
		push	object_index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 2
	}
}

void DisconnectFromMap(datum_index object_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_DISCONNECT_FROM_MAP);

	if(object_index.IsNull()) return;

	__asm {
		push	object_index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1
	}
}

void GetOrigin(datum_index object_index, real_point3d* out_origin)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_GET_ORIGIN);

	if(object_index.IsNull()) return;

	__asm {
		mov		eax, out_origin
		mov		ecx, object_index
		call	TEMP_CALL_ADDR
	}
}

void GetOrientation(datum_index object_index, real_vector3d* out_forward, real_vector3d* out_up)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_GET_ORIENTATION);

	if(object_index.IsNull()) return;

	__asm {
		mov		ecx, object_index
		mov		eax, out_forward
		push	out_up
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1
	}
}

s_scenario_location* GetLocation(datum_index object_index, s_scenario_location* out_location)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_GET_LOCATION);

	if(object_index.IsNull()) return out_location;

	__asm {
		push	edi

		mov		edi, object_index
		mov		eax, out_location
		call	TEMP_CALL_ADDR

		pop		edi
	}
}

void SetPosition(datum_index object_index, real_point3d* new_pos, real_vector3d* new_forward, real_vector3d* new_up)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_SET_POSITION);

	if(object_index.IsNull()) return;

	__asm {
		push	edi

		mov		edi, new_pos
		push	new_up
		push	new_forward
		push	object_index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 3

		pop		edi
	}
}

void SetPositionNetwork(datum_index object_index, real_point3d* new_pos)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_SET_POSITION_NETWORK);

	if(object_index.IsNull()) return;

	__asm {
		push	edi
		push	esi

		mov		edi, object_index
		mov		esi, new_pos
		call	TEMP_CALL_ADDR

		pop		esi
		pop		edi
	}
}

bool RestoreBody(datum_index object_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_RESTORE_BODY);

	if(object_index.IsNull()) return false;

	__asm {
		mov		eax, object_index
		call	TEMP_CALL_ADDR
	}
}

void DepleteBody(datum_index object_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_DEPLETE_BODY);

	if(object_index.IsNull()) return;

	__asm {
		mov		eax, object_index
		call	TEMP_CALL_ADDR
	}
}

void DepleteShield(datum_index object_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_DEPLETE_SHIELD);

	if(object_index.IsNull()) return;

	__asm {
		push	edi

		mov		edi, object_index
		call	TEMP_CALL_ADDR

		pop		edi
	}
}

void DoubleChargeShield(datum_index object_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_DOUBLE_CHARGE_SHIELD);

	if(object_index.IsNull()) return;

	__asm {
		mov		eax, object_index
		call	TEMP_CALL_ADDR
	}
}

void DefinitionPredict(datum_index object_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(UNIT_GET_CAMERA_POSITION);

	if(object_index.IsNull()) return;

	__asm {
		mov		eax, object_index
		call	TEMP_CALL_ADDR
	}
}

void API_FUNC_NAKED SetScale(datum_index object_index, real scale, int32 ticks)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_SET_SCALE);

	NAKED_FUNC_START()
		push    ticks
		push    scale
		mov		eax, object_index
		call	TEMP_CALL_ADDR
		add     esp, 4 * 2
	NAKED_FUNC_END(3)
}

void UnitOrientToCutsceneFlag(datum_index unit_index, int32 cutscene_flag_index, bool set_facing, bool i_dont_know)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(UNIT_ORIENT_TO_FLAG);

	if(unit_index.IsNull()) return;

	__asm {
		movzx	eax, set_facing
		push	eax
		movzx	eax, i_dont_know
		push	eax
		mov		eax, cutscene_flag_index
		push	unit_index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 3
	}
}

void UnitGetCameraPosition(datum_index unit_index, real_point3d* out_position)
{
#if !PLATFORM_IS_DEDI
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(UNIT_GET_CAMERA_POSITION);

	if(unit_index.IsNull()) return;

	__asm {
		push	edi

		mov		edi, out_position
		mov		ecx, unit_index
		call	TEMP_CALL_ADDR

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
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(UNIT_SET_ANIMATION);

	if(unit_index.IsNull()) return;

	__asm {
		push	animation_index
		push	animation_graph_index
		mov		eax, unit_index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 2
	}
}

int16 API_FUNC_NAKED UnitGetCustomAnimationTime(datum_index unit_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(UNIT_GET_CUSTOM_ANIMATION_TIME);

	NAKED_FUNC_START()
		mov		eax, unit_index
		call	TEMP_CALL_ADDR
	NAKED_FUNC_END(1)
}