/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

void GatherException(const void* data, int32 arg_0, int32 arg_4, int32 arg_8)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GATHER_EXCEPTION);

	GET_PTR(gather_exception_data) = data;

	__asm {
		push	arg_8
		push	arg_4
#if PLATFORM_IS_DEDI
		mov		ecx, arg_0
#else
		push	arg_0
#endif
		call	TEMP_CALL_ADDR
#if PLATFORM_IS_DEDI
		add		esp, 4 * 2
#else
		add		esp, 4 * 3
#endif
	}
}

IDirect3DBaseTexture9** TextureCacheRequestTexture(Yelo::TagGroups::s_bitmap_data* bitmap, 
												   bool add_to_cache, bool block_thread)
{
#if !PLATFORM_IS_DEDI
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(TEXTURE_CACHE_TEXTURE_REQUEST);

	__asm {
		xor		eax, eax
		mov		al, add_to_cache
		push	eax
		mov		al, block_thread
		push	eax
		mov		eax, bitmap
		call	TEMP_CALL_ADDR
		add		esp, 4 * 2
	}
#else
	return NULL;
#endif
}

bool SoundCacheRequestSound(Yelo::TagGroups::s_sound_permutation* sound_perm, 
							bool add_to_cache, bool block_thread, bool unknown2)
{
#if !PLATFORM_IS_DEDI
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(SOUND_CACHE_SOUND_REQUEST);

	__asm {
		push	edi
		push	ebx

		xor		eax, eax
		mov		al, unknown2
		push	eax
		mov		al, add_to_cache
		push	eax
		xor		ebx, ebx
		mov		bl, block_thread
		mov		edi, sound_perm
		call	TEMP_CALL_ADDR
		add		esp, 4 * 2

		pop		ebx
		pop		edi
	}
#else
	return false;
#endif
}

void MapListAddMap(cstring map_name, int32 map_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MAP_LIST_ADD_MAP);

	__asm {
		push	map_index
		mov		eax, map_name
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1
	}
}
void RasterizerAddResolution(uint32 width, uint32 height, uint32 refresh_rate)
{
#if !PLATFORM_IS_DEDI
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(RESOLUTION_LIST_ADD_RESOLUTION);

	_asm{
		mov		ecx, width
		mov		eax, height
		mov		edx, refresh_rate
		push	edx
		push	ecx
		call	TEMP_CALL_ADDR
		add		esp, 4 * 2
	}
#endif
}

int16 API_FUNC_NAKED AnimationPickRandomPermutation(bool animation_update_kind_affects_game_state, datum_index animation_graph_index, int32 animation_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(ANIMATION_PICK_RANDOM_PERMUTATION);

	API_FUNC_NAKED_START()
		push	ecx
		movzx	ecx, animation_update_kind_affects_game_state

		mov		edx, animation_index
		mov		eax, animation_graph_index
		push	ecx
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1

		pop		ecx
	API_FUNC_NAKED_END(3)
}

void API_FUNC_NAKED GenerateMD5(cstring data, const DWORD data_length, cstring output)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GENERATE_MD5);

	API_FUNC_NAKED_START()
		push	output
		push	data_length
		push	data
		call	TEMP_CALL_ADDR
	API_FUNC_NAKED_END_CDECL(3);
}