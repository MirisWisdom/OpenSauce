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
			CAST_PTR(cstring*, PLATFORM_VALUE(0x443008, 0x443B28)), CAST_PTR(cstring*, PLATFORM_VALUE(0x443027, 0x443B47)), 
		};
		static cstring* K_DATA_FILE_SOUNDS_NAME_REFERENCES[] = {
			CAST_PTR(cstring*, PLATFORM_VALUE(0x44310C, 0x443C2C)), CAST_PTR(cstring*, PLATFORM_VALUE(0x443120, 0x443C40)), 
		};
		static cstring* K_DATA_FILE_LOCALE_NAME_REFERENCES[] = {
			CAST_PTR(cstring*, PLATFORM_VALUE(0x4431F0, 0x443D10)), CAST_PTR(cstring*, PLATFORM_VALUE(0x443212, 0x443D32)), 
		};

	#if PLATFORM_IS_DEDI
		static char* K_DATA_FILES_STUPID_STRINGS_GET_NULLED[] = {
			CAST_PTR(char*, 0x559E30), CAST_PTR(char*, 0x559E60), 
		};
	#endif

		FUNC_PTR(DATA_FILES_OPEN,			0x442FF0, 0x443B10);
		FUNC_PTR(DATA_FILES_CLOSE,			0x443310, 0x443E30);

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

#if !PLATFORM_IS_DEDI
		static cstring* K_UI_SCENARIO_NAME_REFERENCES[] = {
			CAST_PTR(cstring*, 0x4CBDAF), CAST_PTR(cstring*, 0x4CBDD4),
		};
#endif

		ENGINE_PTR(s_cache_file_globals, cache_file_globals,0x642F78, 0x5B4DB8);
		ENGINE_PTR(char, maps_folder_parent_dir,			0x68C5D0, 0x5F4E50);

		ENGINE_PTR(t_multiplayer_map_data, multiplayer_maps,0x6ADD4C, 0x61654C);
		ENGINE_PTR(cstring, map_list_map_extension,		0x6094B4, 0x563C70); // ".map"
		ENGINE_PTR(cstring, map_list_map_extension_ref,	0x49810D, 0x48DAFD);
		ENGINE_PTR(cstring, map_list_ignored_map_names,	0x629B3C, 0x59F4DC);
		FUNC_PTR(MULTIPLAYER_MAP_LIST_INITIALIZE_CALL,	0x4CABB2, 0x4B3F1A);
		FUNC_PTR(MULTIPLAYER_MAP_LIST_INITIALIZE,		0x498280, 0x48DC70);
		FUNC_PTR(MULTIPLAYER_MAP_LIST_DISPOSE,			0x498300, 0x48DCF0);
	};

//////////////////////////////////////////////////////////////////////////
// TagGroups.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_TAGGROUPS_TAG_GROUPS
	ENGINE_DPTR(s_cache_tag_instance, tag_instances,0x816D04, 0x6E2C04);

	FUNC_PTR(TAG_ITERATOR_NEXT,	0x442A10, 0x4435A0);


#else
	#error Undefined engine layout include for: __EL_INCLUDE_TAGGROUPS
#endif