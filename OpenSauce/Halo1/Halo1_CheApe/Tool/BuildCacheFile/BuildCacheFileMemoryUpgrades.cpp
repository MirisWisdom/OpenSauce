/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/BuildCacheFile/BuildCacheFileMemoryUpgrades.hpp"
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/cache/cache_files_structures.hpp>
#include <YeloLib/Halo1/cache/cache_files_structures_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

#include "Engine/Scripting.hpp"
#include "TagGroups/yelo_scenario_definitions.hpp"
#include "Tool/BuildCacheFile.hpp"
#include "Tool/BuildCacheFile/BuildCacheFileGlobals.hpp"

namespace Yelo { namespace Tool { namespace BuildCacheFileEx { namespace MemoryUpgrades {

	Cache::s_cache_header_yelo yelo_cache_header_globals;

	void InitializeHeaderGlobals(bool using_mod_sets, cstring mod_name, bool use_memory_upgrades)
	{
		Cache::s_cache_header_yelo& header = yelo_cache_header_globals;

		if (header.flags.uses_mod_data_files = using_mod_sets)
		{
			YELO_ASSERT(mod_name);
			strcpy_s(header.mod_name, mod_name);
		}

		if (header.flags.uses_memory_upgrades = use_memory_upgrades)
			header.k_memory_upgrade_increase_amount = K_MEMORY_UPGRADE_INCREASE_AMOUNT;
	}

	// Initializes the yelo header with the default build info settings
	void InitializeHeaderGlobalsBuildInfo()
	{
		Cache::s_cache_header_yelo& header = yelo_cache_header_globals;

		byte nil_uuid[Enums::k_uuid_buffer_size] = {};
		header.InitializeBuildInfo(Enums::_production_build_stage_ship, 0, nil_uuid);
	}

	// Initializes the yelo header with a scenario's yelo build info
	void InitializeHeaderGlobalsBuildInfo(const TagGroups::s_project_yellow_scenario_build_info& build_info)
	{
		Cache::s_cache_header_yelo& header = yelo_cache_header_globals;

		header.InitializeBuildInfo(build_info.build_stage, build_info.revision, build_info.uuid_buffer);
	}

	void InitializeHeaderGlobalsMinimumBuild(const byte major, const byte minor, const uint16 build)
	{
		Cache::s_cache_header_yelo& header = yelo_cache_header_globals;

		header.InitializeMinimumBuildInfo(major, minor, build);
	}

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

		// Fix up the scratch buffer tool uses when streaming the cache's tag header & instances & data
		static uint32* BuildCacheFileScratchBufferSize = CAST_PTR(uint32*, 0x4553D0);

		// Fix up the scratch buffer tool uses when streaming out the vertex and index data
		// * This buffer is actually never free'd in tool...and add_tags is called twice. Memory leak!
		static uint32* VertexAndIndexBufferSizeMax = CAST_PTR(uint32*, 0x454D57); // build_cache_file_add_tags

		// Addresses of the value used for the tag memory's base address
		static uint32* TagMemoryBaseAddress[] = {
			CAST_PTR(uint32*, 0x454EBE),
			CAST_PTR(uint32*, 0x4551DB),
		};
	};

	#pragma region Interceptor system
	//////////////////////////////////////////////////////////////////////////
	// Interceptor system
	static const uintptr_t BUILD_CACHE_FILE_ADD_TAGS = 0x454D40;
	static char* cache_tag_names_array_upgraded[Enums::k_maximum_simultaneous_tag_instances_upgrade];

