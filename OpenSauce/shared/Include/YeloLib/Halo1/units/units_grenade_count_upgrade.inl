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
		byte* count_ref = CAST_PTR(byte*, K_NUMBER_OF_UNIT_GRENADE_TYPES_REFS[x]);

		DebugRunOnce( ASSERT( Enums::k_unit_grenade_types_count == *count_ref, "GrenadeCounts asm mismatch!" ) );
		*count_ref = CAST(byte, count);
	}

	//////////////////////////////////////////////////////////////////////////
	// patch the maximum unit grenade index
	count--;

	for(int x = 0; x < NUMBEROF(K_MAXIMUM_UNIT_GRENADE_INDEX_REFS); x++)
	{
		byte* count_ref = CAST_PTR(byte*, K_MAXIMUM_UNIT_GRENADE_INDEX_REFS[x]);

		DebugRunOnce( ASSERT( Enums::k_unit_grenade_types_count-1 == *count_ref, "GrenadeCounts asm mismatch!" ) );
		*count_ref = CAST(byte, count);
	}
}

namespace unit_grenade_counts_mods
{
	// TODO: game_engine_map_reset resets all the player unit's grenade counts to zero
	// We may need to patch it up too

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
	// NOTE: currently don't apply this in the editor...shouldn't be an issue
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

/*
.text:00529526 _player_add_equipment                       mov     [esi+31Eh], ax              
.text:005295B6 _player_add_equipment                       lea     eax, [esi+31Eh]             
.text:0053E6CA game_engine_map_reset_player_units          mov     [eax+31Eh], bl              IGNORED
.text:005411D0 sub_541060                                  mov     [edx+31Eh], cl              IGNORED
.text:00587813 sub_5877B0                                  cmp     byte ptr [ecx+ebx+31Eh], 0  
.text:005889F8 sub_588970                                  movsx   cx, byte ptr [eax+edi+31Eh] NAR
.text:00588A0D sub_588970                                  mov     cl, [edx+edi+31Eh]          NAR
.text:00588A14 sub_588970                                  lea     eax, [edx+edi+31Eh]         NAR
.text:00588AF2 sub_588A80                                  lea     eax, [ecx+esi+31Eh]         
.text:00588E7C sub_588E30                                  movsx   ax, byte ptr [ecx+edi+31Eh] 
.text:00588FB0 sub_588EE0                                  mov     al, [edx+esi+31Eh]          
.text:00588FE2 sub_588EE0                                  mov     cl, [eax+esi+31Eh]          DUP
.text:00588FE9 sub_588EE0                                  lea     eax, [eax+esi+31Eh]         DUP
.text:00589C6E sub_589C50                                  lea     ebx, [eax+31Eh]             
.text:005906D9 sub_590660                                  lea     eax, [esi+31Eh]             
.text:005928C2                                             mov     [eax+esi+31Eh], cl          
.text:0059324E _unit_update                                lea     eax, [ebx+31Eh]             
.text:0061C06C sub_61BF60                                  movsx   edx, byte ptr [ecx+esi+31Eh] NAR
.text:00650810 biped_new_from_network                      lea     ecx, [esi+31Eh]             
.text:00650968 _biped_update_baseline                      lea     ecx, [eax+31Eh]             
.text:00650C6E _biped_build_update_delta                   lea     ecx, [esi+31Eh]             
.text:00651097 _biped_process_update_delta                 lea     eax, [esi+31Eh]             
.text:0065B07F sub_65AFA0                                  mov     al, [eax+ebp+31Eh]          NAR
.text:0065B165 sub_65AFA0                                  mov     al, [edx+ebp+31Eh]          NAR
.text:0065B692 sub_65B1F0                                  lea     ecx, [ebp+31Eh]             
.text:0065BE6A sub_65BB50                                  lea     ecx, [esi+31Eh]             
                   
.text:00697CC1 _actor_died                 add     ebx, 31Eh
.text:006CAC77 sub_6CAC60                  add     ecx, 31Eh
*/
