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


//////////////////////////////////////////////////////////////////////////
// Memory upgrade hacks
namespace BuildCacheFileEx { namespace MemoryUpgrades {

	static s_cache_header_yelo yelo_cache_header_globals;

	namespace AddressOf
	{
		// Addresses which check a counter against the max tag limit
		static uint32* MaxTagsCheck[] = {
			CAST_PTR(uint32*, 0x454049),
			CAST_PTR(uint32*, 0x4549B6),
		};

		// Fix up address for the value representing the highest 
		// tag memory address in cache builds
		static uint32* TagMaxAddress = CAST_PTR(uint32*, 0x454BCC); // build_cache_file_add_structure_bsps

		// Fix up address for the tag allocation size
		static int32* TagAllocationSize[] = {
			CAST_PTR(int32*, 0x4552DF), // build_cache_file_add_tags
			CAST_PTR(int32*, 0x455348), // build_cache_file_add_tags
		};

		// Fix up address for the tag allocation size (in negative 
		// signed form)
		static int32* TagAllocationSizeNegitive[] = {
			CAST_PTR(int32*, 0x4552F9), // build_cache_file_add_tags
		};

		// Fix up address to write the code to always jump to the 
		// campaign type case
		// * write 0xEB (jmp) here. Used to be 0x74 (jz)
		static byte* CacheSizeJmpAddress = CAST_PTR(byte*, 0x45317D); // build_cache_file_write_header_and_compress

		// Fix up address of the maximum cache size
		// * This address is actually for campaign types, but OS forces 
		// the campaign case
		static uint32* CacheSizeMax = CAST_PTR(uint32*, 0x4531B7); // build_cache_file_write_header_and_compress

		// Addresses of the value used for the tag memory's base address
		static uint32* TagMemoryBaseAddress[] = {
			CAST_PTR(uint32*, 0x454EBE),
			CAST_PTR(uint32*, 0x4551DB),
		};
		// Addresses of the value used for the structure_bsp tag memory's base address
		static uint32* TagMemoryBaseAddressStructureBsp[] = {
			CAST_PTR(uint32*, 0x454BCC),	// Same address as [TagMaxAddress]
		};
	};

	#pragma region Interceptor system
	//////////////////////////////////////////////////////////////////////////
	// Interceptor system
	static const uint32 BUILD_CACHE_FILE_ADD_TAGS = 0x454D40;
	static datum_index new_tag_handles[K_MAXIMUM_TAG_COUNT_UPGRADE];

	static const uint32 INTERCEPTOR1_HOOK_ADDR = 0x455583;
	API_FUNC_NAKED static void Interceptor1()
	{
		static const uint32 INTERCEPTOR_EXIT = 0x45559C;//9

		__asm {
			push	0

			lea		edx, new_tag_handles
			//lea		edx, [esp+0x14]

			push	edx
			lea		eax, [esp+0x5018]			// cache_header
			push	0
			push	eax
			call	BUILD_CACHE_FILE_ADD_TAGS
			add		esp, 0x10
			jmp		INTERCEPTOR_EXIT
		};
	}

	static const uint32 INTERCEPTOR2_HOOK_ADDR = 0x4555A0;
	API_FUNC_NAKED static void Interceptor2()
	{
		static const uint32 BUILD_CACHE_FILE_ADD_STRUCTURE_BSPS = 0x454B70;

		static const uint32 INTERCEPTOR_EXIT = 0x4555B4;//1

		__asm {
			lea		ecx, [esp+0xC]				// largest bsp size reference
			push	ecx

			lea		edx, new_tag_handles
			//lea		edx, [esp+0x14]

			push	edx
			mov		eax, ebx					// "scratch", tag memory
			call	BUILD_CACHE_FILE_ADD_STRUCTURE_BSPS
			add		esp, 0x8
			jmp		INTERCEPTOR_EXIT
		};
	}

	static const uint32 INTERCEPTOR3_HOOK_ADDR = 0x4555B8;
	API_FUNC_NAKED static void Interceptor3()
	{
		static const uint32 INTERCEPTOR_EXIT = 0x4555D3;//0

		__asm {
			lea		eax, [esp+0xC]				// largest bsp size reference
			push	eax

			lea		ecx, new_tag_handles
			//lea		ecx, [esp+0x14]

			push	ecx
			lea		edx, [esp+0x5018]			// cache_header
			push	ebx							// "scratch"
			push	edx
			call	BUILD_CACHE_FILE_ADD_TAGS
			add		esp, 0x10
			jmp		INTERCEPTOR_EXIT
		};
	}

	static const uint32 INTERCEPTOR_END_HOOK_ADDR = 0x453221; // build_cache_file_write_header_and_compress
	static bool PLATFORM_API InterceptorEnd(s_cache_header* header)
	{
		memcpy_s(&header->yelo, sizeof(header->yelo), &yelo_cache_header_globals, sizeof(yelo_cache_header_globals));

		return build_cache_file_for_scenario_internals.build_cache_file_end(header);
	}

