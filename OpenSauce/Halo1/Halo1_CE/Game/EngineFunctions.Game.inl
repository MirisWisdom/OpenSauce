/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

API_FUNC_NAKED void SwitchBsp(int16 index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(SCENARIO_SWITCH_STRUCTURE_BSP);

	API_FUNC_NAKED_START()
		xor		esi, esi
		mov		si, index
		call	TEMP_CALL_ADDR
	API_FUNC_NAKED_END(1)
}

void PlayVideo(cstring bink)
{
#if !PLATFORM_IS_DEDI
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(BINK_PLAYBACK_START);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
	char local[k_engine_function_string_buffer_size];
	memset(local, 0, k_engine_function_string_buffer_size);
	strcpy_s(local, bink);
#else
	cstring local = bink;
#endif

	__asm {
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
		lea		eax, local
#else
		mov		eax, local
#endif
		push	eax
		call	TEMP_CALL_ADDR
		add		esp, 4
	}
#endif
}

API_FUNC_NAKED void RasterizerMessage(wcstring msg, uint32 flags)
{
#if !PLATFORM_IS_DEDI
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GAME_ENGINE_RASTERIZE_MESSAGE);

	API_FUNC_NAKED_START()
		push	flags
		push	msg
		call	TEMP_CALL_ADDR
	API_FUNC_NAKED_END_CDECL(2)
#endif
}

bool TeamIsEnemy(long_enum team, long_enum team_to_test)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GAME_TEAM_IS_ENEMY);

	__asm {
		mov		ecx, team_to_test
		mov		edx, team
		call	TEMP_CALL_ADDR
	}
}

void PlayMultiplayerSound(_enum multiplayer_sound_index)
{
#if !PLATFORM_IS_DEDI
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GAME_ENGINE_PLAY_MULTIPLAYER_SOUND);

	__asm {
		push	0
		movsx	esi, multiplayer_sound_index
		or		edi, NONE
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1
	}
#endif
}