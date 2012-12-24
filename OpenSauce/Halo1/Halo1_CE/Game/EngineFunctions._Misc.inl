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

void SetTextureSamplerStage(Yelo::TagGroups::s_bitmap_data* bitmap, uint32 texture_stage)
{
	// use this instead of TextureCacheRequestTexture where applicable as this function
	// tests whether the texture is already loaded and sets the texture sampler itself

#if !PLATFORM_IS_DEDI
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(SET_TEXTURE_SAMPLER_STAGE);

	_asm
	{
		mov		esi, bitmap
		push	texture_stage
		call	TEMP_CALL_ADDR
		add		esp, 4
	}
#endif
}

IDirect3DBaseTexture9** TextureCacheBitmapGetHardwareFormat(Yelo::TagGroups::s_bitmap_data* bitmap, 
												   bool block_thread, bool load)
{
#if !PLATFORM_IS_DEDI
	static const uintptr_t FUNCTION = GET_FUNC_PTR(TEXTURE_CACHE_BITMAP_GET_HARDWARE_FORMAT);

	__asm {
		xor		eax, eax
		mov		al, load
		push	eax
		mov		al, block_thread
		push	eax
		mov		eax, bitmap
		call	FUNCTION
		add		esp, 4 * 2
	}
#else
	return NULL;
#endif
}

bool SoundCacheRequestSound(Yelo::TagGroups::s_sound_permutation* sound_perm, 
							bool block_thread, bool load, bool reference)
{
#if !PLATFORM_IS_DEDI
	static const uintptr_t FUNCTION = GET_FUNC_PTR(SOUND_CACHE_SOUND_REQUEST);

	__asm {
		push	edi
		push	ebx

		xor		eax, eax
		mov		al, reference
		push	eax
		mov		al, load
		push	eax
		xor		ebx, ebx
		mov		bl, block_thread
		mov		edi, sound_perm
		call	FUNCTION
		add		esp, 4 * 2

		pop		ebx
		pop		edi
	}
#else
	return false;
#endif
}

void MapListAddMap(cstring map_name, cstring extension, bool skip_crc, int32 map_index)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MAP_LIST_ADD_MAP);
	static void* SKIP_CRC_JZ_ADDRESS = GET_PTR2(MAP_LIST_ADD_SKIP_CRC_JZ);
	static void* MAP_LIST_EXTENSION_REF = GET_PTR2(MAP_LIST_MAP_EXTENSION_REF);
	static const char* MAP_LIST_EXTENSION_STOCK = GET_PTR2(MAP_LIST_MAP_EXTENSION);

	__asm {
		cmp		skip_crc, 0
		jz		skip_crc_disable
		push	eax
		mov		eax, SKIP_CRC_JZ_ADDRESS
		mov		byte ptr [eax], 0xEB
		pop		eax
skip_crc_disable:

		push	eax
		push	ebx
		mov		eax, MAP_LIST_EXTENSION_REF
		mov		ebx, extension
		mov		dword ptr [eax], ebx
		pop		ebx
		pop		eax

		push	map_index
		mov		eax, map_name
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1

		push	eax
		push	ebx
		mov		eax, MAP_LIST_EXTENSION_REF
		mov		ebx, MAP_LIST_EXTENSION_STOCK
		mov		dword ptr [eax], ebx
		pop		ebx
		pop		eax

		cmp		skip_crc, 0
		jz		skip_crc_enable
		push	eax
		mov		eax, SKIP_CRC_JZ_ADDRESS
		mov		byte ptr [eax], 0x74
		pop		eax
skip_crc_enable:
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

bool CompareMD5(cstring data, const DWORD data_length, cstring comparison_md5)
{
	char data_md5[33];

	data_md5[0] = 0;
	GenerateMD5(data, data_length, data_md5);

	return _stricmp(data_md5, comparison_md5) == 0;
}

bool GetCmdLineParameter(cstring parameter, cstring* value_out)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GET_CMD_LINE_PARAMETER);

	_asm
	{
		push	edi
		push	parameter
		mov		edi, value_out
		call	TEMP_CALL_ADDR
		add		esp, 4
		pop		edi
	};
}

API_FUNC_NAKED bool CacheFileReadRequest(/*datum_index tag_index,*/
						  uint32 offset_, uint32 size_, void* buffer, const Cache::s_cache_file_request_params& params, 
						  bool block, Enums::cache_file_request_source source)
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(CACHE_FILE_READ_REQUEST);

	API_FUNC_NAKED_START()
		push	esi
		xor		eax, eax

		movzx	eax, source
		push	eax
		movzx	eax, block
		push	eax
		mov		esi, params
		push	buffer
		push	size_
		push	offset_
		call	FUNCTION
		add		esp, 4 * 5

		pop		esi
	API_FUNC_NAKED_END(6); // not including the unused tag_index
}