	static const uint32 INTERCEPTOR_BUILD_CACHE_FILE_FAILED = 0x4555EE;
	static uint32 INTERCEPTOR_BUILD_CACHE_FILE_FAILED_restore_point = 0;
	static void PLATFORM_API InterceptorBuildCacheFileFailed()
	{
		build_cache_file_for_scenario_internals.build_failed = true;
	}

	static const uint32 INTERCEPTOR_BUILD_CACHE_FILE_BEGIN = 0x455531;
	static uint32 INTERCEPTOR_BUILD_CACHE_FILE_BEGIN_restore_point = 0;
	API_FUNC_NAKED static bool PLATFORM_API InterceptorBuildCacheFileBegin(cstring scenario_name)
	{
		__asm {
			push	ebp
			mov		ebp, esp
			push	scenario_name
			call	build_cache_file_begin_preprocess
			push	scenario_name
			call	build_cache_file_for_scenario_internals.build_cache_file_begin
			add		esp, 4 * 1
			pop		ebp
			retn	
		}
	}

	static void InterceptorsInitialize(bool only_using_data_file_hacks)
	{
		if(!only_using_data_file_hacks)
		{
			memset(CAST_PTR(void*, INTERCEPTOR1_HOOK_ADDR), 0x90, 0x16);
			Memory::WriteRelativeJmp(&Interceptor1, CAST_PTR(void*, INTERCEPTOR1_HOOK_ADDR), true);

			memset(CAST_PTR(void*, INTERCEPTOR2_HOOK_ADDR), 0x90, 0x11);
			Memory::WriteRelativeJmp(&Interceptor2, CAST_PTR(void*, INTERCEPTOR2_HOOK_ADDR), true);

			memset(CAST_PTR(void*, INTERCEPTOR3_HOOK_ADDR), 0x90, 0x18);
			Memory::WriteRelativeJmp(&Interceptor3, CAST_PTR(void*, INTERCEPTOR3_HOOK_ADDR), true);
		}

		Memory::WriteRelativeCall(&InterceptorEnd, CAST_PTR(void*, INTERCEPTOR_END_HOOK_ADDR));

		INTERCEPTOR_BUILD_CACHE_FILE_FAILED_restore_point = 
			Memory::WriteRelativeCall(&InterceptorBuildCacheFileFailed, CAST_PTR(void*, INTERCEPTOR_BUILD_CACHE_FILE_FAILED));

		INTERCEPTOR_BUILD_CACHE_FILE_BEGIN_restore_point = 
			Memory::WriteRelativeCall(&InterceptorBuildCacheFileBegin, CAST_PTR(void*, INTERCEPTOR_BUILD_CACHE_FILE_BEGIN));
	}

