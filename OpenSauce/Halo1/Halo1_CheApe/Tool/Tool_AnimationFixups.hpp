/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once
#if PLATFORM_TYPE == PLATFORM_TOOL

namespace Yelo
{
	namespace Tool
	{
		class c_animation_fixups
		{
			static int32 max_animations;
			static bool* importer_valid_animations_array;
			static int32* importer_animation_index_array;
			static int16* importer_animation_remapping_index_array;

			static void InitializeValidAnimationsArrayFixups();
			static void DisposeValidAnimationsArrayFixups();

			static void InitializeAnimationIndexArrayFixups();
			static void DisposeAnimationIndexArrayFixups();

			static void InitializeAnimationRemappingIndexArrayFixups();
			static void DisposeAnimationRemappingIndexArrayFixups();

			static void InitializeIntermediateMemoryFixups();
		public:
			static void Initialize();
			static void Dispose();
		};
	};
};
#endif

// 0x402420, update with 0x21 * k_max_animations


// FUCKING LOCAL BOOLEAN ARRAY BULLSHIT FIXUPS!
/* overwrite with our own boolean array initializer
.text:0040448B                 push    100h
.text:00404490                 lea     edx, [esp+734h+var_710]
.text:00404494                 push    ebx ; 1
.text:00404495                 push    edx
.text:00404496                 call    csmemset
.text:0040449B                 mov     edx, dword_777720		// jmp to here
*/

/* 
.text:00404506                 mov     [esp+ebp+730h+var_710], 0
.text:0040450B loc_40450B:										// jmp to here
opcodes: C6 44  2C 20 00

use a relative jmp to execute the following:
mov		byte ptr our_boolean_array[ebp], 0		; this requires 8 bytes, so we have to do a relative jmp
jmp		0x40450B
*/

/*
.text:00404530                 mov     bl, [esp+eax+730h+var_710]
.text:00404534                 test    bl, bl
.text:00404536                 jz      short loc_40455B
opcodes: 8A 5C 04 20 84 DB 74 23

use a relative jmp to execute the following:
mov		bl, our_boolean_array[eax]				; this requires 6 bytes, so we have to do a relative jmp
test	bl, bl
jz		0x40455B
jmp		0x404538
*/

/*
.text:00404646                 mov     al, [esp+esi+73Ch+var_710]
.text:0040464A                 add     esp, 0Ch
.text:0040464D                 test    al, al
.text:0040464F                 jz      short loc_404666

use a relative jmp to execute the following:
add		esp, 0xC
mov		al, our_boolean_array[esi]
test	al, al
jz		0x404666
jmp		0x404651
*/


/*
.text:00404415                 mov     [esp+ebp*4+730h+Str], esi
.text:0040441C                 jmp     short loc_40447F


mov		our_index_array[ebp*4], esi
jmp		0x40447F
*/

/*
.text:00404452                 mov     [esp+ebp*4+730h+Str], 0FFFFFFFFh
.text:0040445D                 jle     short loc_40447F
.text:0040445F                 lea     eax, [esp+730h+Str]
.text:00404466                 movzx   edx, cx
.text:00404469                 lea     esp, [esp+0]
.text:00404470
.text:00404470 loc_404470:                             ; CODE XREF: sub_404340+13Dj


mov		our_index_array[ebp*4], 0xFFFFFFFF
jle		0x40447F
lea		eax, our_index_array
movzx	edx, cx
jmp		0x404470
*/