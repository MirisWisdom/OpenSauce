/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Tool_AnimationFixups.hpp"

#if PLATFORM_ID == PLATFORM_TOOL
#include "Yelo/MemoryFixups.hpp"
#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace Tool
	{
		int32 c_animation_fixups::max_animations = NONE;
		bool* c_animation_fixups::importer_valid_animations_array = NULL;
		int32* c_animation_fixups::importer_animation_index_array = NULL;
		int16* c_animation_fixups::importer_animation_remapping_index_array = NULL;


		//////////////////////////////////////////////////////////////////////////
		// valid animations array

		// release builds were having issues with our inline assembly calling memset...
		// This is may be due (I never checked the output .asm) to the compiler putting args into registers (which our inline asm clearly doesn't do)
		static void API_FUNC memset_hack(void* addr, size_t size)
		{
			memset(addr, true, size);
		}

		API_FUNC_NAKED static void _importer_valid_animations_initializer()
		{
			static const uint32 RETN_ADDRESS = 0x40449B;

			__asm {
				push	c_animation_fixups::max_animations
				push	c_animation_fixups::importer_valid_animations_array
				call	memset_hack

				jmp		RETN_ADDRESS
			}
		}

		API_FUNC_NAKED static void _importer_valid_animations_falsify_entry()
		{
			static const uint32 RETN_ADDRESS = 0x40450B;

			__asm {
				push	ecx // we use ecx for our address register, so we have to save it
				mov		ecx, c_animation_fixups::importer_valid_animations_array
				mov		byte ptr [ecx+ebp], 0
				pop		ecx

				jmp		RETN_ADDRESS
			}
		}

		API_FUNC_NAKED static void _importer_valid_animations_test1()
		{
			static const uint32 JZ_ADDRESS = 0x40455B;
			static const uint32 RETN_ADDRESS = 0x404538;

			__asm {
				push	esi // we use esi for our address register, so we have to save it
				mov		esi, c_animation_fixups::importer_valid_animations_array
				mov		bl, byte ptr [esi+eax]
				pop		esi
				test	bl, bl
				jz		_result_is_zero

				jmp		RETN_ADDRESS
_result_is_zero:
				jmp		JZ_ADDRESS
			}
		}

		API_FUNC_NAKED static void _importer_valid_animations_test2()
		{
			static uint32 JZ_ADDRESS = 0x404666;
			static uint32 RETN_ADDRESS = 0x404651;

			__asm {
				add		esp, 0xC

				push	ecx // we use ecx for our address register, so we have to save it
				mov		ecx, c_animation_fixups::importer_valid_animations_array
				mov		al, byte ptr [ecx+esi]
				pop		ecx

				test	al, al
				jz		_result_is_zero

				jmp		RETN_ADDRESS
_result_is_zero:
				jmp		JZ_ADDRESS
			}
		}

		void c_animation_fixups::InitializeValidAnimationsArrayFixups()
		{
			importer_valid_animations_array = YELO_NEW_ARRAY(bool, max_animations);
			YELO_ASSERT( importer_valid_animations_array );
			memset(importer_valid_animations_array, true, max_animations);

			// The output code of this is causing buffer overruns somehow, so we 
			// just do our own memset call above this comment ^
// 			void* IMPORTER_VALID_ANIMATIONS_INITIALIZER_HOOK = CAST_PTR(void*, 0x40448B);
// 			Memory::WriteRelativeJmp(_importer_valid_animations_initializer, 
// 				IMPORTER_VALID_ANIMATIONS_INITIALIZER_HOOK, true);

			void* IMPORTER_VALID_ANIMATIONS_FALSIFY_ENTRY_HOOK = CAST_PTR(void*, 0x404506);
			Memory::WriteRelativeJmp(_importer_valid_animations_falsify_entry, 
				IMPORTER_VALID_ANIMATIONS_FALSIFY_ENTRY_HOOK, true);

			void* IMPORTER_VALID_ANIMATIONS_TEST1_HOOK = CAST_PTR(void*, 0x404530);
			Memory::WriteRelativeJmp(_importer_valid_animations_test1, 
				IMPORTER_VALID_ANIMATIONS_TEST1_HOOK, true);
			void* IMPORTER_VALID_ANIMATIONS_TEST2_HOOK = CAST_PTR(void*, 0x404646);
			Memory::WriteRelativeJmp(_importer_valid_animations_test2, 
				IMPORTER_VALID_ANIMATIONS_TEST2_HOOK, true);
		}

		void c_animation_fixups::DisposeValidAnimationsArrayFixups()
		{
			YELO_DELETE(importer_valid_animations_array);
		}
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// animation index array

		API_FUNC_NAKED static void _importer_animation_index_set_indice()
		{
			static const uint32 RETN_ADDRESS = 0x40447F;

			__asm {
				// eax isn't used around this code, so it is safe
				mov		eax, c_animation_fixups::importer_animation_index_array
				mov		[eax+ebp*4], esi

				jmp		RETN_ADDRESS
			}
		}

		API_FUNC_NAKED static void _importer_animation_index_set_indice2()
		{
			static uint32 JLE_ADDRESS = 0x40447F;
			static uint32 RETN_ADDRESS = 0x404470;

			__asm {
				// eax isn't used around this code, so it is safe
				mov		eax, c_animation_fixups::importer_animation_index_array

				mov		[eax+ebp*4], NONE
				jle		_result_is_le
				mov		eax, c_animation_fixups::importer_animation_index_array
				movzx	edx, cx

				jmp		RETN_ADDRESS
_result_is_le:
				jmp		JLE_ADDRESS
			}
		}

		void c_animation_fixups::InitializeAnimationIndexArrayFixups()
		{
			importer_animation_index_array = YELO_NEW_ARRAY(int32, max_animations);
			YELO_ASSERT( importer_animation_index_array );

			void* IMPORTER_ANIMATION_INDEX_SET_INDICE_HOOK = CAST_PTR(void*, 0x404415);
			Memory::WriteRelativeJmp(_importer_animation_index_set_indice, 
				IMPORTER_ANIMATION_INDEX_SET_INDICE_HOOK, true);

			void* IMPORTER_ANIMATION_INDEX_SET_INDICE2_HOOK = CAST_PTR(void*, 0x404452);
			Memory::WriteRelativeJmp(_importer_animation_index_set_indice2, 
				IMPORTER_ANIMATION_INDEX_SET_INDICE2_HOOK, true);
		}

		void c_animation_fixups::DisposeAnimationIndexArrayFixups()
		{
			YELO_DELETE(importer_animation_index_array);
		}
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// animation index array

		API_FUNC_NAKED static void _importer_animation_remapping_index_set_indice()
		{
			static const uint32 RETN_ADDRESS = 0x40332B;

			__asm {
				// eax isn't used around this code, so it is safe
				mov		eax, c_animation_fixups::importer_animation_remapping_index_array
				mov		[eax+ebp*2], si

				jmp		RETN_ADDRESS
			}
		}

		API_FUNC_NAKED static void _importer_animation_remapping_index_get_indice()
		{
			static const uint32 RETN_ADDRESS = 0x4033B4;

			__asm {
				// eax isn't used around this code, so it is safe
				mov		eax, c_animation_fixups::importer_animation_remapping_index_array
				mov		dx, [eax+ecx*2]

				jmp		RETN_ADDRESS
			}
		}

		API_FUNC_NAKED static void _importer_animation_remapping_index_get_indice2()
		{
			static const uint32 RETN_ADDRESS = 0x4034A2;

			__asm {
				// eax isn't used around this code, so it is safe
				mov		eax, c_animation_fixups::importer_animation_remapping_index_array
				mov		ax, [eax+edx*2]

				jmp		RETN_ADDRESS
			}
		}

		void c_animation_fixups::InitializeAnimationRemappingIndexArrayFixups()
		{
			importer_animation_remapping_index_array = YELO_NEW_ARRAY(int16, max_animations);
			YELO_ASSERT( importer_animation_remapping_index_array );
			memset(importer_animation_remapping_index_array, NONE, max_animations * sizeof(importer_animation_remapping_index_array[0]));

			void* IMPORTER_ANIMATION_REMAPPING_INDEX_SET_INDICE_HOOK = CAST_PTR(void*, 0x403326);
			Memory::WriteRelativeJmp(_importer_animation_remapping_index_set_indice, 
				IMPORTER_ANIMATION_REMAPPING_INDEX_SET_INDICE_HOOK, true);

			void* IMPORTER_ANIMATION_REMAPPING_INDEX_GET_INDICE_HOOK = CAST_PTR(void*, 0x4033AF);
			Memory::WriteRelativeJmp(_importer_animation_remapping_index_get_indice, 
				IMPORTER_ANIMATION_REMAPPING_INDEX_GET_INDICE_HOOK, true);

			void* IMPORTER_ANIMATION_REMAPPING_INDEX_GET_INDICE2_HOOK = CAST_PTR(void*, 0x40349D);
			Memory::WriteRelativeJmp(_importer_animation_remapping_index_get_indice2, 
				IMPORTER_ANIMATION_REMAPPING_INDEX_GET_INDICE2_HOOK, true);
		}

		void c_animation_fixups::DisposeAnimationRemappingIndexArrayFixups()
		{
			YELO_DELETE(importer_animation_remapping_index_array);
		}
		//////////////////////////////////////////////////////////////////////////


		void c_animation_fixups::InitializeIntermediateMemoryFixups()
		{
			int32* INTERMEDIATE_DATA_MEMORY_ALLOCATION_SIZE = CAST_PTR(int32*, 0x402420);

			*INTERMEDIATE_DATA_MEMORY_ALLOCATION_SIZE = max_animations * 0x21;
		}


		void c_animation_fixups::Initialize()
		{
			CheApe::s_cache_header& header = CheApe::GlobalCacheHeader();

			if(header.Fixups.Count > 0)
			{
				c_memory_fixups::s_fixup** fixups = CAST_PTR(c_memory_fixups::s_fixup**, header.Fixups.Address);
				for(int32 x = 0; x < header.Fixups.Count; x++)
					if(fixups[x]->address[PLATFORM_ID-1] == CAST_PTR(void*, 0x6BAAC8)) // address of animation_block.max_count
						max_animations = CAST_PTR(int32, fixups[x]->definition[PLATFORM_ID-1]);

				// if the fix-up list contained an entry for the animation_block's max count 
				// then use that value to fix-up tool, else no animation code fix-ups are performed
				if(max_animations != NONE)
				{
					if(max_animations > Enums::k_maximum_tool_import_files)
						EngineFunctions::error(Enums::_error_message_priority_warning, 
						"CheApe: maximum animations is greater than maximum number of import files tool is able to process (%d > %d)", max_animations, Enums::k_maximum_tool_import_files);

					InitializeValidAnimationsArrayFixups();
					InitializeAnimationIndexArrayFixups();
					InitializeAnimationRemappingIndexArrayFixups();

					InitializeIntermediateMemoryFixups();
				}
			}
		}

		void c_animation_fixups::Dispose()
		{
			if(max_animations != NONE)
			{
				DisposeValidAnimationsArrayFixups();
				DisposeAnimationIndexArrayFixups();
				DisposeAnimationRemappingIndexArrayFixups();
			}
		}
	};
};
#endif