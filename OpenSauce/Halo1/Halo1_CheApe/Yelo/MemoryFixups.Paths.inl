/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

c_memory_fixups::s_override_paths c_memory_fixups::_override_paths;

DWORD WINAPI c_memory_fixups::GetCurrentDirectoryHack(
	__in DWORD nBufferLength,
	__out_ecount_part_opt(nBufferLength, return + 1) LPSTR lpBuffer)
{
	strcpy_s(lpBuffer, nBufferLength, _override_paths.root
		/*Settings::Get().active_profile.root_path*/);

	return S_OK;
}

#if PLATFORM_TYPE != PLATFORM_GUERILLA
// The following is a hack which fixes a problem with tag_file_index_build.
//
// Basically, the code for that function assumes that all tag names begin 
// with "tags\" (ie, it assumes the tags path is in the same startup directory as the exe), 
// which is why it calls strchr('\'). This hack is only ran when there is an explicit tags path override.

char* PLATFORM_API c_memory_fixups::tag_file_index_build_strchr_hack(char* str, char val)
{
	return str + strlen(c_memory_fixups::_override_paths.tags.path);
}
void c_memory_fixups::tag_file_index_build_strchr_hack_initialize()
{
	FUNC_PTR(TAG_FILE_INDEX_BUILD_STRCHR_CALL, FUNC_PTR_NULL, 0x445C9B, 0x4FEACB);

	Memory::WriteRelativeCall(tag_file_index_build_strchr_hack, 
		GET_FUNC_VPTR(TAG_FILE_INDEX_BUILD_STRCHR_CALL));
}
#endif

void c_memory_fixups::FixupsInitializeDataPaths(cstring data_override)
{
#if PLATFORM_TYPE == PLATFORM_TOOL
	if(data_override == nullptr) return;


	strcpy_s(_override_paths.data.path, data_override);
	{
		// remove trailing slash if present
		size_t length = strlen(_override_paths.data.path);
		char& last_char = _override_paths.data.path[length-1];

		if(last_char == '\\' || last_char == '/')
			last_char = '\0';
	}

	static cstring* K_DATA_PATH_REFERENCE_FIXUPS[] = { // "data"
		CAST_PTR(cstring*, 0x448D55), CAST_PTR(cstring*, 0x458E92), 
	};
	for(int32 x = 0; x < NUMBEROF(K_DATA_PATH_REFERENCE_FIXUPS); x++)
		*K_DATA_PATH_REFERENCE_FIXUPS[x] = _override_paths.data.path;


	strcpy_s(_override_paths.data.path_with_slash, _override_paths.data.path);
	strcat_s(_override_paths.data.path_with_slash, "\\");

	static cstring* K_DATA_PATH_REFERENCE_FIXUPS_WITH_SLASH[] = { // "data\"
		CAST_PTR(cstring*, 0x414CEC),
	};
	for(auto ptr : K_DATA_PATH_REFERENCE_FIXUPS_WITH_SLASH)
		*ptr = _override_paths.data.path_with_slash;
#endif
}
void c_memory_fixups::FixupsInitializeMapsPaths(cstring maps_override)
{
#if PLATFORM_TYPE == PLATFORM_TOOL
	if(maps_override == nullptr) return;

	strcpy_s(_override_paths.maps.path, maps_override);
	{
		// remove trailing slash if present
		size_t length = strlen(_override_paths.maps.path);
		char& last_char = _override_paths.maps.path[length-1];

		if(last_char == '\\' || last_char == '/')
			last_char = '\0';
	}

	static cstring* K_MAPS_PATH_REFERENCE_FIXUPS[] = { // "maps"
		CAST_PTR(cstring*, 0x455453), CAST_PTR(cstring*, 0x455641),
	};
	for(int32 x = 0; x < NUMBEROF(K_MAPS_PATH_REFERENCE_FIXUPS); x++)
		*K_MAPS_PATH_REFERENCE_FIXUPS[x] = _override_paths.maps.path;


	strcpy_s(_override_paths.maps.path_with_slash, _override_paths.maps.path);
	strcat_s(_override_paths.maps.path_with_slash, "\\");

	static cstring* K_MAPS_PATH_REFERENCE_FIXUPS2[] = { // "maps\"
		CAST_PTR(cstring*, 0x569851),
	};
	for(auto ptr : K_MAPS_PATH_REFERENCE_FIXUPS2)
		*ptr = _override_paths.maps.path_with_slash;


	strcpy_s(_override_paths.maps.temp_uncompressed_cache_file, _override_paths.maps.path);
	strcat_s(_override_paths.maps.temp_uncompressed_cache_file, "\\temporary uncompressed cache file.bin");

	static cstring* K_TEMP_CACHE_FILE_PATH_REFERENCE_FIXUPS[] = { // "temporary uncompressed cache file.bin"
		CAST_PTR(cstring*, 0x4B9046), CAST_PTR(cstring*, 0x4B929E), 
		CAST_PTR(cstring*, 0x4B945C), CAST_PTR(cstring*, 0x4B9467),
	};
	for(auto ptr : K_TEMP_CACHE_FILE_PATH_REFERENCE_FIXUPS)
		*ptr = _override_paths.maps.temp_uncompressed_cache_file;
#endif
}

