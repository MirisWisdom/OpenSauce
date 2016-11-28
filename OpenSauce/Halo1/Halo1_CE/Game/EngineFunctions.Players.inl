/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

datum_index IndexFromUnitIndex(datum_index unit_index)
{
	static const uintptr_t TEMP_CALL_ADDR = GET_FUNC_PTR(PLAYER_INDEX_FROM_UNIT_INDEX);

	if(unit_index.IsNull()) return datum_index::null;

	__asm {
		push	unit_index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1
	}
}

bool Teleport(datum_index player_index, const real_point3d& position, datum_index source_unit_index)
{
	static const uintptr_t TEMP_CALL_ADDR = GET_FUNC_PTR(PLAYER_TELEPORT);

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
