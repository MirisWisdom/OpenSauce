/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

static int __cdecl CacheFormatPathHack(string256 buffer, cstring format,
	cstring root_directory, cstring maps_folder, cstring map_name)
{
	int access, result;

	result = sprintf_s(buffer, sizeof(string256), format, root_directory, maps_folder, map_name);
	access = _access_s(buffer, 0);

	if(access == ENOENT)
	{
		result = sprintf_s(buffer, sizeof(string256), "%s%s%s.yelo", root_directory, maps_folder, map_name);
		access = _access_s(buffer, 0);

		if(access == ENOENT)
			YELO_DEBUG_FORMAT("CacheFormatPathHack is about to fail on [%s] %d %d", buffer, access, errno);
	}

	return result;
}
static int __cdecl CacheFormatPathHackN(char* buffer, size_t max_count, cstring format,
	cstring root_directory, cstring maps_folder, cstring map_name)
{
	const size_t buffer_size = max_count+1;
	int access, result;

	result = _snprintf_s(buffer, buffer_size, _TRUNCATE, format, root_directory, maps_folder, map_name);
	access = _access_s(buffer, 0);

	if(access == ENOENT)
	{
		format = format[2] == '\\' ? "%s\\%s%s.yelo" : "%s%s%s.yelo";

		result = _snprintf_s(buffer, buffer_size, _TRUNCATE, format, root_directory, maps_folder, map_name);
		access = _access_s(buffer, 0);

		if(access == ENOENT)
			YELO_DEBUG_FORMAT("CacheFormatPathHackN is about to fail on [%s]", buffer);
	}

	return result;
}
static bool CacheFileReadHeaderHackImpl(cstring relative_map_name, s_cache_header& out_header)
{
	bool yelo_is_ok;
	return ReadHeader(relative_map_name, out_header, yelo_is_ok, true);
}
static API_FUNC_NAKED bool __cdecl CacheFileReadHeaderHack()
{
	__asm {
		// eax (client), ecx (dedi) = relative_map_name
		// esi = out_header

		push	esi
		push	PLATFORM_VALUE(eax, ecx)
		call	CacheFileReadHeaderHackImpl
		retn
	}
}
static void CacheFormatPathHackInitialize()
{
	for(int32 x = 0; x < NUMBEROF(K_CACHE_PATH_SPRINTF_CALLS); x++)
		Memory::WriteRelativeCall(CacheFormatPathHack, K_CACHE_PATH_SPRINTF_CALLS[x]);
	for(int32 x = 0; x < NUMBEROF(K_CACHE_PATH_SNPRINTF_CALLS)-1; x++)
		Memory::WriteRelativeCall(CacheFormatPathHackN, K_CACHE_PATH_SNPRINTF_CALLS[x]);

	// Redirect all game calls to cache_file_read_header to our implementation which supports .yelo validation
	for(int32 x = 0; x < NUMBEROF(K_CACHE_FILE_READ_HEADER_CALLS); x++)
		Memory::WriteRelativeCall(CacheFileReadHeaderHack, K_CACHE_FILE_READ_HEADER_CALLS[x]);
}


static void MapListInitializeYelo()
{
	WIN32_FIND_DATAA fd;
	HANDLE h = FindFirstFileA("maps\\*.yelo", &fd);
	if(h != INVALID_HANDLE_VALUE)
	{
		*GET_PTR2(map_list_map_extension_ref) = ".yelo";

		do Engine::MapListAddMap(fd.cFileName);
		while(FindNextFileA(h, &fd));

		*GET_PTR2(map_list_map_extension_ref) = GET_PTR(map_list_map_extension);

		FindClose(h);
	}
}