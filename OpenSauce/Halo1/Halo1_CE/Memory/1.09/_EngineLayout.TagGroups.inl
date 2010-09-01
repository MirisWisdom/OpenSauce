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

		ENGINE_PTR(t_multiplayer_map_data, multiplayer_maps,0x6ADE34, 0x616664);

		ENGINE_PTR(s_cache_file_globals, cache_file_globals,0x643060, 0x5B4E50);

		FUNC_PTR(MULTIPLAYER_MAP_LIST_INITIALIZE,	0x4982B0, 0x48DCA0);
		FUNC_PTR(MULTIPLAYER_MAP_LIST_DISPOSE,		0x498330, 0x48DD20);
	};

//////////////////////////////////////////////////////////////////////////
// TagGroups.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_TAGGROUPS_TAG_GROUPS
	ENGINE_DPTR(s_cache_tag_instance, tag_instances,0x816DE4, 0x6E2CA4);

	FUNC_PTR(TAG_ITERATOR_NEXT,	0x442A20, 0x4435B0);


#else
	#error Undefined engine layout include for: __EL_INCLUDE_TAGGROUPS
#endif