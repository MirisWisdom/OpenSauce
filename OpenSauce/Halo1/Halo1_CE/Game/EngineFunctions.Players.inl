/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

datum_index IndexFromUnitIndex(datum_index unit_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(PLAYER_INDEX_FROM_UNIT_INDEX);

	if(unit_index.IsNull()) return datum_index::null;

	__asm {
		push	unit_index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1
	}
}

bool Teleport(datum_index player_index, const real_point3d& position, datum_index source_unit_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(PLAYER_TELEPORT);

	if(player_index.IsNull()) return false;

	__asm {
		push	position
		push	source_unit_index
		push	player_index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 3
	}
}


static void player_screen_effect(datum_index player_index, uint32 func)
{
#if !PLATFORM_IS_DEDI
	__asm {
		mov		edx, player_index
		mov		eax, func
		call	eax
	}
#endif
}

void ScreenEffectOvershield(datum_index player_index)
{
#if !PLATFORM_IS_DEDI
	if(player_index.IsNull()) return;
	player_screen_effect(player_index, GET_FUNC_PTR(PLAYER_OVER_SHIELD_SCREEN_EFFECT));
#endif
}

void ScreenEffectCamo(datum_index player_index)
{
#if !PLATFORM_IS_DEDI
	if(player_index.IsNull()) return;
	player_screen_effect(player_index, GET_FUNC_PTR(PLAYER_CAMO_SCREEN_EFFECT));
#endif
}

void ScreenEffectHealth(datum_index player_index)
{
#if !PLATFORM_IS_DEDI
	if(player_index.IsNull()) return;
	player_screen_effect(player_index, GET_FUNC_PTR(PLAYER_HEALTH_PACK_SCREEN_EFFECT));
#endif
}

datum_index FindClosestPlayerIndex(datum_index player_index)
{
#if !PLATFORM_IS_DEDI
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(FIND_CLOSEST_PLAYER_INDEX);

	if(player_index.IsNull()) return datum_index::null;

	__asm {
		push	player_index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1
	}
#else
	return datum_index::null;
#endif
}

void PlayerExamineNearbyVehicle(datum_index player_index, datum_index vehicle_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(PLAYER_EXAMINE_NEARBY_VEHICLE);

	__asm {
		push	vehicle_index
		push	player_index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 2
	}
}

void PlayerSetActionResult(datum_index player_index, datum_index action_object_index, int32 action_result, int32 action_seat_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(PLAYER_SET_ACTION_RESULT);

	__asm {
		push	action_seat_index
		push	action_result
		mov		ebx, action_object_index
		mov		eax, player_index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 2
	}
}

void PlayerSetActionResultToNetwork(datum_index player_index, datum_index action_object_index, int32 unknown, int32 action_result, int32 action_seat_index, datum_index next_weapon_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(PLAYER_SET_ACTION_RESULT_TO_NETWORK);

	__asm {
		push	next_weapon_index
		push	action_seat_index
		push	action_result
		push	unknown		// doesn't even look like the function uses this
		mov		edi, action_object_index
		mov		ecx, player_index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 4
	}
}