void c_memory_fixups::FixupsInitializeTagPaths(cstring tags_override, cstring tags_name_override)
{
	if(tags_override == nullptr) return;

	strcpy_s(_override_paths.tags.path, tags_override);
	{
		_strlwr(_override_paths.tags.path);
		// remove trailing slash if present
		size_t length = strlen(_override_paths.tags.path);
		char& last_char = _override_paths.tags.path[length - 1];

		if (last_char == '\\' || last_char == '/')
			last_char = '\0';
	}

	char local_tags_name[64];
	strcpy_s(local_tags_name, tags_name_override);
	{
		_strlwr(local_tags_name);
		size_t length = strlen(local_tags_name);
		char& last_char = local_tags_name[length-1];

		if(last_char == '\\' || last_char == '/')
			last_char = '\0';
	}

	strcpy_s(_override_paths.tags.path_with_slash, _override_paths.tags.path);
	strcat_s(_override_paths.tags.path_with_slash, "\\");


	static cstring* K_TAGS_PATH_REFERENCE_FIXUPS[] = { // "tags"
#if PLATFORM_TYPE == PLATFORM_GUERILLA
		// The first reference is in a start-up thunk. If we didn't initialize before those thunks were ran, 
		// we'd need to handled it with a 'strcpy'.
		CAST_PTR(cstring*, 0x41453B), CAST_PTR(cstring*, 0x439C61), CAST_PTR(cstring*, 0x43A096),
		CAST_PTR(cstring*, 0x43BAEE), CAST_PTR(cstring*, 0x43BEB8), CAST_PTR(cstring*, 0x43C0F8),

#elif PLATFORM_TYPE == PLATFORM_TOOL
		CAST_PTR(cstring*, 0x444F36), CAST_PTR(cstring*, 0x445C2E), CAST_PTR(cstring*, 0x445FF8), 

#elif PLATFORM_TYPE == PLATFORM_SAPIEN
		CAST_PTR(cstring*, 0x4FCBD1), CAST_PTR(cstring*, 0x4FD006), CAST_PTR(cstring*, 0x4FEA5E), 
		CAST_PTR(cstring*, 0x4FEE28), CAST_PTR(cstring*, 0x4FF068), 

#endif
	};
	for(auto ptr : K_TAGS_PATH_REFERENCE_FIXUPS)
		*ptr = _override_paths.tags.path;


#if PLATFORM_TYPE != PLATFORM_TOOL
	// Replace the a few key GetWorkingDir calls with our own so it uses 
	// the user's defined root path
	static void* K_GET_WORKING_DIR_CALLS[] = {
	#if PLATFORM_TYPE == PLATFORM_GUERILLA
		CAST_PTR(void*, 0x415ABB), CAST_PTR(void*, 0x415DF8), 
	#elif PLATFORM_TYPE == PLATFORM_SAPIEN
		CAST_PTR(void*, 0x485034), CAST_PTR(void*, 0x485248), CAST_PTR(void*, 0x4E820C), CAST_PTR(void*, 0x4E71E9),
		CAST_PTR(void*, 0x4E1272), CAST_PTR(void*, 0x4E0F36), CAST_PTR(void*, 0x489052)
	#endif
	};
	for(auto ptr : K_GET_WORKING_DIR_CALLS)
		Memory::CreateHookRelativeCall(GetCurrentDirectoryHack, 
			ptr, Enums::_x86_opcode_nop);


	// "tags\"
	strcpy_s(_override_paths.tags.folder_name_with_slash_single, local_tags_name);
	strcat_s(_override_paths.tags.folder_name_with_slash_single, "\\");
	// ".\tags"
	strcpy_s(_override_paths.tags.folder_name_with_slash_single_relative, ".\\");
	strcat_s(_override_paths.tags.folder_name_with_slash_single_relative, local_tags_name);

	// "\tags\"
	strcpy_s(_override_paths.tags.folder_name_with_slash, "\\");
	strcat_s(_override_paths.tags.folder_name_with_slash, _override_paths.tags.folder_name_with_slash_single);
	// ".\tags\"
	strcpy_s(_override_paths.tags.folder_name_with_slash_relative, ".");
	strcat_s(_override_paths.tags.folder_name_with_slash_relative, _override_paths.tags.folder_name_with_slash);

	static cstring* K_TAGS_NAME_REFERENCE_FIXUPS[] = { // "\tags\"
	#if PLATFORM_TYPE == PLATFORM_GUERILLA
		CAST_PTR(cstring*, 0x415B5A), CAST_PTR(cstring*, 0x415E3B), 
	#elif PLATFORM_TYPE == PLATFORM_SAPIEN
		CAST_PTR(cstring*, 0x485060), CAST_PTR(cstring*, 0x485065), 
		CAST_PTR(cstring*, 0x48526A), CAST_PTR(cstring*, 0x48526F), 
	#endif
	};
	for(auto ptr : K_TAGS_NAME_REFERENCE_FIXUPS)
		*ptr = _override_paths.tags.folder_name_with_slash;
#endif

#if PLATFORM_TYPE != PLATFORM_GUERILLA
	tag_file_index_build_strchr_hack_initialize();
#endif


#if PLATFORM_TYPE == PLATFORM_GUERILLA
	static cstring* K_TAGS_PATH_REFERENCE_FIXUPS2[] = { // "tags\"
		CAST_PTR(cstring*, 0x41B112), CAST_PTR(cstring*, 0x415FB4), CAST_PTR(cstring*, 0x41601F), 
	};
	for(auto ptr : K_TAGS_PATH_REFERENCE_FIXUPS2)
		*ptr = _override_paths.tags.folder_name_with_slash_single;


#if 0 // this code no longer applies since we hook all the tools before their entry points even execute
	static char* K_TAGS_PATH_DIRECT_FIXUPS[] = {
		CAST_PTR(char*, 0xAA0278),	
	};
	for(int32 x = 0; x < NUMBEROF(K_TAGS_PATH_DIRECT_FIXUPS); x++)
		strcpy_s(K_TAGS_PATH_DIRECT_FIXUPS[x], MAX_PATH, _override_paths.tags.path);
#endif


	byte name_length = 0; // In guerilla, there are a few places we have to fix-up some hardcoded lengths too

	static byte* K_TAGS_NAME_LENGTH_FIXUPS[] = { // "\tags\"
		CAST_PTR(byte*, 0x415B58), CAST_PTR(byte*, 0x415E39), 
	};
	name_length = CAST(byte, strlen(_override_paths.tags.folder_name_with_slash));
	for(auto ptr : K_TAGS_NAME_LENGTH_FIXUPS)
		*ptr = name_length;

	static byte* K_TAGS_PATH_REFERENCE_LENGTH_FIXUPS2[] = { // "tags\"
		CAST_PTR(byte*, 0x41601D), 
	};
	name_length = CAST(byte, strlen(_override_paths.tags.folder_name_with_slash_single));
	for(auto ptr : K_TAGS_PATH_REFERENCE_LENGTH_FIXUPS2)
		*ptr = name_length;
#endif


#if PLATFORM_TYPE == PLATFORM_SAPIEN
	strcpy_s(_override_paths.tags.folder_name_with_levels, _override_paths.tags.folder_name_with_slash_single);
	strcat_s(_override_paths.tags.folder_name_with_levels, "levels");
	
	strcpy_s(_override_paths.tags.root_with_tags_levels, _override_paths.tags.path_with_slash);
	strcat_s(_override_paths.tags.root_with_tags_levels, "levels");


	static char* K_WORKING_DIR_DIRECT_FIXUPS[] = {
		CAST_PTR(char*, 0xB17F20),	
	};
	for(auto ptr : K_WORKING_DIR_DIRECT_FIXUPS)
		strcpy_s(ptr, MAX_PATH, _override_paths.root);


	static cstring* K_TAGS_PATH_REFERENCE_FIXUPS_SLASH_RELATIVE[] = { // ".\tags\"
		CAST_PTR(cstring*, 0x4890E2), CAST_PTR(cstring*, 0x4E0F6F), CAST_PTR(cstring*, 0x4E0FD8), 
		CAST_PTR(cstring*, 0x4E12AB), CAST_PTR(cstring*, 0x4E1320), CAST_PTR(cstring*, 0x4E7215), 
		CAST_PTR(cstring*, 0x4E728A), CAST_PTR(cstring*, 0x4E8239), CAST_PTR(cstring*, 0x4E82A2), 
	};
	for(auto ptr : K_TAGS_PATH_REFERENCE_FIXUPS_SLASH_RELATIVE)
		*ptr = _override_paths.tags.folder_name_with_slash_relative;


	static cstring* K_TAGS_PATH_REFERENCE_FIXUPS_SLASH_SINGLE_RELATIVE[] = { // ".\tags"
		CAST_PTR(cstring*, 0x4890A7),
	};
	for(auto ptr : K_TAGS_PATH_REFERENCE_FIXUPS_SLASH_SINGLE_RELATIVE)
		*ptr = _override_paths.tags.folder_name_with_slash_single_relative;


	static cstring* K_TAGS_PATH_REFERENCE_FIXUPS_WITH_LEVELS[] = { // "tags\levels"
		CAST_PTR(cstring*, 0x4E0EF7), CAST_PTR(cstring*, 0x4E1233),
	};
	for(auto ptr : K_TAGS_PATH_REFERENCE_FIXUPS_WITH_LEVELS)
		*ptr = _override_paths.tags.folder_name_with_levels;

	char* registry_scnr_path = CAST_PTR(char*, 0xA25F98);
	strcpy_s(registry_scnr_path, MAX_PATH, _override_paths.tags.folder_name_with_levels);	
		
	static cstring* K_INITIAL_OPEN_DIRECTORY_REFERENCE_FIXUPS[] = { // "*root path*\tags\levels"
		CAST_PTR(cstring*, 0x4E0EF7), CAST_PTR(cstring*, 0x4E1233), CAST_PTR(cstring*, 0x4E81D0),
		CAST_PTR(cstring*, 0x4E71A9)
	};
	for(auto ptr : K_INITIAL_OPEN_DIRECTORY_REFERENCE_FIXUPS)
		*ptr = _override_paths.tags.root_with_tags_levels;

	// Remove reading and writing the last directory registry entry when using a custom profile
	static void* K_REGISTRY_QUERY_LAST_DIRECTORY_CALLS[] = {
		CAST_PTR(void*, 0x4E0EE5), CAST_PTR(void*, 0x4E1025), CAST_PTR(void*, 0x4E1221),
		CAST_PTR(void*, 0x4E13AF)
	};
	{
		byte null_code[5] = { // sizeof(s_call)
			Enums::_x86_opcode_nop, Enums::_x86_opcode_nop, Enums::_x86_opcode_nop, 
			Enums::_x86_opcode_nop, Enums::_x86_opcode_nop
		};
		for (auto ptr : K_REGISTRY_QUERY_LAST_DIRECTORY_CALLS)
			Memory::WriteMemory(ptr, null_code, 5);
	}
#endif
}

