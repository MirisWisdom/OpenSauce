/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

API_FUNC_NAKED void AllWeapons()
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CHEAT_ALL_WEAPONS);

	__asm {
		call	TEMP_CALL_ADDR
		retn
	}
}

API_FUNC_NAKED void SpawnWarthog()
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CHEAT_SPAWN_WARTHOG);

	__asm {
		call	TEMP_CALL_ADDR
		retn
	}
}

API_FUNC_NAKED void TeleportToCamera()
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CHEAT_TELEPORT_TO_CAMERA);

	__asm {
		call	TEMP_CALL_ADDR
		retn
	}
}

API_FUNC_NAKED void ActiveCamoflage()
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CHEAT_ACTIVE_CAMOFLAGE);

	__asm {
		call	TEMP_CALL_ADDR
		retn
	}
}

API_FUNC_NAKED void ActiveCamoflageLocalPlayer()
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CHEAT_ACTIVE_CAMOFLAGE_LOCAL_PLAYER);

	__asm {
		call	TEMP_CALL_ADDR
		retn
	}
}

API_FUNC_NAKED datum_index PlayerIndex()
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CHEAT_PLAYER_INDEX);

	__asm {
		call	TEMP_CALL_ADDR
		retn
	}
}

API_FUNC_NAKED void DirectorSaveCamera()
{
#if !PLATFORM_IS_DEDI
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DIRECTOR_SAVE_CAMERA);

	__asm {
		call	TEMP_CALL_ADDR
		retn
	}
#endif
}

API_FUNC_NAKED void DirectorLoadCamera()
{
#if !PLATFORM_IS_DEDI
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DIRECTOR_LOAD_CAMERA);

	__asm {
		call	TEMP_CALL_ADDR
		retn
	}
#endif
}