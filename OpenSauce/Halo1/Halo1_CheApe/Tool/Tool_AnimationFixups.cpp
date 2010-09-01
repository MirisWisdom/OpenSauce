/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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


		//////////////////////////////////////////////////////////////////////////
		// valid animations array

		// release builds were having issues with our inline assembly calling memset...
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
					if(max_animations > 500)
						EngineFunctions::error(Enums::_error_message_priority_warning, 
						"CheApe: tool has been known to stop processing after the 500th animation (user defined: %d)", max_animations);

					InitializeValidAnimationsArrayFixups();
					InitializeAnimationIndexArrayFixups();

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
			}
		}
	};
};
#endif