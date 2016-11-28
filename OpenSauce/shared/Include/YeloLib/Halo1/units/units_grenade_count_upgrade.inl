/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

// DEBUG: Before each memory write when we're enabling, we assert we're overwriting the expected value or bytes

static void InitializeGrenadeCounts_UnitZoomLevelRefs(bool enabled)
{
	static bool verified;

	uint32 offset = enabled 
		? s_unit_data::k_offset_zoom_level_yelo 
		: s_unit_data::k_offset_zoom_level;

	for(int x = 0; x < NUMBEROF(K_UNIT_ZOOM_LEVEL_OFFSET_REFS); x++)
	{
		uint32* offset_ref = CAST_PTR(uint32*, K_UNIT_ZOOM_LEVEL_OFFSET_REFS[x]);

		DebugOnly( 
			if(!verified)
				YELO_ASSERT_DISPLAY( s_unit_data::k_offset_zoom_level == *offset_ref, "GrenadeCounts asm mismatch!" ) 
		);
		*offset_ref = offset;
	}
	DebugOnly( verified = true );
}
static void InitializeGrenadeCounts_UnitDesiredZoomLevelRefs(bool enabled)
{
	static bool verified;

	uint32 offset = enabled 
		? s_unit_data::k_offset_desired_zoom_level_yelo 
		: s_unit_data::k_offset_desired_zoom_level;

	for(int x = 0; x < NUMBEROF(K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REFS); x++)
	{
		uint32* offset_ref = CAST_PTR(uint32*, K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REFS[x]);

		DebugOnly( 
			if(!verified)
				YELO_ASSERT_DISPLAY( s_unit_data::k_offset_desired_zoom_level == *offset_ref, "GrenadeCounts asm mismatch!" ) 
		);
		*offset_ref = offset;
	}
	DebugOnly( verified = true );
}
static void InitializeGrenadeCounts_NumberOfUnitGrenadeTypes(uint32 count)
{
	static bool verified;

	for(int x = 0; x < NUMBEROF(K_NUMBER_OF_UNIT_GRENADE_TYPES_REFS); x++)
	{
		byte* count_ref = CAST_PTR(byte*, K_NUMBER_OF_UNIT_GRENADE_TYPES_REFS[x]);

		DebugOnly( 
			if(!verified)
				YELO_ASSERT_DISPLAY( Enums::k_unit_grenade_types_count == *count_ref, "GrenadeCounts asm mismatch!" ) 
		);
		*count_ref = CAST(byte, count);
	}

	//////////////////////////////////////////////////////////////////////////
	// patch the maximum unit grenade index
	count--;

	for(int x = 0; x < NUMBEROF(K_MAXIMUM_UNIT_GRENADE_INDEX_REFS); x++)
	{
		byte* count_ref = CAST_PTR(byte*, K_MAXIMUM_UNIT_GRENADE_INDEX_REFS[x]);

		DebugOnly( 
			if(!verified)
				YELO_ASSERT_DISPLAY( Enums::k_unit_grenade_types_count-1 == *count_ref, "GrenadeCounts asm mismatch!" ) 
		);
		*count_ref = CAST(byte, count);
	}

	DebugOnly( verified = true );
}

namespace unit_grenade_counts_mods
{
	enum {
		k_offset_grenade_counts = FIELD_OFFSET(s_unit_datum, unit.grenade_counts),
		k_offset_biped_update_grenade_counts = FIELD_OFFSET(s_biped_datum, biped.update_baseline.grenade_counts),
	};

	// TODO: game_engine_map_reset resets all the player unit's grenade counts to zero
	// We may need to patch it up too

#if !PLATFORM_IS_EDITOR
	//////////////////////////////////////////////////////////////////////////
	// actor_died
	NAKED_FUNC_WRITER_ASM_BEGIN(actor_died__unit_grenade_count_word)
		mov	word ptr [ebx+k_offset_grenade_counts], 0	NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(actor_died__unit_grenade_count_dword)
		xor	ecx, ecx
		mov	dword ptr [ebx+k_offset_grenade_counts], ecx
		nop												NAKED_FUNC_WRITER_ASM_END();
#endif
	//////////////////////////////////////////////////////////////////////////
	// player_add_equipment
	// NOTE: currently don't apply this in the editor...shouldn't be an issue
	NAKED_FUNC_WRITER_ASM_BEGIN(player_add_equipment__unit_grenade_count_word)
		mov	[PLATFORM_ENGINE_VALUE(ebp,esi)+k_offset_grenade_counts], ax
														NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(player_add_equipment__unit_grenade_count_dword)
		mov	[PLATFORM_ENGINE_VALUE(ebp,esi)+k_offset_grenade_counts], eax
		nop												NAKED_FUNC_WRITER_ASM_END();
#if !PLATFORM_IS_EDITOR
	//////////////////////////////////////////////////////////////////////////
	// biped_new_from_network
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_new_from_network__unit_grenade_count_word)
		mov	dx, [eax+k_offset_biped_update_grenade_counts]
		mov	[eax+k_offset_grenade_counts], dx			NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_new_from_network__unit_grenade_count_dword)
		mov	edx, [eax+k_offset_biped_update_grenade_counts]
		nop
		mov	[eax+k_offset_grenade_counts], edx
		nop												NAKED_FUNC_WRITER_ASM_END();
	//////////////////////////////////////////////////////////////////////////
	// biped_update_baseline
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_update_baseline__unit_grenade_count_word1)
		mov	cx, [eax+k_offset_grenade_counts]			NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_update_baseline__unit_grenade_count_word2)
		mov	[eax+k_offset_biped_update_grenade_counts], cx
														NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_update_baseline__unit_grenade_count_dword1)
		mov	ecx, [eax+k_offset_grenade_counts]
		nop												NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_update_baseline__unit_grenade_count_dword2)
		mov	[eax+k_offset_biped_update_grenade_counts], ecx
		nop												NAKED_FUNC_WRITER_ASM_END();
	//////////////////////////////////////////////////////////////////////////
	// biped_build_update_delta
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_build_update_delta__unit_grenade_count_word1)
		mov	dx, [edi+k_offset_grenade_counts]			NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_build_update_delta__unit_grenade_count_word2)		// [esp+44h+var_10]
		mov	[esp+44h+(-10h)], dx						NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_build_update_delta__unit_grenade_count_dword1)
		mov	edx, [edi+k_offset_grenade_counts]
		nop												NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_build_update_delta__unit_grenade_count_dword2)	// [esp+44h+var_10]
		mov	[esp+44h+(-10h)], edx
		nop												NAKED_FUNC_WRITER_ASM_END();
	//////////////////////////////////////////////////////////////////////////
	// biped_process_update_delta
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_process_update_delta__unit_grenade_count_word)
		mov	word ptr [esi+k_offset_grenade_counts], ax	NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_process_update_delta__unit_grenade_count_dword)
		mov	dword ptr [esi+k_offset_grenade_counts], eax
		nop												NAKED_FUNC_WRITER_ASM_END();
#endif
};