	static void InterceptorsDispose(bool only_using_data_file_hacks)
	{
		if(!only_using_data_file_hacks)
		{
			static const byte INTERCEPTOR1_HOOK_MEMORY_UNDO[0x16] = {
				0x6A, 0x00,									// push	0
				0x8D, 0x54, 0x24, 0x14,						// lea	edx, tag_handles
				0x52,										// push	edx
				0x8D, 0x84, 0x24, 0x18, 0x50, 0x00, 0x00,	// lea	eax, cache_header
				0x6A, 0x00,									// push	0
				0x50,										// push	eax
				0xE8, 0xA7, 0xF7, 0xFF, 0xFF,				// call build_cache_file_add_tags
			}; Memory::WriteMemory(
				CAST_PTR(void*, INTERCEPTOR1_HOOK_ADDR), 
				INTERCEPTOR1_HOOK_MEMORY_UNDO, 
				NUMBEROF(INTERCEPTOR1_HOOK_MEMORY_UNDO));

			static const byte INTERCEPTOR2_HOOK_MEMORY_UNDO[0x11] = {
				0x8D, 0x4C, 0x24, 0x0C,						// lea	ecx, largest bsp size reference
				0x51,										// push	ecx
				0x8D, 0x54, 0x24, 0x14,						// lea	edx, tag_handles
				0x52,										// push edx
				0x8B, 0xC3,									// mov	eax, ebx
				0xE8, 0xBF, 0xF5, 0xFF, 0xFF,				// call	build_cache_file_add_structure_bsps
			}; Memory::WriteMemory(
				CAST_PTR(void*, INTERCEPTOR2_HOOK_ADDR), 
				INTERCEPTOR2_HOOK_MEMORY_UNDO, 
				NUMBEROF(INTERCEPTOR2_HOOK_MEMORY_UNDO));

			static const byte INTERCEPTOR3_HOOK_MEMORY_UNDO[0x18] = {
				0x8B, 0x44, 0x24, 0x0C,						// mov	eax, largest bsp size reference
				0x50,										// push	eax
				0x8D, 0x4C, 0x24, 0x14,						// lea	ecx, tag_handles
				0x51,										// push	ecx
				0x8D, 0x94, 0x24, 0x18, 0x50, 0x00, 0x00,	// lea	edx, cache_header
				0x53,										// push	ebx
				0x52,										// push	edx
				0xE8, 0x70, 0xF7, 0xFF, 0xFF,				// call build_cache_file_add_tags
			}; Memory::WriteMemory(
				CAST_PTR(void*, INTERCEPTOR3_HOOK_ADDR), 
				INTERCEPTOR3_HOOK_MEMORY_UNDO, 
				NUMBEROF(INTERCEPTOR3_HOOK_MEMORY_UNDO));
		}

		static const byte INTERCEPTOR_END_HOOK_MEMORY_UNDO[4] = {
			0x8A, 0x61, 0x06, 0x00,
		}; Memory::WriteMemory(
			CAST_PTR(void*, INTERCEPTOR_END_HOOK_ADDR+1), // we're just restoring the relative offset value, not to include the opcode
			INTERCEPTOR_END_HOOK_MEMORY_UNDO, 
			NUMBEROF(INTERCEPTOR_END_HOOK_MEMORY_UNDO));

		Memory::WriteRelativeCall(CAST_PTR(void*, INTERCEPTOR_BUILD_CACHE_FILE_FAILED_restore_point),
			CAST_PTR(void*, INTERCEPTOR_BUILD_CACHE_FILE_FAILED));

		Memory::WriteRelativeCall(CAST_PTR(void*, INTERCEPTOR_BUILD_CACHE_FILE_BEGIN_restore_point),
			CAST_PTR(void*, INTERCEPTOR_BUILD_CACHE_FILE_BEGIN));
	}
	//////////////////////////////////////////////////////////////////////////
	#pragma endregion

	#pragma region Tag Memory Upgrades
	static void TagMemoryInitialize()
	{
		for(int32 x = 0; x < NUMBEROF(AddressOf::MaxTagsCheck); x++)
			*AddressOf::MaxTagsCheck[x] = Konstants::MaximumTagCountUpgrade();

		*AddressOf::TagMaxAddress = Konstants::TagMaxAddressUpgrade();

		for(int32 x = 0; x < NUMBEROF(AddressOf::TagAllocationSize); x++)
			*AddressOf::TagAllocationSize[x] = Konstants::TagAllocationSizeUpgrade();

		for(int32 x = 0; x < NUMBEROF(AddressOf::TagAllocationSizeNegitive); x++)
			*AddressOf::TagAllocationSizeNegitive[x] = -Konstants::TagAllocationSizeUpgrade();
	}

	static void TagMemoryDispose()
	{
		for(int32 x = 0; x < NUMBEROF(AddressOf::MaxTagsCheck); x++)
			*AddressOf::MaxTagsCheck[x] = Enums::k_maximum_tag_count;

		*AddressOf::TagMaxAddress = Enums::k_tag_max_address;

		for(int32 x = 0; x < NUMBEROF(AddressOf::TagAllocationSize); x++)
			*AddressOf::TagAllocationSize[x] = Enums::k_tag_allocation_size;

		for(int32 x = 0; x < NUMBEROF(AddressOf::TagAllocationSizeNegitive); x++)
			*AddressOf::TagAllocationSizeNegitive[x] = -Enums::k_tag_allocation_size;
	}
	#pragma endregion

	#pragma region Cache Memory Upgrades
	static void CacheMemoryInitialize()
	{
		*AddressOf::CacheSizeJmpAddress = 0xEB;

		*AddressOf::CacheSizeMax = Yelo::Enums::k_max_cache_size_upgrade;
	}

	static void CacheMemoryDispose()
	{
		*AddressOf::CacheSizeJmpAddress = 0x74;

		*AddressOf::CacheSizeMax = Yelo::Enums::k_max_cache_size;
	}
	#pragma endregion

	static void Initialize(bool only_using_data_file_hacks)
	{
		InterceptorsInitialize(only_using_data_file_hacks);

		if(!only_using_data_file_hacks)
		{
			TagMemoryInitialize();
			CacheMemoryInitialize();
		}

		TagGroups::ScenarioYeloLoadHookInitialize();
	}

	static void Dispose(bool only_using_data_file_hacks)
	{
		InterceptorsDispose(only_using_data_file_hacks);

		if(!only_using_data_file_hacks)
		{
			TagMemoryDispose();
			CacheMemoryDispose();
		}

		TagGroups::ScenarioYeloLoadHookDispose();
	}
}; };
//////////////////////////////////////////////////////////////////////////