void c_memory_fixups::FixupsInitializeFilePaths()
{
	enum report_type : long_enum {
		_report_debug,
#if PLATFORM_TYPE != PLATFORM_GUERILLA
		_report_network,
		_report_tag_dump,
		_report_heap_dump,
		_report_gamestate,
		_report_sound_cache_dump,
		_report_game_time_statistics,
		_report_debug_bsp,
		_report_object_memory,
		_report_hs_doc,

	#if PLATFORM_TYPE == PLATFORM_SAPIEN
		_report_baggage,
		_report_message_delta_message_log,
		_report_sapien_model_index,
	#endif
#endif
		_report_type
	};
	static char k_file_path_fixup_names[][MAX_PATH] = {
		"debug.txt",

#if PLATFORM_TYPE != PLATFORM_GUERILLA
		"network.log",
		"tag_dump.txt",
		"heap_dump.txt",
		"gamestate.txt",
		"sound_cache_dump.txt",
		"game_time_statistics.txt",
		"debug_bsp.txt",
		"object_memory.txt",
		"hs_doc.txt",

	#if PLATFORM_TYPE == PLATFORM_SAPIEN
		"baggage.txt",
		"message_delta_message_log.txt",
		"sapien_model_index.dat",
	#endif
#endif
	}; BOOST_STATIC_ASSERT( NUMBEROF(k_file_path_fixup_names) == _report_type );

	cstring k_reports_path = nullptr;
	
	if (Settings::c_settings_cheape::IsCustomProfile())
	{
		auto& profile = Settings::c_settings_cheape::Profile();
		strcpy_s(_override_paths.root, profile.GetRootPath());
		{
			_strlwr(_override_paths.root);

			// remove trailing slash if present
			size_t length = strlen(_override_paths.root);
			char& last_char = _override_paths.root[length - 1];

			if (last_char == '\\' || last_char == '/')
				last_char = '\0';
		}

		FixupsInitializeDataPaths(profile.GetDataPath());
		FixupsInitializeMapsPaths(profile.GetMapsPath());
		FixupsInitializeTagPaths(profile.GetTagsPath(), profile.GetTagsName());
	}

	k_reports_path = Settings::ReportsPath();

	// don't use a range-based for loop here because auto deduces to just char*, so we lose the implicit length info needed for the secure string functions
	for(int x = 0; x < NUMBEROF(k_file_path_fixup_names); x++)
	{
		// Temporarily store the original relative path
		char path[MAX_PATH];
		strcpy_s(path, k_file_path_fixup_names[x]);

		// Prepend the reports path then copy back the file's path
		strcpy_s(k_file_path_fixup_names[x], k_reports_path);
		strcat_s(k_file_path_fixup_names[x], path);
	}


	struct report_fixup {
		report_type type;
		cstring* address;
	}; static report_fixup fixups[] = {
		{_report_debug,						CAST_PTR(cstring*,PLATFORM_VALUE(0x420565,	0x42CAB1,	0x4173A1))},
//		{_report_debug,						CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,	nullptr,	nullptr))},

#if PLATFORM_TYPE != PLATFORM_GUERILLA
		{_report_network,					CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		0x42C983,	0x417223))},
		{_report_network,					CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		nullptr,	0x6F1073))},
		{_report_tag_dump,					CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		0x433609,	0x4FA199))},
		{_report_heap_dump,					CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		0x43E45F,	0x501CFF))},
		{_report_heap_dump,					CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		0x43E5E6,	0x501E86))},
		{_report_gamestate,					CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		nullptr,	0x50B48F))},
		{_report_gamestate,					CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		0x4561F6,	0x50B5A6))},
		{_report_gamestate,					CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		nullptr,	0x50B6B6))},
		{_report_sound_cache_dump,			CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		0x4765F9,	0x51E0E9))},
		{_report_game_time_statistics,		CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		0x47729F,	0x51EF1F))},
		{_report_debug_bsp,					CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		0x423282,	0x521F7A))},
		{_report_object_memory,				CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		0x4A1D24,	0x563534))},
		{_report_hs_doc,					CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		0x4C2AF5,	0x584BC5))},

	#if PLATFORM_TYPE == PLATFORM_SAPIEN
		{_report_baggage,					CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		nullptr,	0x4EF540))},
		{_report_message_delta_message_log,	CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		nullptr,	0x6EC3B8))},

		{_report_sapien_model_index,		CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		nullptr,	0x4D115A))},
		{_report_sapien_model_index,		CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,		nullptr,	0x4D11D2))},
	#endif
#endif
		//{_report_,	CAST_PTR(cstring*,PLATFORM_VALUE(nullptr,	,	))},
	};

	for(auto fixup : fixups)
		if(fixup.address != nullptr)
			*fixup.address = k_file_path_fixup_names[ fixup.type ];
}