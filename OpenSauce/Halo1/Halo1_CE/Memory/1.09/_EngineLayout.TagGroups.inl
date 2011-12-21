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
		static cstring* K_DATA_FILE_BITMAPS_NAME_REFERENCES[] = {
			CAST_PTR(cstring*, PLATFORM_VALUE(0x443018, 0x443B38)), CAST_PTR(cstring*, PLATFORM_VALUE(0x443037, 0x443B57)), 
		};
		static cstring* K_DATA_FILE_SOUNDS_NAME_REFERENCES[] = {
			CAST_PTR(cstring*, PLATFORM_VALUE(0x44311C, 0x443C3C)), CAST_PTR(cstring*, PLATFORM_VALUE(0x443130, 0x443C50)), 
		};
		static cstring* K_DATA_FILE_LOCALE_NAME_REFERENCES[] = {
			CAST_PTR(cstring*, PLATFORM_VALUE(0x443200, 0x443D20)), CAST_PTR(cstring*, PLATFORM_VALUE(0x443222, 0x443D42)), 
		};

	#if PLATFORM_IS_DEDI
		static char* K_DATA_FILES_STUPID_STRINGS_GET_NULLED[] = {
			CAST_PTR(char*, 0x559DF0), CAST_PTR(char*, 0x559E20), 
		};
	#endif

		FUNC_PTR(DATA_FILES_OPEN,			0x443000, 0x443B20);
		FUNC_PTR(DATA_FILES_CLOSE,			0x443320, 0x443E40);

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

#if !PLATFORM_IS_DEDI
		static cstring* K_UI_SCENARIO_NAME_REFERENCES[] = {
			CAST_PTR(cstring*, 0x4CBDDF), CAST_PTR(cstring*, 0x4CBE04),
		};
#endif

		ENGINE_PTR(s_cache_file_globals, cache_file_globals,0x643060, 0x5B4E50);
		ENGINE_PTR(char, maps_folder_parent_dir,			0x68C6B8, 0x5F4EE8);

		ENGINE_PTR(t_multiplayer_map_data, multiplayer_maps,0x6ADE34, 0x616664);
		ENGINE_PTR(cstring, map_list_map_extension,		0x609494, 0x563C30); // ".map"
		ENGINE_PTR(cstring, map_list_map_extension_ref,	0x49813D, 0x48DB2D);
		ENGINE_PTR(cstring, map_list_ignored_map_names,	0x629B3C, 0x59F4DC);
		FUNC_PTR(MULTIPLAYER_MAP_LIST_INITIALIZE_CALL,	0x4CABE2, 0x4B3F4A);
		FUNC_PTR(MULTIPLAYER_MAP_LIST_INITIALIZE,		0x4982B0, 0x48DCA0);
		FUNC_PTR(MULTIPLAYER_MAP_LIST_DISPOSE,			0x498330, 0x48DD20);
	};

//////////////////////////////////////////////////////////////////////////
// TagGroups.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_TAGGROUPS_TAG_GROUPS
	ENGINE_DPTR(s_cache_tag_instance, tag_instances,0x816DE4, 0x6E2CA4);

	FUNC_PTR(TAG_ITERATOR_NEXT,	0x442A20, 0x4435B0);


#else
	#error Undefined engine layout include for: __EL_INCLUDE_TAGGROUPS
#endif