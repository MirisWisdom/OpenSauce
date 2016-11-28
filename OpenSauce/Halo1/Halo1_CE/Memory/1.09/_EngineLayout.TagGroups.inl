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
		FUNC_PTR(CACHE_FILES_INITIALIZE,		0x443000, 0x443B20);
		FUNC_PTR(CACHE_FILES_INITIALIZE_HOOK,	0x44300B, 0x443B2B);
		FUNC_PTR(CACHE_FILES_INITIALIZE_RETN,	0x4432F3, 0x443E13);
		FUNC_PTR(CACHE_FILES_DISPOSE,			0x443320, 0x443E40);
		FUNC_PTR(CACHE_FILES_DISPOSE_HOOK,		0x443368, 0x443E88);
		FUNC_PTR(CACHE_FILES_DISPOSE_RETN,		0x443582, 0x4440A2);
		FUNC_PTR(DATA_FILE_READ,				0x4445E0+0x10, 0x444ED0+0x10);

		FUNC_PTR(SCENARIO_TAGS_LOAD_HOOK,	0x4423AD, 0x442F61);
	};

	namespace Cache
	{
		static uint32* K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_LIST[] = {
			CAST_PTR(uint32*, PLATFORM_VALUE(0x442364,0x442F18)), CAST_PTR(uint32*, PLATFORM_VALUE(0x442B75,0x4436B3)), CAST_PTR(uint32*, PLATFORM_VALUE(0x4436B2,0x4441BF)),
			CAST_PTR(uint32*, PLATFORM_VALUE(0x44417C,0x44492C)), CAST_PTR(uint32*, PLATFORM_VALUE(0x4442E4,0x444A94)), CAST_PTR(uint32*, PLATFORM_VALUE(0x444311,0x444AC1)),
			CAST_PTR(uint32*, PLATFORM_VALUE(0x444334,0x444AE4)), 

			CAST_PTR(uint32*, PLATFORM_VALUE(0x4442FA,0x444AAA)), CAST_PTR(uint32*, PLATFORM_VALUE(0x444329,0x444AD9)), CAST_PTR(uint32*, PLATFORM_VALUE(0x44434A,0x444AFA)),
		};

		static void* K_CACHE_PATH_SPRINTF_CALLS[] = {
			CAST_PTR(void*, PLATFORM_VALUE(0x44363F,0x444DC6)), 
	#if PLATFORM_IS_USER
			CAST_PTR(void*, 0x443ECB), 
			CAST_PTR(void*, 0x4CCCAE),
	#endif
		};
		static void* K_CACHE_PATH_SNPRINTF_CALLS[] = {
	#if PLATFORM_IS_DEDI
			CAST_PTR(void*, 0x444EB2), 
	#endif
			NULL,
		};

		static void* K_CACHE_FILE_READ_HEADER_CALLS[] = {
			CAST_PTR(void*, PLATFORM_VALUE(0x443E78, 0x444668)), CAST_PTR(void*, PLATFORM_VALUE(0x4981DD, 0x48DBCF)), CAST_PTR(void*, PLATFORM_VALUE(0x4A09A3, 0x493825)), 
		};

#if !PLATFORM_IS_DEDI
		static cstring* K_UI_SCENARIO_NAME_REFERENCES[] = {
			CAST_PTR(cstring*, 0x4CBDDF), CAST_PTR(cstring*, 0x4CBE04),
		};
#endif

		ENGINE_PTR(s_cache_file_globals, cache_file_globals,0x643060, 0x5B4E50);
		ENGINE_PTR(char, maps_folder_parent_dir,			0x68C6B8, 0x5F4EE8);
	};
	namespace Interface
	{
		ENGINE_PTR(map_list_data_t, multiplayer_maps,	0x6ADE34, 0x616664);
		FUNC_PTR(MULTIPLAYER_MAP_LIST_INITIALIZE,		0x4982B0, 0x48DCA0);
		FUNC_PTR(MULTIPLAYER_MAP_LIST_DISPOSE,			0x498330, 0x48DD20);
	};

//////////////////////////////////////////////////////////////////////////
// TagGroups.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_TAGGROUPS_TAG_GROUPS
	ENGINE_DPTR(Cache::s_cache_tag_instance, tag_instances,	0x816DE4, 0x6E2CA4);


#else
	#error Undefined engine layout include for: __EL_INCLUDE_TAGGROUPS
#endif