/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.TagGroups.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.TagGroups.inl"
#else
#include "Pointers/1.10/HaloCE_110_Dedi.TagGroups.inl"
#include "Pointers/1.10/HaloCE_110_Dedi_Manual.TagGroups.inl"
#endif

#include <blam/cache/cache_files_globals.hpp>
#include <blam/interface/map_list.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelo/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace DataFiles
	{
		FUNC_PTR(CACHE_FILES_INITIALIZE, DUO_PTR(K_CACHE_FILES_INITIALIZE));
		FUNC_PTR(CACHE_FILES_INITIALIZE_HOOK, DUO_PTR(K_CACHE_FILES_INITIALIZE_HOOK));
		FUNC_PTR(CACHE_FILES_INITIALIZE_RETN, DUO_PTR(K_CACHE_FILES_INITIALIZE_RETN));
		FUNC_PTR(CACHE_FILES_DISPOSE, DUO_PTR(K_CACHE_FILES_DISPOSE));
		FUNC_PTR(CACHE_FILES_DISPOSE_HOOK, DUO_PTR(K_CACHE_FILES_DISPOSE_HOOK));
		FUNC_PTR(CACHE_FILES_DISPOSE_RETN, DUO_PTR(K_CACHE_FILES_DISPOSE_RETN));
		FUNC_PTR(DATA_FILE_READ, DUO_PTR(K_DATA_FILE_READ));

		FUNC_PTR(SCENARIO_TAGS_LOAD_HOOK, DUO_PTR(K_SCENARIO_TAGS_LOAD_HOOK));
	}

	namespace Cache
	{
		static uint32* K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_LIST[] = {
			CAST_PTR(uint32*,K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_0),
			CAST_PTR(uint32*,K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_1),
			CAST_PTR(uint32*,K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_2),
			CAST_PTR(uint32*,K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_3),
			CAST_PTR(uint32*,K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_4),
			CAST_PTR(uint32*,K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_5),
			CAST_PTR(uint32*,K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_6),
			CAST_PTR(uint32*,K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_7),
			CAST_PTR(uint32*,K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_8),
			CAST_PTR(uint32*,K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_9),
		};

		static void* K_CACHE_PATH_SPRINTF_CALLS[] = {
			CAST_PTR(void*,K_CACHE_PATH_SPRINTF_CALL_0),
			#if PLATFORM_IS_USER
			CAST_PTR(void*,K_CACHE_PATH_SPRINTF_CALL_1),
			CAST_PTR(void*,K_CACHE_PATH_SPRINTF_CALL_2),
			#endif
		};
		static void* K_CACHE_PATH_SNPRINTF_CALLS[] = {
			#if PLATFORM_IS_DEDI
			CAST_PTR(void*, K_CACHE_PATH_SNPRINTF_CALL_0),
			#endif
			nullptr,
		};

		static void* K_CACHE_FILE_READ_HEADER_CALLS[] = {
			CAST_PTR(void*,K_CACHE_FILE_READ_HEADER_CALL_0),
			CAST_PTR(void*,K_CACHE_FILE_READ_HEADER_CALL_1),
			CAST_PTR(void*,K_CACHE_FILE_READ_HEADER_CALL_2),
		};

		#if !PLATFORM_IS_DEDI
		static cstring* K_UI_SCENARIO_NAME_REFERENCES[] = {
			CAST_PTR(cstring*,K_UI_SCENARIO_NAME_REFERENCE_0),
			CAST_PTR(cstring*,K_UI_SCENARIO_NAME_REFERENCE_1),
		};
		#endif

		ENGINE_PTR(s_cache_file_globals, cache_file_globals, DUO_PTR(K_CACHE_FILE_GLOBALS));
		ENGINE_PTR(char, maps_folder_parent_dir, DUO_PTR(K_MAPS_FOLDER_PARENT_DIR));
	}

	namespace Interface
	{
		ENGINE_PTR(map_list_data_t, multiplayer_maps, DUO_PTR(K_MULTIPLAYER_MAPS));
		FUNC_PTR(MULTIPLAYER_MAP_LIST_INITIALIZE, DUO_PTR(K_MULTIPLAYER_MAP_LIST_INITIALIZE));
		FUNC_PTR(MULTIPLAYER_MAP_LIST_DISPOSE, DUO_PTR(K_MULTIPLAYER_MAP_LIST_DISPOSE));
	}
}
