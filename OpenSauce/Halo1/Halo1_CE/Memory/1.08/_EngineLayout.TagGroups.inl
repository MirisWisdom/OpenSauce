/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
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

		ENGINE_PTR(t_multiplayer_map_data, multiplayer_maps,0x6ADD4C, 0x61654C);

		ENGINE_PTR(s_cache_file_globals, cache_file_globals,0x642F78, 0x5B4DB8);

		FUNC_PTR(MULTIPLAYER_MAP_LIST_INITIALIZE,	0x498280, 0x48DC70);
		FUNC_PTR(MULTIPLAYER_MAP_LIST_DISPOSE,		0x498300, 0x48DCF0);
	};

//////////////////////////////////////////////////////////////////////////
// TagGroups.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_TAGGROUPS_TAG_GROUPS
	ENGINE_DPTR(s_cache_tag_instance, tag_instances,0x816D04, 0x6E2C04);

	FUNC_PTR(TAG_ITERATOR_NEXT,	0x442A10, 0x4435A0);


#else
	#error Undefined engine layout include for: __EL_INCLUDE_TAGGROUPS
#endif