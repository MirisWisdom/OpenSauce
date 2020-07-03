/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

void GatherException(const void* data, int32 arg_0, int32 arg_4, int32 arg_8)
{
	static const uintptr_t TEMP_CALL_ADDR = GET_FUNC_PTR(GATHER_EXCEPTION);

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

bool SetTextureSamplerStage(Yelo::TagGroups::s_bitmap_data* bitmap, uint32 texture_stage)
{
	// use this instead of TextureCacheRequestTexture where applicable as this function
	// tests whether the texture is already loaded and sets the texture sampler itself

#if !PLATFORM_IS_DEDI
	static const uintptr_t TEMP_CALL_ADDR = GET_FUNC_PTR(RASTERIZER_SET_TEXTURE_BITMAP_DATA);

	_asm
	{
		mov		esi, bitmap
		push	texture_stage
		call	TEMP_CALL_ADDR
		add		esp, 4
	}
#else
	return false;
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
	return nullptr;
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


void RasterizerAddResolution(uint32 width, uint32 height, uint32 refresh_rate)
{
#if !PLATFORM_IS_DEDI
	static const uintptr_t TEMP_CALL_ADDR = GET_FUNC_PTR(RESOLUTION_LIST_ADD_RESOLUTION);

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

int16 API_FUNC_NAKED AnimationPickRandomPermutation(long_enum render_or_affects_game_state, datum_index animation_graph_index, int32 animation_index)
{
	static const uintptr_t TEMP_CALL_ADDR = GET_FUNC_PTR(ANIMATION_CHOOSE_RANDOM_PERMUTATION_INTERNAL);

	API_FUNC_NAKED_START()
		mov		edx, animation_index
		mov		eax, animation_graph_index
		push	render_or_affects_game_state
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1
	API_FUNC_NAKED_END(3)
}

void API_FUNC_NAKED GSMD5Digest(byte* input, unsigned int input_length, char output[33])
{
	static const uintptr_t FUNCTION = GET_FUNC_PTR(GSMD5DIGEST);

	API_FUNC_NAKED_START()
		push	output
		push	input_length
		push	input
		call	FUNCTION
	API_FUNC_NAKED_END_CDECL(3);
}

bool CompareMD5(byte* input, unsigned int input_length, const char comparison_md5[33])
{
	char input_md5[33];

	input_md5[0] = '\0';
	GSMD5Digest(input, input_length, input_md5);

	return _strnicmp(input_md5, comparison_md5, NUMBEROF(input_md5)) == 0;
}
