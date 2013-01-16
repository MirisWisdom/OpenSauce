/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

// DEBUG: Before each memory write when we're enabling, we assert we're overwriting the expected value or bytes

static void InitializeGrenadeCounts_UnitZoomLevelRefs(bool enabled)
{
	uint32 offset = enabled ? s_unit_data::k_offset_zoom_level_yelo : s_unit_data::k_offset_zoom_level;

	for(int x = 0; x < NUMBEROF(K_UNIT_ZOOM_LEVEL_OFFSET_REFS); x++)
	{
		uint32* offset_ref = CAST_PTR(uint32*, K_UNIT_ZOOM_LEVEL_OFFSET_REFS[x]);

		DebugRunOnce( ASSERT( s_unit_data::k_offset_zoom_level == *offset_ref, "GrenadeCounts asm mismatch!" ) );
		*offset_ref = offset;
	}
}
static void InitializeGrenadeCounts_UnitDesiredZoomLevelRefs(bool enabled)
{
	uint32 offset = enabled ? s_unit_data::k_offset_desired_zoom_level_yelo : s_unit_data::k_offset_desired_zoom_level;

	for(int x = 0; x < NUMBEROF(K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REFS); x++)
	{
		uint32* offset_ref = CAST_PTR(uint32*, K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REFS[x]);

		DebugRunOnce( ASSERT( s_unit_data::k_offset_desired_zoom_level == *offset_ref, "GrenadeCounts asm mismatch!" ) );
		*offset_ref = offset;
	}
}
static void InitializeGrenadeCounts_NumberOfUnitGrenadeTypes(uint32 count)
{
	for(int x = 0; x < NUMBEROF(K_NUMBER_OF_UNIT_GRENADE_TYPES_REFS); x++)
	{
		uint32* count_ref = CAST_PTR(uint32*, K_NUMBER_OF_UNIT_GRENADE_TYPES_REFS[x]);

		DebugRunOnce( ASSERT( Enums::k_unit_grenade_types_count == *count_ref, "GrenadeCounts asm mismatch!" ) );
		*count_ref = count;
	}

	//////////////////////////////////////////////////////////////////////////
	// patch the maximum unit grenade index
	count--;

	for(int x = 0; x < NUMBEROF(K_MAXIMUM_UNIT_GRENADE_INDEX_REFS_32bit); x++)
	{
		uint32* count_ref = CAST_PTR(uint32*, K_MAXIMUM_UNIT_GRENADE_INDEX_REFS_32bit[x]);

		DebugRunOnce( ASSERT( Enums::k_unit_grenade_types_count-1 == *count_ref, "GrenadeCounts asm mismatch!" ) );
		*count_ref = count;
	}
	for(int x = 0; x < NUMBEROF(K_MAXIMUM_UNIT_GRENADE_INDEX_REFS_8bit); x++)
	{
		byte* count_ref = CAST_PTR(byte*, K_MAXIMUM_UNIT_GRENADE_INDEX_REFS_8bit[x]);

		DebugRunOnce( ASSERT( Enums::k_unit_grenade_types_count-1 == *count_ref, "GrenadeCounts asm mismatch!" ) );
		*count_ref = CAST(byte, count);
	}
}

namespace unit_grenade_counts_mods
{
#if !PLATFORM_IS_EDITOR
	//////////////////////////////////////////////////////////////////////////
	// actor_died
	NAKED_FUNC_WRITER_ASM_BEGIN(actor_died__unit_grenade_count_word)
		mov	word ptr [ebx+31Eh], 0					NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(actor_died__unit_grenade_count_dword)
		xor	ecx, ecx
		mov	dword ptr [ebx+31Eh], ecx
		nop											NAKED_FUNC_WRITER_ASM_END();
#endif
	//////////////////////////////////////////////////////////////////////////
	// player_add_equipment
	NAKED_FUNC_WRITER_ASM_BEGIN(player_add_equipment__unit_grenade_count_word)
		mov	[PLATFORM_ENGINE_VALUE(ebp,esi)+31Eh], ax	NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(player_add_equipment__unit_grenade_count_dword)
		mov	[PLATFORM_ENGINE_VALUE(ebp,esi)+31Eh], eax
		nop												NAKED_FUNC_WRITER_ASM_END();
#if !PLATFORM_IS_EDITOR
	//////////////////////////////////////////////////////////////////////////
	// biped_new_from_network
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_new_from_network__unit_grenade_count_word)
		mov	dx, [eax+52Ch]
		mov	[eax+31Eh], dx							NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_new_from_network__unit_grenade_count_dword)
		mov	edx, [eax+52Ch]
		nop
		mov	[eax+31Eh], edx
		nop											NAKED_FUNC_WRITER_ASM_END();
	//////////////////////////////////////////////////////////////////////////
	// biped_update_baseline
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_update_baseline__unit_grenade_count_word1)
		mov	cx, [eax+31Eh]							NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_update_baseline__unit_grenade_count_word2)
		mov	[eax+52Ch], cx						NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_update_baseline__unit_grenade_count_dword1)
		mov	ecx, [eax+31Eh]
		nop											NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_update_baseline__unit_grenade_count_dword2)
		mov	[eax+52Ch], ecx
		nop											NAKED_FUNC_WRITER_ASM_END();
	//////////////////////////////////////////////////////////////////////////
	// biped_build_update_delta
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_build_update_delta__unit_grenade_count_word1)
		mov	dx, [edi+31Eh]							NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_build_update_delta__unit_grenade_count_word2)		// [esp+44h+var_10]
		mov	[esp+44h+(-10h)], dx					NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_build_update_delta__unit_grenade_count_dword1)
		mov	edx, [edi+31Eh]
		nop											NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_build_update_delta__unit_grenade_count_dword2)	// [esp+44h+var_10]
		mov	[esp+44h+(-10h)], edx
		nop											NAKED_FUNC_WRITER_ASM_END();
	//////////////////////////////////////////////////////////////////////////
	// biped_process_update_delta
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_process_update_delta__unit_grenade_count_word)
		mov	word ptr [esi+31Eh], ax					NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(biped_process_update_delta__unit_grenade_count_dword)
		mov	dword ptr [esi+31Eh], eax
		nop											NAKED_FUNC_WRITER_ASM_END();
#endif
};