	static const uintptr_t INTERCEPTOR1_HOOK_ADDR = 0x455583;
	API_FUNC_NAKED static void Interceptor1()
	{
		static const uintptr_t INTERCEPTOR_EXIT = 0x45559C;//9

		__asm {
			push	0

			lea		edx, cache_tag_names_array_upgraded
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

	static const uintptr_t INTERCEPTOR2_HOOK_ADDR = 0x4555A0;
	API_FUNC_NAKED static void Interceptor2()
	{
		static const uintptr_t BUILD_CACHE_FILE_ADD_STRUCTURE_BSPS = 0x454B70;

		static const uintptr_t INTERCEPTOR_EXIT = 0x4555B4;//1

		__asm {
			lea		ecx, [esp+0xC]				// largest bsp size reference
			push	ecx

			lea		edx, cache_tag_names_array_upgraded
			//lea		edx, [esp+0x14]

			push	edx
			mov		eax, ebx					// "scratch", tag memory
			call	BUILD_CACHE_FILE_ADD_STRUCTURE_BSPS
			add		esp, 0x8
			jmp		INTERCEPTOR_EXIT
		};
	}

	static const uintptr_t INTERCEPTOR3_HOOK_ADDR = 0x4555B8;
	API_FUNC_NAKED static void Interceptor3()
	{
		static const uintptr_t INTERCEPTOR_EXIT = 0x4555D3;//0

		__asm {
			lea		eax, [esp+0xC]				// largest bsp size reference
			push	eax

			lea		ecx, cache_tag_names_array_upgraded
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

	static const uintptr_t INTERCEPTOR_END_HOOK_ADDR = 0x453221; // build_cache_file_write_header_and_compress
	static bool PLATFORM_API InterceptorEnd(Cache::s_cache_header* header)
	{
		build_cache_file_end_preprocess(header, yelo_cache_header_globals);

		memcpy_s(&header->yelo, sizeof(header->yelo), &yelo_cache_header_globals, sizeof(yelo_cache_header_globals));

		return build_cache_file_for_scenario_internals.build_cache_file_end(header);
	}

	static const uintptr_t INTERCEPTOR_BUILD_CACHE_FILE_FAILED = 0x4555EE;
	static uint32 INTERCEPTOR_BUILD_CACHE_FILE_FAILED_restore_point = 0;
	static void PLATFORM_API InterceptorBuildCacheFileFailed()
	{
		build_cache_file_for_scenario_internals.build_failed = true;
	}

	static const uintptr_t INTERCEPTOR_BUILD_CACHE_FILE_BEGIN = 0x455531;
	static uint32 INTERCEPTOR_BUILD_CACHE_FILE_BEGIN_restore_point = 0;
	API_FUNC_NAKED static bool PLATFORM_API InterceptorBuildCacheFileBegin(cstring scenario_name)
	{
		API_FUNC_NAKED_START()

			push	scenario_name
			call	build_cache_file_begin_preprocess
			push	scenario_name
			call	build_cache_file_for_scenario_internals.build_cache_file_begin
			add		esp, 4 * 1

		API_FUNC_NAKED_END(0)
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
		for(auto ptr : AddressOf::MaxTagsCheck)
			*ptr = Enums::k_maximum_simultaneous_tag_instances_upgrade;

		*AddressOf::TagMaxAddress = Enums::k_tag_max_address_upgrade;

		for(auto ptr : AddressOf::TagAllocationSize)
			*ptr = Enums::k_tag_allocation_size_upgrade;

		for(auto ptr : AddressOf::TagAllocationSizeNegitive)
			*ptr = -Enums::k_tag_allocation_size_upgrade;
	}

	static void TagMemoryDispose()
	{
		for(auto ptr : AddressOf::MaxTagsCheck)
			*ptr = Enums::k_maximum_simultaneous_tag_instances;

		*AddressOf::TagMaxAddress = Enums::k_tag_max_address;

		for(auto ptr : AddressOf::TagAllocationSize)
			*ptr = Enums::k_tag_allocation_size;

		for(auto ptr : AddressOf::TagAllocationSizeNegitive)
			*ptr = -Enums::k_tag_allocation_size;
	}
	#pragma endregion

	#pragma region Cache Memory Upgrades
	static void CacheMemoryInitialize()
	{
		const size_t k_scratch_buffer_size_upgrade = 40 * Enums::k_mega;
		static_assert(k_scratch_buffer_size_upgrade >= Enums::k_tag_allocation_size_upgrade,
			"Scratch buffer upgrade is still too small to properly fit the tag cache data");

		*AddressOf::CacheSizeJmpAddress = Enums::_x86_opcode_jmp_short;

		*AddressOf::CacheSizeMax = Yelo::Enums::k_max_cache_size_upgrade;

		*AddressOf::VertexAndIndexBufferSizeMax = Yelo::Enums::k_max_cache_vertex_y_index_buffer_size_upgrade;

		*AddressOf::BuildCacheFileScratchBufferSize = k_scratch_buffer_size_upgrade;
	}

	static void CacheMemoryDispose()
	{
		const size_t k_scratch_buffer_size = 32 * Enums::k_mega;

		*AddressOf::CacheSizeJmpAddress = Enums::_x86_opcode_jz_short;

		*AddressOf::CacheSizeMax = Yelo::Enums::k_max_cache_size;

		*AddressOf::VertexAndIndexBufferSizeMax = Yelo::Enums::k_max_cache_vertex_y_index_buffer_size;

		*AddressOf::BuildCacheFileScratchBufferSize = k_scratch_buffer_size;
	}
	#pragma endregion

	void PrintScriptUpgradesUsage()
	{
		printf_s(	"> Script compiling will fail on scripts with OS-functions and globals\n"
					"> Use build-cache-file-ex with memory upgrades enabled if you use any\n\n");
	}

	void Initialize(bool only_using_data_file_hacks)
	{
		InterceptorsInitialize(only_using_data_file_hacks);

		// Only allow script node upgrades when building with use-memory-upgrades on
		if(only_using_data_file_hacks)
		{
			printf_s("CheApe: warning: memory upgrades are off, turning off OS script definitions support...\n");
			PrintScriptUpgradesUsage();

			Scripting::DisposeScriptNodeUpgrades();
		}
		else
		{
			TagMemoryInitialize();
			CacheMemoryInitialize();
			Scripting::InitializeCustomScriptingDefinitions();
		}

		TagGroups::ScenarioYeloLoadHookInitialize();
	}

	void Dispose(bool only_using_data_file_hacks)
	{
		InterceptorsDispose(only_using_data_file_hacks);

		if(only_using_data_file_hacks)
			Scripting::InitializeScriptNodeUpgrades();
		else
		{
			TagMemoryDispose();
			CacheMemoryDispose();
		}

		TagGroups::ScenarioYeloLoadHookDispose();
	}

}; }; }; };
#endif