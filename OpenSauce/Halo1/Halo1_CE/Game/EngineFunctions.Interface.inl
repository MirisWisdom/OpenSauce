/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

void KeystoneChatLogAddString(wcstring str)
{
#if !PLATFORM_IS_DEDI
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(KEYSTONE_CHAT_LOG_ADD_STRING);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
	wchar_t local[64];
	memset(local, 0, sizeof(local));
	wcscpy_s(local, str);
#else
	wcstring local = str;
#endif

	__asm {
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
		lea		eax, local
#else
		mov		eax, local
#endif
		push	eax
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1
	}
#endif
}

void HudPrintMessage(wcstring str)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(HUD_PRINT_MESSAGE);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
	wchar_t local[64];
	memset(local, 0, sizeof(local));
	wcscpy_s(local, str);
#else
	wcstring local = str;
#endif

	__asm {
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
		lea		eax, local
#else
		mov		eax, local
#endif
		push	eax
		mov		eax, 0 // player index
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1
	}
}

wcstring HudGetItemMessage(int32 message_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(HUD_GET_ITEM_MESSAGE);

	__asm {
		mov		edx, message_index
		call	TEMP_CALL_ADDR
	}
}

void MainmenuLoad()
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MAINMENU_LOAD);

	GameState::MainGlobals()->map.main_menu_scenario_load = true;

	__asm {
		call	TEMP_CALL_ADDR
	}
}