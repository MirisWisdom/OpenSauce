/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

void EncodeObjectDeletionMessage(datum_index object_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MDP_ENCODE_OBJECT_DELETION_MESSAGE);

	if(object_index.IsNull()) return;

	__asm {
		push	edi

		mov		edi, object_index
		call	TEMP_CALL_ADDR

		pop		edi
	}
}

void EncodeHudChatNetworkData(int32 player_number, _enum chat_type, wcstring msg)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(ENCODE_HUD_CHAT_NETWORK_DATA);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
	wchar_t local[255];
	memset(local, 0, sizeof(local));
	wcscpy_s(local, msg);
#else
	cstring local = msg;
#endif

	__asm {
		movsx	eax, chat_type
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
		lea		edx, local
#else
		mov		edx, local
#endif
		push	player_number // really a byte, but you can't push a byte!
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1
	}
}

datum_index TranslateObject(datum_index network_object)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MDP_TRANSLATED_INDEX_TRANSLATE);

	if(network_object.IsNull()) return datum_index::null;

	__asm {
		push	esi

		mov		ecx, network_object
		mov		esi, [GET_DATA_PTR(MESSAGE_DELTA_FIELD_OBJECT_INDEX_PARAMETERS)]
		call	TEMP_CALL_ADDR

		pop		esi
	}
}

datum_index TranslatePlayer(datum_index network_player)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MDP_TRANSLATED_INDEX_TRANSLATE);

	if(network_player.IsNull()) return datum_index::null;

	__asm {
		push	esi

		mov		ecx, network_player
		mov		esi, [GET_DATA_PTR(MESSAGE_DELTA_FIELD_PLAYER_INDEX_PARAMETERS)]
		call	TEMP_CALL_ADDR

		pop		esi
	}
}