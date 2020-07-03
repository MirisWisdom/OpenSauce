/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// CacheFiles.cpp
#if __EL_INCLUDE_FILE_ID == __EL_TAGGROUPS_CACHE_FILES
	namespace DataFiles
	{
		FUNC_PTR(CACHE_FILES_INITIALIZE,		0x442FF0, 0x443B10);
		FUNC_PTR(CACHE_FILES_INITIALIZE_HOOK,	0x442FFB, 0x443B1B);
		FUNC_PTR(CACHE_FILES_INITIALIZE_RETN,	0x4432E3, 0x443E03);
		FUNC_PTR(CACHE_FILES_DISPOSE,			0x443310, 0x443E30);
		FUNC_PTR(CACHE_FILES_DISPOSE_HOOK,		0x443358, 0x443E78);
		FUNC_PTR(CACHE_FILES_DISPOSE_RETN,		0x443572, 0x444092);
		FUNC_PTR(DATA_FILE_READ,			0x4445E0, 0x444ED0);

		FUNC_PTR(SCENARIO_TAGS_LOAD_HOOK,	0x44239D, 0x442F51);
	};

	namespace Cache
	{
		static uint32* K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_LIST[] = {
			CAST_PTR(uint32*, PLATFORM_VALUE(0x442354,0x442F08)), CAST_PTR(uint32*, PLATFORM_VALUE(0x442B65,0x4436A3)), CAST_PTR(uint32*, PLATFORM_VALUE(0x4436A2,0x4441AF)),
			CAST_PTR(uint32*, PLATFORM_VALUE(0x44416C,0x44491C)), CAST_PTR(uint32*, PLATFORM_VALUE(0x4442D4,0x444A84)), CAST_PTR(uint32*, PLATFORM_VALUE(0x444301,0x444AB1)),
			CAST_PTR(uint32*, PLATFORM_VALUE(0x444324,0x444AD4)), 

			CAST_PTR(uint32*, PLATFORM_VALUE(0x4442EA,0x444A9A)), CAST_PTR(uint32*, PLATFORM_VALUE(0x444319,0x444AC9)), CAST_PTR(uint32*, PLATFORM_VALUE(0x44433A,0x444AEA)),
		};

		static void* K_CACHE_PATH_SPRINTF_CALLS[] = {
			CAST_PTR(void*, PLATFORM_VALUE(0x44362F,0x444DB6)), 
	#if PLATFORM_IS_USER
			CAST_PTR(void*, 0x443EBB), 
			CAST_PTR(void*, 0x4CCC7E),
	#endif
		};
		static void* K_CACHE_PATH_SNPRINTF_CALLS[] = {
	#if PLATFORM_IS_DEDI
			CAST_PTR(void*, 0x444EA2), 
	#endif
			NULL,
		};

		static void* K_CACHE_FILE_READ_HEADER_CALLS[] = {
			CAST_PTR(void*, PLATFORM_VALUE(0x443E68, 0x444658)), CAST_PTR(void*, PLATFORM_VALUE(0x4981AD, 0x48DB9F)), CAST_PTR(void*, PLATFORM_VALUE(0x4A0973, 0x4937F5)), 
		};

#if !PLATFORM_IS_DEDI
		static cstring* K_UI_SCENARIO_NAME_REFERENCES[] = {
			CAST_PTR(cstring*, 0x4CBDAF), CAST_PTR(cstring*, 0x4CBDD4),
		};
#endif

		ENGINE_PTR(s_cache_file_globals, cache_file_globals,0x642F78, 0x5B4DB8);
		ENGINE_PTR(char, maps_folder_parent_dir,			0x68C5D0, 0x5F4E50);
	};
	namespace Interface
	{
		ENGINE_PTR(map_list_data_t, multiplayer_maps,	0x6ADD4C, 0x61654C);
		FUNC_PTR(MULTIPLAYER_MAP_LIST_INITIALIZE,		0x498280, 0x48DC70);
		FUNC_PTR(MULTIPLAYER_MAP_LIST_DISPOSE,			0x498300, 0x48DCF0);
	};

//////////////////////////////////////////////////////////////////////////
// TagGroups.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_TAGGROUPS_TAG_GROUPS
	ENGINE_DPTR(Cache::s_cache_tag_instance, tag_instances,	0x816D04, 0x6E2C04);


#else
	#error Undefined engine layout include for: __EL_INCLUDE_TAGGROUPS
#endif