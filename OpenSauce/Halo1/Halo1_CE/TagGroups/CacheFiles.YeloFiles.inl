/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C)  Kornner Studios (http://kornner.com)

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

static int __cdecl CacheFormatPathHack(string256 buffer, cstring format,
	cstring root_directory, cstring maps_folder, cstring map_name)
{
	int access, result;

	result = sprintf_s(buffer, sizeof(string256), format, root_directory, maps_folder, map_name);
	access = _access_s(buffer, 0);

	if(access == NONE && errno == ENOENT)
	{
		result = sprintf_s(buffer, sizeof(string256), "%s%s%s.yelo", root_directory, maps_folder, map_name);
		access = _access_s(buffer, 0);

		if(access == NONE && errno == ENOENT)
			YELO_DEBUG_FORMAT("CacheFormatPathHack is about to fail on [%s]", buffer);
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

	if(access == NONE && errno == ENOENT)
	{
		format = format[2] == '\\' ? "%s\\%s%s.yelo" : "%s%s%s.yelo";

		result = _snprintf_s(buffer, buffer_size, _TRUNCATE, format, root_directory, maps_folder, map_name);
		access = _access_s(buffer, 0);

		if(access == NONE && errno == ENOENT)
			YELO_DEBUG_FORMAT("CacheFormatPathHackN is about to fail on [%s]", buffer);
	}

	return result;
}
static void CacheFormatPathHackInitialize()
{
	for(int32 x = 0; x < NUMBEROF(K_CACHE_PATH_SPRINTF_CALLS); x++)
		Memory::WriteRelativeCall(CacheFormatPathHack, K_CACHE_PATH_SPRINTF_CALLS[x]);
	for(int32 x = 0; x < NUMBEROF(K_CACHE_PATH_SNPRINTF_CALLS)-1; x++)
		Memory::WriteRelativeCall(CacheFormatPathHackN, K_CACHE_PATH_SNPRINTF_CALLS[x]);
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