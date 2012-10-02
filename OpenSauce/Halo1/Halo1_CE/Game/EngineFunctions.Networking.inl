/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

void EncodeObjectDeletionMessage(datum_index object_index)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(MDP_ENCODE_OBJECT_DELETION_MESSAGE);

	if(object_index.IsNull()) return;

	__asm {
		push	edi

		mov		edi, object_index
		call	CALL_ADDR

		pop		edi
	}
}

void EncodeHudChatNetworkData(int32 player_number, _enum chat_type, wcstring msg)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(ENCODE_HUD_CHAT_NETWORK_DATA);

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
		call	CALL_ADDR
		add		esp, 4 * 1
	}
}

API_FUNC_NAKED datum_index TranslateIndex(MessageDeltas::field_properties_definition* properties_definition, datum_index network_index)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(MDP_TRANSLATED_INDEX_TRANSLATE);

	API_FUNC_NAKED_START()
		push	esi

		mov		ecx, network_index
		mov		esi, properties_definition
		call	CALL_ADDR

		pop		esi
	API_FUNC_NAKED_END(2)
}

datum_index TranslateObject(datum_index network_object)
{
	if(network_object.IsNull()) return datum_index::null;

	__asm {
		push	network_object
		push	[GET_DATA_PTR(MESSAGE_DELTA_FIELD_OBJECT_INDEX_PARAMETERS)]
		call	TranslateIndex
	}
}

datum_index TranslatePlayer(datum_index network_player)
{
	if(network_player.IsNull()) return datum_index::null;

	__asm {
		push	network_player
		push	[GET_DATA_PTR(MESSAGE_DELTA_FIELD_PLAYER_INDEX_PARAMETERS)]
		call	TranslateIndex
	}
}

#if !PLATFORM_IS_DEDI

API_FUNC_NAKED void ConnectToServer(cstring address, cstring password)
{
	static const uintptr_t CALL_ADDR = GET_FUNC_PTR(CONNECT_TO_MP_SERVER);

	API_FUNC_NAKED_START()
		push	password
		push	address
		call	CALL_ADDR
	API_FUNC_NAKED_END_CDECL(2)
}

#endif